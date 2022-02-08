#include "ConfigurationDialog.h"
#include "Configuration.h"
#include <DSPostgreSQLDatabase.h>
#include <DSFile.h>
#include "../lib/UserConfigurationFileController.h"
#include "../lib/DatabaseType.h"
#include "../lib/DatabaseLibrary.h"
#include "../lib/EnterpriseEntity.h"
#include "../lib/EnterpriseController.h"
#include "DatabaseConfigurationDialog.h"
#include "EnterpriseCreateDatabaseDialog.h"
#include "resources/images/WarehouseIcon.xpm"


BEGIN_EVENT_TABLE(ConfigurationDialog, wxDialog)
	EVT_MENU(RUN_EVENT, ConfigurationDialog::RunKeyEvent)
	EVT_MENU(CANCEL_EVENT, ConfigurationDialog::CancelKeyEvent)
	EVT_MENU(MANUAL_EVENT, ConfigurationDialog::ManualKeyEvent)
	EVT_CHECKBOX(XRCID("configurationCheckBox"), ConfigurationDialog::AutoConfigurationEvent) 
	EVT_TEXT(XRCID("enterpriseEnterpriseTextCtrl"), ConfigurationDialog::EnterpriseDatabaseEvent) 
END_EVENT_TABLE()

ConfigurationDialog::ConfigurationDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ConfigurationDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ConfigurationDialog"));

	mainPanel                    = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                 = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar                  = XRCCTRL(*this, "mainToolbar", Toolbar);
	runToolbarButton             = XRCCTRL(*this, "runToolbarButton", ToolbarButton);
	manualToolbarButton          = XRCCTRL(*this, "manualToolbarButton", ToolbarButton);
	cancelToolbarButton          = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);

	enterpriseEnterpriseTextCtrl = XRCCTRL(*this, "enterpriseEnterpriseTextCtrl", wxTextCtrl);
	enterpriseDatabaseStaticText = XRCCTRL(*this, "enterpriseDatabaseStaticText", wxStaticText);

	configurationCheckBox        = XRCCTRL(*this, "configurationCheckBox", wxCheckBox);
	databasePanel                = XRCCTRL(*this, "databasePanel", wxPanel);
	databaseFlexGridSizer        = static_cast<wxFlexGridSizer *>(databasePanel->GetSizer());

	hostnameStaticText           = XRCCTRL(*this, "hostnameStaticText", wxStaticText);
	hostnameComboBox             = XRCCTRL(*this, "hostnameComboBox", wxComboBox);
	databaseStaticText           = XRCCTRL(*this, "databaseStaticText", wxStaticText);
	databaseComboBox             = XRCCTRL(*this, "databaseComboBox", wxComboBox);
	usernameStaticText           = XRCCTRL(*this, "usernameStaticText", wxStaticText);
	usernameComboBox             = XRCCTRL(*this, "usernameComboBox", wxComboBox);
	passwordStaticText           = XRCCTRL(*this, "passwordStaticText", wxStaticText);
	passwordTextCtrl             = XRCCTRL(*this, "passwordTextCtrl", DSTextCtrl);
	portStaticText               = XRCCTRL(*this, "portStaticText", wxStaticText);
	portComboBox                 = XRCCTRL(*this, "portComboBox", wxComboBox);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(runToolbarButton, ConfigurationDialog::RunEvent);
	CONNECT_TOOLBARBUTTON(manualToolbarButton, ConfigurationDialog::ManualEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ConfigurationDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'm',  MANUAL_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'r',  RUN_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetMinSize(wxSize(400, 300));
	enterpriseDatabaseStaticText->SetLabel(_("<automatic>"));
	enterpriseEnterpriseTextCtrl->SetFocus();
	databasePanel->Hide();

	ICON();
}

void ConfigurationDialog::RunEvent(wxMouseEvent &event) {
	Run();
	SKIP();
}

void ConfigurationDialog::RunKeyEvent(wxCommandEvent &event) {
	Run();
}

void ConfigurationDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ConfigurationDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ConfigurationDialog::ManualEvent(wxMouseEvent &event) {
	Manual();
	SKIP();
}

void ConfigurationDialog::ManualKeyEvent(wxCommandEvent &event) {
	Manual();
}

void ConfigurationDialog::AutoConfigurationEvent(wxCommandEvent &event) {
	bool autoCheck = event.IsChecked();

	databasePanel->Show(!autoCheck);
	hostnameStaticText->Enable(!autoCheck);
	hostnameComboBox->Enable(!autoCheck);
	databaseStaticText->Enable(!autoCheck);
	databaseComboBox->Enable(!autoCheck);
	usernameStaticText->Enable(!autoCheck);
	usernameComboBox->Enable(!autoCheck);
	passwordStaticText->Enable(!autoCheck);
	passwordTextCtrl->Enable(!autoCheck);
	portStaticText->Enable(!autoCheck);
	portComboBox->Enable(!autoCheck);
}

