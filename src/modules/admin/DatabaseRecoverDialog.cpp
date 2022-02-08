#include "DatabaseRecoverDialog.h"
#include <wx/filedlg.h>
#include <DSFile.h>
#include "../lib/DatabaseLibrary.h"
#include "../lib/DatabaseType.h"
#include "../lib/EnterpriseController.h"
#include "../lib/EnterpriseEntity.h"

BEGIN_EVENT_TABLE(DatabaseRecoverDialog, wxDialog)
	EVT_BUTTON(ABORT_EVENT, DatabaseRecoverDialog::AbortEvent)
	EVT_BUTTON(RECOVER_EVENT, DatabaseRecoverDialog::RecoverEvent)
	EVT_BUTTON(BROWSE_EVENT, DatabaseRecoverDialog::BrowseEvent)
END_EVENT_TABLE()

DatabaseRecoverDialog::DatabaseRecoverDialog(
	wxWindow *parent,
	wxWindowID id,
	DSPostgreSQLDatabase *db
) : wxDialog(
	parent,
	id,
	_("Database recover"),
	wxDefaultPosition,
	wxDefaultSize
) {
	this->db = db;

	const wxString informations(_(
		"Please notice: The database must not exist. It will be created automatically."
	));

	mainPanel = new wxPanel(this, -1);
	mainBoxSizer = new wxBoxSizer(wxVERTICAL);

	informationTextCtrl = new wxTextCtrl(mainPanel, -1, informations, wxDefaultPosition, wxSize(-1, 150), wxTE_READONLY | wxTE_MULTILINE);
	mainBoxSizer->Add(informationTextCtrl, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 15);

	mainBoxSizer->AddSpacer(5);
	informationStaticLine = new wxStaticLine(mainPanel);
	mainBoxSizer->Add(informationStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	dataFlexGridSizer = new wxFlexGridSizer(2, 5, 5);
	dataFlexGridSizer->AddGrowableCol(1);

	filenameStaticText = new wxStaticText(mainPanel, -1, _("File name:"));
	filenameBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	filenameTextCtrl = new wxTextCtrl(mainPanel, -1);
	filenameButton = new wxButton(mainPanel, BROWSE_EVENT, _("browse"));
	filenameBoxSizer->Add(filenameTextCtrl, 1, wxALIGN_CENTER_VERTICAL);
	filenameBoxSizer->AddSpacer(5);
	filenameBoxSizer->Add(filenameButton, 0, wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(filenameStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(filenameBoxSizer, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	companyStaticText = new wxStaticText(mainPanel, -1, _("Company:"));
	companyTextCtrl = new wxTextCtrl(mainPanel, -1, _("New company"));
	dataFlexGridSizer->Add(companyStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(companyTextCtrl, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	hostnameStaticText = new wxStaticText(mainPanel, -1, _("Hostname:"));
	hostnameTextCtrl = new wxTextCtrl(mainPanel, -1, _N("localhost"));
	dataFlexGridSizer->Add(hostnameStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(hostnameTextCtrl, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	databaseStaticText = new wxStaticText(mainPanel, -1, _("Database:"));
	databaseTextCtrl = new wxTextCtrl(mainPanel, -1, _("new_company"));
	dataFlexGridSizer->Add(databaseStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(databaseTextCtrl, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	usernameStaticText = new wxStaticText(mainPanel, -1, _("Username:"));
	usernameTextCtrl = new wxTextCtrl(mainPanel, -1, _N("postgres"));
	dataFlexGridSizer->Add(usernameStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(usernameTextCtrl, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	passwordStaticText = new wxStaticText(mainPanel, -1, _("Password:"));
	passwordTextCtrl = new wxTextCtrl(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
	dataFlexGridSizer->Add(passwordStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(passwordTextCtrl, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	portStaticText = new wxStaticText(mainPanel, -1, _("Port:"));
	portTextCtrl = new wxTextCtrl(mainPanel, -1, _N("5432"));
	dataFlexGridSizer->Add(portStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(portTextCtrl, 0, wxALIGN_CENTER_VERTICAL);

	mainBoxSizer->Add(dataFlexGridSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 15);

	optionsStaticLine = new wxStaticLine(mainPanel);
	mainBoxSizer->Add(optionsStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	optionsBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	abortButton = new wxButton(mainPanel, ABORT_EVENT, _("Abort"));
	recoverButton = new wxButton(mainPanel, RECOVER_EVENT, _("Save"));
	optionsBoxSizer->Add(abortButton);
	optionsBoxSizer->AddSpacer(5);
	optionsBoxSizer->Add(recoverButton);

	mainBoxSizer->Add(optionsBoxSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxLEFT | wxBOTTOM, 15);
	
	mainPanel->SetSizer(mainBoxSizer);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(450, -1));

	recoverButton->SetDefault();
	databaseTextCtrlIsEdited = false;

	companyTextCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxTextEventHandler(DatabaseRecoverDialog::CompanyEvent), NULL, this);
	databaseTextCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxTextEventHandler(DatabaseRecoverDialog::DatabaseEvent), NULL, this);
}

void DatabaseRecoverDialog::AbortEvent(wxCommandEvent &event) {
	Abort();
}

void DatabaseRecoverDialog::RecoverEvent(wxCommandEvent &event) {
	Recover();
}

void DatabaseRecoverDialog::BrowseEvent(wxCommandEvent &event) {
	Browse();
}

void DatabaseRecoverDialog::Abort() {
	EndModal(wxID_CANCEL);
}

void DatabaseRecoverDialog::CompanyEvent(wxCommandEvent &event) {
	Company();
}

void DatabaseRecoverDialog::DatabaseEvent(wxCommandEvent &event) {
	Database();
}

void DatabaseRecoverDialog::Recover() {
	if(filenameTextCtrl->GetValue().IsEmpty()) {
		wxMessageDialog *errorMessage = new wxMessageDialog(this, _("Please select the file you want to recover."), _N("Recover"));
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	DSFile file(filenameTextCtrl->GetValue(), _N("r"));
	if(!file.IsOpened()) {
		wxMessageDialog *errorMessage = new wxMessageDialog(this, _("Could not open file: ") + filenameTextCtrl->GetValue(), _N("Recover"));
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	wxString sql;
	if(!file.ReadAll(&sql)) {
		wxMessageDialog *errorMessage = new wxMessageDialog(this, _("Could not read file: ") + filenameTextCtrl->GetValue(), _N("Recover"));
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		file.Close();
		return;
	}

	if(!file.Close()) {
		wxMessageDialog *errorMessage = new wxMessageDialog(this, _("Could not close file: ") + filenameTextCtrl->GetValue(), _N("Recover"));
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}


	wxBeginBusyCursor();

	wxString message;
	DatabaseType databaseType;

	databaseType.SetHostname(hostnameTextCtrl->GetValue());
	databaseType.SetDatabase(databaseTextCtrl->GetValue());
	databaseType.SetUsername(usernameTextCtrl->GetValue());
	databaseType.SetPassword(passwordTextCtrl->GetValue());
	databaseType.SetPort(portTextCtrl->GetValue());

	bool ok = false;

	DatabaseLibrary databaseLibrary;
	switch(databaseLibrary.CreateDatabase(databaseType, sql)) {
		case DATABASE_LIBRARY_OK:
			message = _("Database was created and the data was imported.");
			ok      = true;
			break;
		case DATABASE_LIBRARY_ERROR_CONNECTION:
			message = _("Could not connect to database postgres or template1. Could not create database.");
			break;
		case DATABASE_LIBRARY_ERROR_CREATION:
			message = _("Could not create database. Unknown error.");
			break;
		case DATABASE_LIBRARY_ERROR_CONNECT:
			message = _("Database was created, but there was an error on importing structure because the connection to the new database could not be established. You cannot use this database.");
			break;
		case DATABASE_LIBRARY_ERROR_INSERT:
			message = _("Database was created, but there was an error on importing structure. You cannot use this database.");
			break;
		case DATABASE_LIBRARY_ERROR_EXISTS:
			message = _("Database exists.");
			break;
		default:
			message = _("Unknown error. Could not create database.");
			break;
	}

	wxEndBusyCursor();

	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		_("Create database")
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();

	if(ok) {
		EnterpriseEntity enterpriseEntity(db);
		enterpriseEntity.SetValue(_N("company"), companyTextCtrl->GetValue());
		enterpriseEntity.SetValue(_N("hostname"), hostnameTextCtrl->GetValue());
		enterpriseEntity.SetValue(_N("database"), databaseTextCtrl->GetValue());
		enterpriseEntity.SetValue(_N("username"), usernameTextCtrl->GetValue());
		enterpriseEntity.SetValue(_N("password"), passwordTextCtrl->GetValue());
		enterpriseEntity.SetValue(_N("port"), DSCasts::ToUnsignedInt(portTextCtrl->GetValue()));
		
		EnterpriseController enterpriseController(db);
		if(enterpriseController.Insert(&enterpriseEntity)) {
			EndModal(wxID_OK);
		} else {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not add the company database to the Warehouse database. Please do this yourself in the Enterprise Configuration Dialog of Warehouse."),
				_("Error")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
			EndModal(wxID_CANCEL);
		}
	}
}

void DatabaseRecoverDialog::Browse() {
	wxFileDialog *openFileDialog = new wxFileDialog(
		this,
		_("Recover from"),
		path,
		filename,
		_N("*.backup"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);
	openFileDialog->Center();
	if(openFileDialog->ShowModal() == wxID_OK) {
		filenameTextCtrl->SetValue(openFileDialog->GetPath());

		path     = openFileDialog->GetDirectory();
		filename = openFileDialog->GetFilename();
	}
	openFileDialog->Destroy();
}

void DatabaseRecoverDialog::Company() {
	if(!databaseTextCtrlIsEdited || databaseTextCtrl->GetValue().IsEmpty()) {
		wxString database;
		wxString company(companyTextCtrl->GetValue());
		company = company.Lower();
		wxChar c;
		const unsigned int length = company.Length();
		for(unsigned int i = 0; i < length; i++) {
			c = company[i];
			if(c == ' ')
				database.Append(_N("_"));
			else if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z'))
				database.Append(c);
		}
		databaseTextCtrl->ChangeValue(database);
		databaseTextCtrlIsEdited = false;
	}
}

void DatabaseRecoverDialog::Database() {
	databaseTextCtrlIsEdited = true;
}
