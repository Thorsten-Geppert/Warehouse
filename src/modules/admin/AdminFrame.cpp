#include "AdminFrame.h"
#include <wx/filedlg.h>
#include <wx/busyinfo.h>
#include "../lib/EnterpriseEntity.h"
#include "../lib/EnterpriseController.h"
#include "../lib/UserEntity.h"
#include "../lib/UserController.h"
#include "../lib/DatabaseType.h"
#include "../lib/DatabaseLibrary.h"
#include "../lib/UpdateLibrary.h"
#include "UserDialog.h"
#include "UpdateSpecialDialog.h"
#include "DatabaseRecoverDialog.h"
#include "NetworkAccessDialog.h"
#include "UpdateDialog.h"
#include "Configuration.h"
#include "InformationDialog.h"
#include "LoginLogo.xpm"
#include "AdminIcon.xpm"

#include <DSDebug.h>

using namespace DynSoft;

BEGIN_EVENT_TABLE(AdminFrame, wxFrame)
	EVT_BUTTON(REFRESH_EVENT, AdminFrame::RefreshEvent)
	EVT_BUTTON(EXECUTE_EVENT, AdminFrame::ExecuteEvent)
END_EVENT_TABLE()

AdminFrame::AdminFrame(
	DSPostgreSQLDatabase *db
) : wxFrame(
	NULL,
	-1,
	TITLE
) {
	this->db = db;
	this->showPasswords = false;

	wxArrayString actions;
	actions.Add(_("Update enterprise database"));
	actions.Add(_("Update Special"));
	actions.Add(_("Add/edit user to/of selected enterprise"));
	actions.Add(_("Backup selected enterprise database"));
	actions.Add(_("Create new database and recover enterprise"));
	actions.Add(_("Delete selected enterprise inclusive database and data"));
	actions.Add(_("Show passwords (on/off)"));
	actions.Add(_("Edit network access (pg_hba.conf)"));
	actions.Add(_("Reset database password"));
	actions.Add(_("Reorganize enterprise database"));
	actions.Add(_("Reorganize main database"));
	actions.Add(_("About"));

	mainPanel = new wxPanel(this, -1);
	mainBoxSizer = new wxBoxSizer(wxVERTICAL);
	
	logoStaticBitmap = new wxStaticBitmap(mainPanel, -1, wxBitmap(LoginLogo_xpm));
	mainBoxSizer->Add(logoStaticBitmap);

	versionStaticText = new wxStaticText(mainPanel, -1, SOFTWARE + _(" Version: ") + VERSION);
	mainBoxSizer->Add(versionStaticText, 0, wxALIGN_RIGHT | wxTOP | wxRIGHT | wxLEFT, 15);

	enterprisesListCtrl = new DSListCtrl(mainPanel, -1, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VRULES | wxBORDER_SUNKEN | wxLC_SINGLE_SEL);
	
	enterprisesListCtrl->InsertColumn(0, _("Company"), wxLIST_FORMAT_LEFT, 150);
	enterprisesListCtrl->InsertColumn(1, _("Hostname"), wxLIST_FORMAT_LEFT, 100);
	enterprisesListCtrl->InsertColumn(2, _("Database"), wxLIST_FORMAT_LEFT, 150);
	enterprisesListCtrl->InsertColumn(3, _("Username"), wxLIST_FORMAT_LEFT, 100);
	enterprisesListCtrl->InsertColumn(4, _("Password"), wxLIST_FORMAT_LEFT, 100);
	enterprisesListCtrl->InsertColumn(5, _("Port"), wxLIST_FORMAT_RIGHT, 60);
	enterprisesListCtrl->InsertColumn(6, _("Version"), wxLIST_FORMAT_CENTER, 100);

	mainBoxSizer->Add(enterprisesListCtrl, 1, wxEXPAND | wxTOP | wxRIGHT | wxLEFT, 15);
	mainBoxSizer->AddSpacer(5);

	optionsBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	refreshButton    = new wxButton(mainPanel, REFRESH_EVENT, _("Refresh"));
	actionStaticText = new wxStaticText(mainPanel, -1, _("Action:"));
	actionsComboBox  = new wxComboBox(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, actions, wxCB_READONLY);
	executeButton    = new wxButton(mainPanel, EXECUTE_EVENT, _("Execute"));

	actionsComboBox->SetSelection(0);

	optionsBoxSizer->Add(refreshButton, 0, wxALIGN_CENTER_VERTICAL);
	optionsBoxSizer->AddSpacer(5);
	optionsBoxSizer->Add(actionStaticText, 0, wxALIGN_CENTER_VERTICAL);
	optionsBoxSizer->AddSpacer(5);
	optionsBoxSizer->Add(actionsComboBox, 1, wxALIGN_CENTER_VERTICAL);
	optionsBoxSizer->AddSpacer(5);
	optionsBoxSizer->Add(executeButton, 0, wxALIGN_CENTER_VERTICAL);
	mainBoxSizer->Add(optionsBoxSizer, 0, wxEXPAND | wxBOTTOM | wxRIGHT | wxLEFT, 15);

	mainPanel->SetSizer(mainBoxSizer);
	mainBoxSizer->SetSizeHints(this);

	SetSize(wxSize(-1, 600));
	SetMinSize(GetSize());
	SetMaxSize(GetSize());

	LoadEnterprises();

	ICON();
}