void ConfigurationDialog::EnterpriseDatabaseEvent(wxCommandEvent &event) {
	NormalizeEnterpriseDatabase();
}

void ConfigurationDialog::NormalizeEnterpriseDatabase() {
	enterpriseDatabaseStaticText->SetLabel(DatabaseLibrary::BuildDatabaseName(enterpriseEnterpriseTextCtrl->GetValue()));
}

void ConfigurationDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void ConfigurationDialog::Manual() {
	DatabaseConfigurationDialog *manualConfigurationDialog = new DatabaseConfigurationDialog(this, -1, rit);
	manualConfigurationDialog->Center();
	if(manualConfigurationDialog->ShowModal() == wxID_OK) {
		EndModal(wxID_OK);
	}
	manualConfigurationDialog->Destroy();
	
}

void ConfigurationDialog::Run() {
	if(
		enterpriseDatabaseStaticText->GetLabel() == databaseComboBox->GetValue()
	) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please use different names for the enterprise database and the warehouse database."),
			_("Configuration")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return;
	}

	if(
		(enterpriseEnterpriseTextCtrl->GetValue().IsEmpty() ||
		!configurationCheckBox->IsChecked()) && (
			hostnameComboBox->GetValue().IsEmpty() || 
			databaseComboBox->GetValue().IsEmpty() ||
			usernameComboBox->GetValue().IsEmpty() ||
			portComboBox->GetValue().IsEmpty()
		) 
	) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill in the all fields."),
			_("Configuration")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return;
	}

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

	wxBeginBusyCursor();

	company = enterpriseEnterpriseTextCtrl->GetValue();
	enterpriseDatabase = enterpriseDatabaseStaticText->GetLabel();
	if(!configurationCheckBox->IsChecked()) {
		hostname = hostnameComboBox->GetValue();
		database = databaseComboBox->GetValue();
		username = usernameComboBox->GetValue();
		password = passwordTextCtrl->GetValue();
		port = portComboBox->GetValue();
	} else {
		hostname = DEFAULT_HOSTNAME;
		database = DEFAULT_DATABASE;
		username = DEFAULT_USERNAME;
		password = DEFAULT_PASSWORD;
		port = DEFAULT_PORT;
	}

	bool checkMain = false;
	bool checkInsert = false;
	bool checkEnterprise = false;
	if(!TestConnection(
		hostname,
		database,
		username,
		password,
		port
	)) {
		checkMain = CreateMainDatabase();
	} else {
		checkMain = true;
		SaveConfigurationFile();
	}

	if(checkMain)
		checkInsert = InsertEnterprise();

	if(checkMain && checkInsert)
		checkEnterprise = CreateEnterprise();

	if(checkMain && checkInsert && checkEnterprise)
		EndModal(wxID_OK);
	
	wxEndBusyCursor();
}

bool ConfigurationDialog::TestConnection(
	const wxString hostname,
	const wxString database,
	const wxString username,
	const wxString password,
	const wxString port
) {
	DSPostgreSQLDatabase db;

	db.SetHostname(hostname);
	db.SetDatabase(database);
	db.SetUsername(username);
	db.SetPassword(password);
	db.SetPort(port);
	if(db.Connect()) {
		db.Disconnect();
		return true;
	}

	return false;
}

bool ConfigurationDialog::SaveConfigurationFile() {
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetHostname(hostname);
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetDatabase(database);
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetUsername(username);
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetPassword(password);
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetPort(port);

	bool connected = false;
	if(!rit->GetEnterprisesDatabase()->IsConnected()) {
		rit->GetEnterprisesDatabase()->SetHostname(hostname);
		rit->GetEnterprisesDatabase()->SetDatabase(database);
		rit->GetEnterprisesDatabase()->SetUsername(username);
		rit->GetEnterprisesDatabase()->SetPassword(password);
		rit->GetEnterprisesDatabase()->SetPort(port);

		connected = rit->GetEnterprisesDatabase()->Connect();
	} else {
		connected = true;
	}

	if(connected) {
		if(UserConfigurationFileController::Save(
			rit->GetUserConfigurationFileEntity(),
			rit->GetUserConfigurationFileWithPath(),
			UserConfigurationFileController::DB
		)) {
			return true;
		}
	}

	return false;
}

