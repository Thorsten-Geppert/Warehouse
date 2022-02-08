#include "DatabaseConfigurationDialog.h"
#include <DSPostgreSQLDatabase.h>
#include <DSFile.h>
#include "../lib/UserConfigurationFileController.h"
#include "../lib/DatabaseType.h"
#include "../lib/DatabaseLibrary.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(DatabaseConfigurationDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, DatabaseConfigurationDialog::CancelKeyEvent)
	EVT_MENU(TEST_CONNECTION_EVENT, DatabaseConfigurationDialog::TestConnectionKeyEvent)
	EVT_MENU(NEW_CONNECTION_EVENT, DatabaseConfigurationDialog::NewConnectionKeyEvent)
	EVT_MENU(SAVE_EVENT, DatabaseConfigurationDialog::SaveKeyEvent)
END_EVENT_TABLE()

DatabaseConfigurationDialog::DatabaseConfigurationDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("DatabaseConfigurationDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("DatabaseConfigurationDialog"));

	mainPanel                   = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar                 = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton         = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	testConnectionToolbarButton = XRCCTRL(*this, "testConnectionToolbarButton", ToolbarButton);
	newConnectionToolbarButton  = XRCCTRL(*this, "newConnectionToolbarButton", ToolbarButton);
	saveToolbarButton           = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	hostnameComboBox            = XRCCTRL(*this, "hostnameComboBox", wxComboBox);
	databaseComboBox            = XRCCTRL(*this, "databaseComboBox", wxComboBox);
	usernameComboBox            = XRCCTRL(*this, "usernameComboBox", wxComboBox);
	passwordTextCtrl            = XRCCTRL(*this, "passwordTextCtrl", wxTextCtrl);
	portComboBox                = XRCCTRL(*this, "portComboBox", wxComboBox);

	hostnameComboBox->SetValue(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetHostname());
	databaseComboBox->SetValue(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetDatabase());
	usernameComboBox->SetValue(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetUsername());
	passwordTextCtrl->SetValue(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetPassword());
	portComboBox->SetValue(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetPort());

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, DatabaseConfigurationDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, DatabaseConfigurationDialog::SaveEvent);
	CONNECT_TOOLBARBUTTON(testConnectionToolbarButton, DatabaseConfigurationDialog::TestConnectionEvent);
	CONNECT_TOOLBARBUTTON(newConnectionToolbarButton, DatabaseConfigurationDialog::NewConnectionEvent);

	wxAcceleratorEntry acceleratorEntries[5];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'n',  NEW_CONNECTION_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,    (int) 't',  TEST_CONNECTION_EVENT);
	wxAcceleratorTable acceleratorTable(5, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();
}

void DatabaseConfigurationDialog::TestConnectionEvent(wxMouseEvent &event) {
	TestConnection();
	SKIP();
}

void DatabaseConfigurationDialog::TestConnectionKeyEvent(wxCommandEvent &event) {
	TestConnection();
}

void DatabaseConfigurationDialog::NewConnectionEvent(wxMouseEvent &event) {
	NewConnection();
	SKIP();
}

void DatabaseConfigurationDialog::NewConnectionKeyEvent(wxCommandEvent &event) {
	NewConnection();
}

void DatabaseConfigurationDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void DatabaseConfigurationDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void DatabaseConfigurationDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void DatabaseConfigurationDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void DatabaseConfigurationDialog::TestConnection() {
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

	DSPostgreSQLDatabase db;
	db.SetHostname(hostnameComboBox->GetValue());
	db.SetDatabase(databaseComboBox->GetValue());
	db.SetUsername(usernameComboBox->GetValue());
	db.SetPassword(passwordTextCtrl->GetValue());
	db.SetPort(portComboBox->GetValue());
	if(db.Connect()) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Connection could be established."),
			_("Connection OK")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		db.Disconnect();
	} else {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Connection could not be established."),
			_("Connection not OK")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}
}

void DatabaseConfigurationDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void DatabaseConfigurationDialog::Save() {
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetHostname(hostnameComboBox->GetValue());
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetDatabase(databaseComboBox->GetValue());
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetUsername(usernameComboBox->GetValue());
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetPassword(passwordTextCtrl->GetValue());
	rit->GetUserConfigurationFileEntity()->GetDatabase()->SetPort(portComboBox->GetValue());

	if(DSCasts::ToUnsignedInt(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetPort()) == 0) {
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

	if(!UserConfigurationFileController::Save(
		rit->GetUserConfigurationFileEntity(),
		rit->GetUserConfigurationFileWithPath(),
		UserConfigurationFileController::DB
	)) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("The configuration could not be saved. Please check the permissions."),
			_("Save configuration file")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	} else {
		EndModal(wxID_OK);
	}
}

void DatabaseConfigurationDialog::NewConnection() {
	wxString filename(rit->GRP(_N("database"), _N("warehouse.sql")));

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
			_("Create database")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	DatabaseType databaseType;

	databaseType.SetHostname(hostnameComboBox->GetValue());
	databaseType.SetDatabase(databaseComboBox->GetValue());
	databaseType.SetUsername(usernameComboBox->GetValue());
	databaseType.SetPassword(passwordTextCtrl->GetValue());
	databaseType.SetPort(portComboBox->GetValue());

	DatabaseLibrary databaseLibrary(rit);
	switch(databaseLibrary.CreateDatabase(databaseType, sql)) {
		case DATABASE_LIBRARY_OK:
			message = _("Database was created. Please press save or test connection.");
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

	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		_("Create database")
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();
}