AdminFrame::~AdminFrame() {
	enterprisesListCtrl->CleanUp();
}

void AdminFrame::RefreshEvent(wxCommandEvent &event) {
	RefreshList();
}

void AdminFrame::ExecuteEvent(wxCommandEvent &event) {
	Execute(actionsComboBox->GetSelection());
}

void AdminFrame::Execute(const int action) {
	switch(action) {
		case ACTION_UPDATE_ENTERPRISE_DATABASE:
			UpdateEnterpriseDatabase();
			break;
		case ACTION_UPDATE_SPECIAL:
			UpdateSpecial();
			break;
		case ACTION_USER_ADD_EDIT:
			AddUser();
			break;
		case ACTION_ENTERPRISE_BACKUP:
			Backup();
			break;
		case ACTION_ENTERPRISE_RECOVER:
			Recover();
			break;
		case ACTION_ENTERPRISE_DELETE:
			EnterpriseDelete();
			break;
		case ACTION_SHOW_PASSWORDS:
			showPasswords = !showPasswords;
			LoadEnterprises();
			break;
		case ACTION_NETWORK_ACCESSS:
			NetworkAccess();
			break;
		case ACTION_DATABASE_PASSWORD:
			DatabasePassword();
			break;
		case ACTION_DATABASE_VACUUM:
			DatabaseVacuum();
			break;
		case ACTION_MAIN_DATABASE_VACUUM:
			MainDatabaseVacuum();
			break;
		case ACTION_ABOUT:
			About();
			break;
	}
}

void AdminFrame::RefreshList() {
	LoadEnterprises();
}

