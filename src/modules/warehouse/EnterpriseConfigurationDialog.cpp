#include "EnterpriseConfigurationDialog.h"
#include <DSCasts.h>
#include <DSPostgreSQLDatabase.h>
#include <DSFile.h>
#include "UserDialog.h"
#include "EnterpriseCreateDatabaseDialog.h"
#include "YesNoDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(EnterpriseConfigurationDialog, wxDialog)
	EVT_LISTBOX_DCLICK(XRCID("enterpriseListBox"), EnterpriseConfigurationDialog::EditEvent)
	EVT_MENU(CANCEL_EVENT, EnterpriseConfigurationDialog::CancelKeyEvent)
	EVT_MENU(NEW_EVENT, EnterpriseConfigurationDialog::NewKeyEvent)
	EVT_MENU(REMOVE_EVENT, EnterpriseConfigurationDialog::RemoveKeyEvent)
	EVT_MENU(SAVE_EVENT, EnterpriseConfigurationDialog::SaveKeyEvent)
	EVT_MENU(NEW_DATABASE_EVENT, EnterpriseConfigurationDialog::NewDatabaseKeyEvent)
	EVT_MENU(TEST_CONNECTION_EVENT, EnterpriseConfigurationDialog::TestConnectionKeyEvent)
END_EVENT_TABLE()

EnterpriseConfigurationDialog::EnterpriseConfigurationDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);

	this->rit               = rit;
	this->editingEnterprise = NULL;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("EnterpriseConfigurationDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("EnterpriseConfigurationDialog"));

	mainPanel                            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());

	enterpriseListBox                    = XRCCTRL(*this, "enterpriseListBox", wxListBox);
	newToolbarButton                     = XRCCTRL(*this, "newToolbarButton", ToolbarButton);
	removeToolbarButton                  = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	saveToolbarButton                    = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	newDatabaseToolbarButton             = XRCCTRL(*this, "newDatabaseToolbarButton", ToolbarButton);
	testConnectionToolbarButton          = XRCCTRL(*this, "testConnectionToolbarButton", ToolbarButton);
	cancelToolbarButton                  = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	closeToolbarButton                   = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	companyTextCtrl                      = XRCCTRL(*this, "companyTextCtrl", wxTextCtrl);
	hostnameComboBox                     = XRCCTRL(*this, "hostnameComboBox", wxComboBox);
	databaseNameTextCtrl                 = XRCCTRL(*this, "databaseNameTextCtrl", wxTextCtrl);
	usernameComboBox                     = XRCCTRL(*this, "usernameComboBox", wxComboBox);
	passwordTextCtrl                     = XRCCTRL(*this, "passwordTextCtrl", wxTextCtrl);
	portComboBox                         = XRCCTRL(*this, "portComboBox", wxComboBox);

	CONNECT_TOOLBARBUTTON(newToolbarButton, EnterpriseConfigurationDialog::NewEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, EnterpriseConfigurationDialog::RemoveEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, EnterpriseConfigurationDialog::SaveEvent);
	CONNECT_TOOLBARBUTTON(newDatabaseToolbarButton, EnterpriseConfigurationDialog::NewDatabaseEvent);
	CONNECT_TOOLBARBUTTON(testConnectionToolbarButton, EnterpriseConfigurationDialog::TestConnectionEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, EnterpriseConfigurationDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(closeToolbarButton, EnterpriseConfigurationDialog::CloseEvent);

	wxAcceleratorEntry acceleratorEntries[7];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'n',  NEW_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,    (int) '+',  NEW_DATABASE_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL,    (int) 't',  TEST_CONNECTION_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL,    (int) 'd',  REMOVE_EVENT);
	wxAcceleratorTable acceleratorTable(7, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(750, 480));

	SetModus(false);

	LoadEnterprises();

	ICON();
}

EnterpriseConfigurationDialog::~EnterpriseConfigurationDialog() {
	if(editingEnterprise && editingEnterprise->Get(_N("id"))->ToUnsignedInt() == 0)
		delete editingEnterprise;
	UnloadEnterprises();
}

void EnterpriseConfigurationDialog::NewEvent(wxMouseEvent &event) {
	editingEnterprise = new EnterpriseEntity(rit->GetEnterprisesDatabase());
	New();
	SKIP();
}

void EnterpriseConfigurationDialog::NewKeyEvent(wxCommandEvent &event) {
	editingEnterprise = new EnterpriseEntity(rit->GetEnterprisesDatabase());
	New();
}

void EnterpriseConfigurationDialog::EditEvent(wxCommandEvent &event) {
	editingEnterprise = static_cast<EnterpriseEntity *>(event.GetClientData());
	if(!editingEnterprise) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please select the enterprise you want to edit"),
			_("Edit enterprise")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	}

	Edit();
}

void EnterpriseConfigurationDialog::RemoveEvent(wxMouseEvent &event) {
	SKIP();
	Remove();
}