bool ConfigurationDialog::CreateMainDatabase() {
	if(TestConnection(hostname, database, username, password, port))
		return true;

	const wxString filename(rit->GRP(_N("database"), _N("warehouse.sql")));

	wxString message;
	wxString sql;

	DSFile file(filename, _N("r"));
	if(file.IsOpened()) {
		if(!file.ReadAll(&sql))
			message = _("Could not create database, because could not read file: ") + filename;

		if(!file.Close()) {
			if(message.IsEmpty())
				message = _("Could not create database, because could not close file: ") + filename;
		}
	} else {
		message = _("Could not create database, because could not open file: ") + filename;
	}

	if(!message.IsEmpty()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			message,
			_("Configuration")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return false;
	}

	DatabaseType databaseType;

	databaseType.SetHostname(hostname);
	databaseType.SetDatabase(database);
	databaseType.SetUsername(username);
	databaseType.SetPassword(password);
	databaseType.SetPort(port);

	DatabaseLibrary databaseLibrary(rit);
	
	unsigned int databaseError = databaseLibrary.CreateDatabase(databaseType, sql);
	switch(databaseError) {
		case DATABASE_LIBRARY_OK:
			message = _("Database was created.");
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

	if(databaseError == DATABASE_LIBRARY_OK) {
		rit->GetEnterprisesDatabase()->SetHostname(hostname);
		rit->GetEnterprisesDatabase()->SetDatabase(database);
		rit->GetEnterprisesDatabase()->SetUsername(username);
		rit->GetEnterprisesDatabase()->SetPassword(password);
		rit->GetEnterprisesDatabase()->SetPort(port);

		if(!rit->GetEnterprisesDatabase()->IsConnected()) {
			if(!rit->GetEnterprisesDatabase()->Connect()) {
				wxMessageDialog *messageDialog = new wxMessageDialog(
					this,
					_("Could not connect to database."),
					_("Configuration")
				);
				messageDialog->Center();
				messageDialog->ShowModal();
				messageDialog->Destroy();

				return false;
			}
		}
	} else {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			message,
			_("Configuration")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();

		return false;
	}

	SaveConfigurationFile();

	return true;
}

bool ConfigurationDialog::InsertEnterprise() {
	EnterpriseEntity enterpriseEntity(rit->GetEnterprisesDatabase());

	enterpriseEntity.SetValue(_N("company"), company);
	enterpriseEntity.SetValue(_N("hostname"), hostname);
	enterpriseEntity.SetValue(_N("database"), enterpriseDatabase);
	enterpriseEntity.SetValue(_N("username"), username);
	enterpriseEntity.SetValue(_N("password"), password);
	enterpriseEntity.SetValue(_N("port"), DSCasts::ToUnsignedInt(port));

	EnterpriseController enterpriseController(rit->GetEnterprisesDatabase());

	bool ok = false;
	if(!enterpriseController.Exists(_N("database"), enterpriseDatabase)) {
		ok = enterpriseController.Insert(&enterpriseEntity);
	} else {
		ok = true;
	}

	if(!ok) {
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

bool ConfigurationDialog::CreateEnterprise() {
	if(TestConnection(
		hostname,
		enterpriseDatabase,
		username,
		password,
		port
	)) {
		return true;
	}

	wxString filename(rit->GRP(_N("database"), _N("enterprise.sql")));

	bool returnValue = true;
	wxString message;
	wxString sql;

	DSFile file(filename, _N("r"));
	if(file.IsOpened()) {
	//#ifdef __WXMSW__
		if(!file.ReadAll(&sql)) {
	//#else
	//	if(!file.ReadAll(&sql, wxConvISO8859_1)) {
	//#endif
			returnValue = false;
		}

		if(!file.Close())
			returnValue = false;
	} else {
		return false;
	}
	
	if(!returnValue)
		return false;

	DatabaseType databaseType;

	databaseType.SetHostname(hostname);
	databaseType.SetDatabase(enterpriseDatabase);
	databaseType.SetUsername(username);
	databaseType.SetPassword(password);
	databaseType.SetPort(port);

	EnterpriseCreateDatabaseDialog *enterpriseCreateDatabaseDialog = new EnterpriseCreateDatabaseDialog(
		this,
		-1,
		rit,
		company,
		&databaseType,
		sql,
		false
	);
	enterpriseCreateDatabaseDialog->Center();
	if(enterpriseCreateDatabaseDialog->ShowModal() != wxID_OK)
		returnValue = false;
	enterpriseCreateDatabaseDialog->Destroy();

	return returnValue;
}