void AdminFrame::UpdateEnterpriseDatabase() {
	EnterpriseEntity *enterpriseEntity = static_cast<EnterpriseEntity *>(enterprisesListCtrl->GetSingleSelectionPointer());
	if(!enterpriseEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the database you want to update."),
			_("Error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	UpdateLibrary updateLibrary;
	wxString version;
	if(!updateLibrary.GetVersion(*enterpriseEntity, &version)) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not determine version. Aborting."),
			_("Update")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	// Forget to reset the version number
	if(version == _N("1.3.0"))
		version = _N("1.0.0");

	wxArrayString versions(updateLibrary.GetVersions(version));

	if(versions.GetCount() == 0 || version == VERSION) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("No new versions found. No updates available."),
			_("Update")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	UpdateDialog *updateDialog = new UpdateDialog(
		this,
		-1,
		enterpriseEntity,
		&versions,
		version
	);
	updateDialog->Center();
	updateDialog->ShowModal();
	updateDialog->Destroy();

	LoadEnterprises();
}

void AdminFrame::AddUser() {
	EnterpriseEntity *enterpriseEntity = static_cast<EnterpriseEntity *>(enterprisesListCtrl->GetSingleSelectionPointer());
	if(!enterpriseEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to add a user."),
			_("Error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	UserDialog *userDialog = new UserDialog(this, -1);
	userDialog->Center();
	if(userDialog->ShowModal() == wxID_OK) {
		wxString username, password;
		userDialog->GetUsernameAndPassword(username, password);

		DSPostgreSQLDatabase tmpDb;
		tmpDb.SetHostname(enterpriseEntity->Get(_N("hostname"))->ToString());
		tmpDb.SetDatabase(enterpriseEntity->Get(_N("database"))->ToString());
		tmpDb.SetUsername(enterpriseEntity->Get(_N("username"))->ToString());
		tmpDb.SetPassword(enterpriseEntity->Get(_N("password"))->ToString());
		tmpDb.SetPort(enterpriseEntity->Get(_N("port"))->ToString());

		if(tmpDb.Connect()) {
			UserEntity userEntity(&tmpDb);

			userEntity.SetValue(_N("username"),    username);
			userEntity.Set(_N("password"),        _N("MD5('") + password + _N("')"), false, false);
			userEntity.SetValue(_N("realName"),    wxString(_("Emergency admin")));
			userEntity.SetValue(_N("email"),       wxString(_("info@dynsoft.com")));
			userEntity.SetValue(_N("description"), wxString(_("Emergency user")));
			userEntity.SetValue(_N("status"),      4);

			UserController userController(&tmpDb);
			bool ok = false;
			if(userController.Exists(_N("username"), username)) {
				ok = userController.Update(&userEntity, wxEmptyString, true);
			} else {
				const wxString pk(userEntity.GetPrimaryKey());
				userEntity.SetPrimaryKey(wxEmptyString);
				ok = userController.Insert(&userEntity);
				userEntity.SetPrimaryKey(pk);
			}

			wxString message(_("The user was created/changed. You can use the account to login."));
			if(!ok)
				message = _("The user could not been created/changed. An error has occured.");

			wxMessageDialog *okMessage = new wxMessageDialog(this, message, _("Add user"));
			okMessage->Center();
			okMessage->ShowModal();
			okMessage->Destroy();
			
			tmpDb.Disconnect();
		} else {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not connect to database. The changes were not saved."),
				_("Error")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		}
	}
	userDialog->Destroy();
}

unsigned int AdminFrame::LoadEnterprises() {
	enterprisesListCtrl->CleanUp();

	UpdateLibrary updateLibrary;
	wxString version;
	unsigned int count = 0;
	DSListCtrl::DSListCtrlRow *enterpriseListCtrlRow = NULL;
	EnterpriseEntity *enterpriseEntity = NULL;
	EnterpriseController enterpriseController(db);
	enterpriseController.Get();
	while(enterpriseController.Run()) {
		enterpriseEntity = new EnterpriseEntity(db);
		enterpriseController.Get(enterpriseEntity);

		enterpriseListCtrlRow = new DSListCtrl::DSListCtrlRow(enterpriseEntity);
		enterpriseListCtrlRow->Add(enterpriseEntity->Get(_N("company"))->ToString());
		enterpriseListCtrlRow->Add(enterpriseEntity->Get(_N("hostname"))->ToString());
		enterpriseListCtrlRow->Add(enterpriseEntity->Get(_N("database"))->ToString());
		enterpriseListCtrlRow->Add(enterpriseEntity->Get(_N("username"))->ToString());
		if(showPasswords)
			enterpriseListCtrlRow->Add(enterpriseEntity->Get(_N("password"))->ToString());
		else
			enterpriseListCtrlRow->Add(_N("*********"));
		enterpriseListCtrlRow->Add(enterpriseEntity->Get(_N("port"))->ToString());

		if(!updateLibrary.GetVersion(*enterpriseEntity, &version))
			version = _("n/a");

		enterpriseListCtrlRow->Add(version);

		enterprisesListCtrl->Set(enterpriseListCtrlRow, true);

		count++;
	}

	return count;
}

void AdminFrame::Recover() {
	DatabaseRecoverDialog *databaseRecoverDialog = new DatabaseRecoverDialog(this, -1, db);
	databaseRecoverDialog->Center();
	if(databaseRecoverDialog->ShowModal() == wxID_OK)
		LoadEnterprises();
	databaseRecoverDialog->Destroy();
}

void AdminFrame::Backup() {
	EnterpriseEntity *enterpriseEntity = static_cast<EnterpriseEntity *>(enterprisesListCtrl->GetSingleSelectionPointer());
	if(!enterpriseEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to backup."),
			_("Error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	wxFileDialog *saveFileDialog = new wxFileDialog(
		this,
		_("Save file to"),
		wxEmptyString,
		enterpriseEntity->Get(_N("database"))->ToString() + _N(".backup"),
		_N("*.backup"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);
	saveFileDialog->Center();
	const bool ok = saveFileDialog->ShowModal() == wxID_OK;
	const wxString filename(saveFileDialog->GetPath());
	saveFileDialog->Destroy();
	if(!ok)
		return;

	wxBeginBusyCursor();
	wxString message(_("Backup is done."));

	const wxString command(
		PG_DUMP + _N(" --format plain --no-owner --inserts --no-privileges --verbose") +
		_N(" --host \"") + enterpriseEntity->Get(_N("hostname"))->ToString() + _N("\"") +
		_N(" --port ") + enterpriseEntity->Get(_N("port"))->ToString() +
		_N(" --username \"") + enterpriseEntity->Get(_N("username"))->ToString() + _N("\"") +
		_N(" --file \"") + filename + _N("\"") +
		_N(" \"") + enterpriseEntity->Get(_N("database"))->ToString() + _N("\"")
	);

	wxSetEnv(_N("PGPASSWORD"), enterpriseEntity->Get(_N("password"))->ToString());
	if(wxExecute(command, wxEXEC_SYNC) != 0)
		message = _("Could not create backup. An error occured. Maybe you have a to old version of Warehouse Admin with a to old pg_dump version. Please contact us.");
	wxUnsetEnv(_N("PGPASSWORD"));
	wxEndBusyCursor();

	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		GetTitle()
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();
}

void AdminFrame::EnterpriseDelete() {
	EnterpriseEntity *enterpriseEntity = static_cast<EnterpriseEntity *>(enterprisesListCtrl->GetSingleSelectionPointer());
	if(!enterpriseEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to backup."),
			_("Error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	wxMessageDialog *deleteMessageDialog = new wxMessageDialog(
		this,
		_("Do you want to delete the database with all of its data? This move is unrecoverable!"),
		_("Drop database"),
		wxYES_NO
	);
	deleteMessageDialog->Center();
	bool ok = deleteMessageDialog->ShowModal() == wxID_YES;
	deleteMessageDialog->Destroy();
	if(!ok)
		return;

	wxString message;
	DatabaseType databaseType;

	databaseType.SetHostname(enterpriseEntity->Get(_N("hostname"))->ToString());
	databaseType.SetDatabase(enterpriseEntity->Get(_N("database"))->ToString());
	databaseType.SetUsername(enterpriseEntity->Get(_N("username"))->ToString());
	databaseType.SetPassword(enterpriseEntity->Get(_N("password"))->ToString());
	databaseType.SetPort(enterpriseEntity->Get(_N("port"))->ToString());


	DatabaseLibrary databaseLibrary;
	switch(databaseLibrary.DropDatabase(databaseType)) {
		case DATABASE_LIBRARY_OK:
			message = _("Database was removed.");
			ok      = true;
			break;
		case DATABASE_LIBRARY_ERROR_CONNECTION:
			message = _("Could not connect to database postgres or template1. Could not drop database.");
			break;
		case DATABASE_LIBRARY_ERROR_DELETION:
			message = _("Could not drop database. Unknown error.");
			break;
		case DATABASE_LIBRARY_ERROR_EXISTS:
			message = _("Database does no exist.");
			break;
		default:
			message = _("Unknown error. Could not create database.");
			break;
	}

	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		_("Drop database")
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();

	EnterpriseController enterpriseController(db);
	if(!enterpriseController.Delete(enterpriseEntity)) {
		wxMessageDialog *errorDialog = new wxMessageDialog(
			this,
			_("Could not remove the company entry from Warehouse. Please do it by yourself in the Warehouse enterprise configuration dialog."),
			_("Drop database")
		);
		errorDialog->Center();
		errorDialog->ShowModal();
		errorDialog->Destroy();
		ok = false;
	}

	LoadEnterprises();
}

void AdminFrame::About() {
	InformationDialog *informationDialog = new InformationDialog(this, -1);
	informationDialog->Center();
	informationDialog->ShowModal();
	informationDialog->Destroy();
}

void AdminFrame::NetworkAccess() {
	NetworkAccessDialog *networkAccessDialog = new NetworkAccessDialog(this, -1);
	networkAccessDialog->Center();
	networkAccessDialog->ShowModal();
	networkAccessDialog->Destroy();
}

void AdminFrame::DatabasePassword() {
	wxString message;

	const wxString command(
		PSQL + _N(" -d template1 -U postgres -c ") +
		_N("\"ALTER USER postgres WITH PASSWORD 'postgres'\"")
	);

	if(wxExecute(command, wxEXEC_SYNC) != 0) {
		message = _("Could not set password. An error occured. Maybe you have a to old version of Warehouse Admin with a to old psql version. Please contact us.");
	} else {
		message = _("The password of user \"postgres\" was set to \"postgres\"");
	}
	
	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		_("Reset Password")
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();
}

void AdminFrame::DatabaseVacuum() {
	EnterpriseEntity *enterpriseEntity = static_cast<EnterpriseEntity *>(enterprisesListCtrl->GetSingleSelectionPointer());
	if(!enterpriseEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to reorganize."),
			_("Error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	wxMessageDialog *reorgMessageDialog = new wxMessageDialog(
		this,
		_("Do you want to reorganize the database? If you select yes, please stay tuned, this may take a long time. The Warehouse Admin will not resonse in this time. Do not kill this application!"),
		_("Reorganize database"),
		wxYES_NO
	);
	reorgMessageDialog->Center();
	const bool ok = reorgMessageDialog->ShowModal() == wxID_YES;
	reorgMessageDialog->Destroy();
	if(!ok)
		return;

	wxBusyInfo *busyInfo = new wxBusyInfo(_("Please wait..."), this);

	wxYield();
	wxYield();
	wxYield();


	wxString message;
	DatabaseType databaseType;

	databaseType.SetHostname(enterpriseEntity->Get(_N("hostname"))->ToString());
	databaseType.SetDatabase(enterpriseEntity->Get(_N("database"))->ToString());
	databaseType.SetUsername(enterpriseEntity->Get(_N("username"))->ToString());
	databaseType.SetPassword(enterpriseEntity->Get(_N("password"))->ToString());
	databaseType.SetPort(enterpriseEntity->Get(_N("port"))->ToString());

	DatabaseLibrary databaseLibrary;
	switch(databaseLibrary.ReorganizeDatabase(db, databaseType)) {
		case DATABASE_LIBRARY_OK:
			message = _("Database was reorganized.");
			break;
		case DATABASE_LIBRARY_ERROR_CONNECTION:
			message = _("Could not connect to database postgres or template1. Could not reorganize database.");
			break;
		case DATABASE_LIBRARY_ERROR_VACUUM:
			message = _("Could not reorganize database. Unknown error.");
			break;
		case DATABASE_LIBRARY_ERROR_EXISTS:
			message = _("Database does no exist.");
			break;
		default:
			message = _("Unknown error. Could not create database.");
			break;
	}

	delete busyInfo;

	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		_("Reorganize database")
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();
}

void AdminFrame::MainDatabaseVacuum() {
	wxMessageDialog *deleteMessageDialog = new wxMessageDialog(
		this,
		_("Do you want to reorganize the main database? If you select yes, please stay tuned, this may take a long time. The Warehouse Admin will not resonse in this time. Do not kill this application!"),
		_("Reorganize database"),
		wxYES_NO
	);
	deleteMessageDialog->Center();
	bool ok = deleteMessageDialog->ShowModal() == wxID_YES;
	deleteMessageDialog->Destroy();
	if(ok) {
		wxBusyInfo *busyInfo = new wxBusyInfo(_("Please wait..."), this);

		wxYield();
		wxYield();
		wxYield();

		wxString message;
		DatabaseLibrary databaseLibrary;
		if(!databaseLibrary.ReorganizeDatabase(*db)) {
			message = _("Could not reorganize the database. Unknown error.");
		} else {
			message = _("The main database was reorganized.");
		}

		delete busyInfo;

		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			message,
			_("Reorganize database")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}
}

void AdminFrame::UpdateSpecial() {
	EnterpriseEntity *enterpriseEntity = static_cast<EnterpriseEntity *>(enterprisesListCtrl->GetSingleSelectionPointer());
	if(!enterpriseEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the database you want to update."),
			_("Error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	UpdateLibrary updateLibrary;
	wxString version;
	if(!updateLibrary.GetVersion(*enterpriseEntity, &version)) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not determine version. Aborting."),
			_("Update")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	UpdateSpecialDialog *updateSpecialDialog = new UpdateSpecialDialog(
		this,
		-1,
		enterpriseEntity,
		version
	);
	updateSpecialDialog->Center();
	updateSpecialDialog->ShowModal();
	updateSpecialDialog->Destroy();

	LoadEnterprises();
}