void EnterpriseConfigurationDialog::RemoveKeyEvent(wxCommandEvent &event) {
	Remove();
}

void EnterpriseConfigurationDialog::SaveEvent(wxMouseEvent &event) {
	if(!Save())
		SKIP();
}

void EnterpriseConfigurationDialog::SaveKeyEvent(wxCommandEvent &event) {
	if(!Save())
		SKIP();
}

void EnterpriseConfigurationDialog::NewDatabaseEvent(wxMouseEvent &event) {
	NewDatabase();
	SKIP();
}

void EnterpriseConfigurationDialog::NewDatabaseKeyEvent(wxCommandEvent &event) {
	NewDatabase();
}

void EnterpriseConfigurationDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
}

void EnterpriseConfigurationDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void EnterpriseConfigurationDialog::CloseEvent(wxMouseEvent &event) {
	SKIP();
	Close();
}

void EnterpriseConfigurationDialog::TestConnectionEvent(wxMouseEvent &event) {
	TestConnection();
	SKIP();
}

void EnterpriseConfigurationDialog::TestConnectionKeyEvent(wxCommandEvent &event) {
	TestConnection();
}

void EnterpriseConfigurationDialog::New() {
	SetModus(true);
}

void EnterpriseConfigurationDialog::Edit() {
	SetModus(true);
	SetData();
}

void EnterpriseConfigurationDialog::Remove() {
	if(!removeToolbarButton->GetEnabled())
		return;

	const int selection = enterpriseListBox->GetSelection();
	if(selection == wxNOT_FOUND) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please select the enterprise you want to delete."),
			_("Delete enterprise")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	}

	EnterpriseEntity *enterprise = static_cast<EnterpriseEntity *>(enterpriseListBox->GetClientData(selection));
	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you want to delete the enterprise ") + enterprise->Get(_N("company"))->ToString() + _("? The database will not be deleted automatically, you must remove it yourself."),
		_("Delete"),
		_("Delete"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		EnterpriseController enterpriseController(rit->GetEnterprisesDatabase());
		if(enterpriseController.Delete(enterprise)) {
			enterpriseListBox->Delete(static_cast<unsigned int>(selection));
			delete enterprise;
		} else {
			wxMessageDialog *messageDialog = new wxMessageDialog(
				this,
				_("The enterprise could not be deleted. A database error occured."),
				_("Delete")
			);
			messageDialog->Center();
			messageDialog->ShowModal();
			messageDialog->Destroy();
		}
	}
	yesNoDialog->Destroy();
}

bool EnterpriseConfigurationDialog::Save() {
	if(!saveToolbarButton->GetEnabled())
		return false;

	wxString oldCompany(editingEnterprise->Get(_N("company"))->ToString());
	TransferData();

	if(editingEnterprise->Get(_N("port"))->ToUnsignedInt() == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please specify a port greater than zero."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return false;
	}

	EnterpriseController enterpriseController(rit->GetEnterprisesDatabase());

	if(enterpriseController.Exists(
		_N("company"),
		editingEnterprise->Get(_N("company"))->ToString(),
		oldCompany
	)) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("A company with this name already exists."),
			_("Save")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return false;
	}

	bool ok     = false;
	if(editingEnterprise->Get(_N("id"))->ToUnsignedInt() == 0)
		ok = enterpriseController.Insert(editingEnterprise);
	else
		ok = enterpriseController.Update(editingEnterprise);
	
	if(ok) {
		SetModus(false);
		ReloadEnterprises();
	} else {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not save enterprise: a database error occured."),
			_("Save")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}

	return ok;
}

void EnterpriseConfigurationDialog::NewDatabase() {
	if(!newDatabaseToolbarButton->GetEnabled())
		return;
	
	wxString filename(rit->GRP(_N("database"), _N("enterprise.sql")));

	wxString message;
	wxString sql;

	wxBeginBusyCursor();
	DSFile file(filename, _N("r"));
	if(file.IsOpened()) {
	//#ifdef __WXMSW__
		if(!file.ReadAll(&sql))
	//#else
	//	if(!file.ReadAll(&sql, wxConvISO8859_1))
	//#endif
			message = _("Could not create database, because could not read file: ") + filename;

		if(!file.Close()) {
			if(message.IsEmpty())
				message = _("Could not create database, because could not close file: ") + filename;
		}
	} else {
		message = _("Could not create database, because could not open file: ") + filename;
	}
	wxEndBusyCursor();

	if(!message.IsEmpty()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			message,
			_("Create database")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	DatabaseType databaseType;

	databaseType.SetHostname(hostnameComboBox->GetValue());
	databaseType.SetDatabase(databaseNameTextCtrl->GetValue());
	databaseType.SetUsername(usernameComboBox->GetValue());
	databaseType.SetPassword(passwordTextCtrl->GetValue());
	databaseType.SetPort(portComboBox->GetValue());

	EnterpriseCreateDatabaseDialog *enterpriseCreateDatabaseDialog = new EnterpriseCreateDatabaseDialog(
		this,
		-1,
		rit,
		companyTextCtrl->GetValue(),
		&databaseType,
		sql
	);
	enterpriseCreateDatabaseDialog->Center();
	enterpriseCreateDatabaseDialog->ShowModal();
	enterpriseCreateDatabaseDialog->Destroy();
}

void EnterpriseConfigurationDialog::Cancel() {
	SetModus(false);
}

void EnterpriseConfigurationDialog::Close() {
	EndModal(wxID_OK);
}

void EnterpriseConfigurationDialog::TestConnection() {
	if(!testConnectionToolbarButton->GetEnabled())
		return;

	if(DSCasts::ToUnsignedInt(portComboBox->GetValue()) == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please specify a port greater than zero."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	wxString message(_("Could not connect to database"));

	DSPostgreSQLDatabase db;
	db.SetHostname(hostnameComboBox->GetValue());
	db.SetDatabase(databaseNameTextCtrl->GetValue());
	db.SetUsername(usernameComboBox->GetValue());
	db.SetPassword(passwordTextCtrl->GetValue());
	db.SetPort(portComboBox->GetValue());
	if(db.Connect()) {
		db.Disconnect();
		message = _("Connection to database could be established");
	}

	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		_("Test connection")
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();
}

void EnterpriseConfigurationDialog::TransferData() {
	editingEnterprise->SetValue(_N("company"), companyTextCtrl->GetValue());
	editingEnterprise->SetValue(_N("hostname"), hostnameComboBox->GetValue());
	editingEnterprise->SetValue(_N("database"), databaseNameTextCtrl->GetValue());
	editingEnterprise->SetValue(_N("username"), usernameComboBox->GetValue());
	editingEnterprise->SetValue(_N("password"), passwordTextCtrl->GetValue());
	editingEnterprise->SetValue(_N("port"), DSCasts::ToUnsignedInt(portComboBox->GetValue()));
}

void EnterpriseConfigurationDialog::SetData() {
	companyTextCtrl->SetValue(editingEnterprise->Get(_N("company"))->ToString());
	hostnameComboBox->SetValue(editingEnterprise->Get(_N("hostname"))->ToString());
	databaseNameTextCtrl->SetValue(editingEnterprise->Get(_N("database"))->ToString());
	usernameComboBox->SetValue(editingEnterprise->Get(_N("username"))->ToString());
	passwordTextCtrl->SetValue(editingEnterprise->Get(_N("password"))->ToString());
	portComboBox->SetValue(editingEnterprise->Get(_N("port"))->ToString());
}

unsigned int EnterpriseConfigurationDialog::LoadEnterprises() {
	unsigned int count = 0;
	EnterpriseEntity *enterprise = NULL;
	EnterpriseController enterpriseController(rit->GetEnterprisesDatabase());
	enterpriseController.Get();
	while(enterpriseController.Run()) {
		enterprise = new EnterpriseEntity(rit->GetEnterprisesDatabase());
		enterpriseController.Get(enterprise);
		enterpriseListBox->Append(
			enterprise->Get(_N("company"))->ToString(),
			enterprise
		);
		count++;
	}

	return count;
}

unsigned int EnterpriseConfigurationDialog::UnloadEnterprises() {
	EnterpriseEntity *enterprise = NULL;
	const unsigned int count = enterpriseListBox->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		enterprise = static_cast<EnterpriseEntity *>(enterpriseListBox->GetClientData(i));
		delete enterprise;
	}
	enterpriseListBox->Clear();

	return count;
}

unsigned int EnterpriseConfigurationDialog::ReloadEnterprises() {
	UnloadEnterprises();
	return LoadEnterprises();
}

void EnterpriseConfigurationDialog::SetModus(const bool edit) {
	newToolbarButton->SetEnabled(!edit);
	removeToolbarButton->SetEnabled(!edit);
	saveToolbarButton->SetEnabled(edit);
	newDatabaseToolbarButton->SetEnabled(edit);
	testConnectionToolbarButton->SetEnabled(edit);
	cancelToolbarButton->SetEnabled(edit);
	closeToolbarButton->SetEnabled(!edit);

	enterpriseListBox->Enable(!edit);

	companyTextCtrl->Enable(edit);
	hostnameComboBox->Enable(edit);
	databaseNameTextCtrl->Enable(edit);
	usernameComboBox->Enable(edit);
	passwordTextCtrl->Enable(edit);
	portComboBox->Enable(edit);

	if(!edit) {
		companyTextCtrl->Clear();
		hostnameComboBox->SetValue(wxEmptyString);
		databaseNameTextCtrl->Clear();
		usernameComboBox->SetValue(wxEmptyString);
		passwordTextCtrl->Clear();
		portComboBox->SetValue(wxEmptyString);
		if(editingEnterprise && editingEnterprise->Get(_N("id"))->ToUnsignedInt() == 0)
			delete editingEnterprise;
		editingEnterprise = NULL;
	}
}
