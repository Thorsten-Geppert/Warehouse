#include "Warehouse.h"
#include "LoginDialog.h"
#include "WarehouseFrame.h"
#include <wx/xrc/xmlres.h>
#include <wx/notebook.h>
#include <wx/protocol/http.h>
#include <DSLog.h>
#include <DSPostgreSQLQuery.h>
#include "Configuration.h"
#include "DatabaseConfigurationDialog.h"
#include "BusyInfo.h"
#include "ConfigurationDialog.h"
#include "../lib/Toolbar.h"
#include "../lib/UserConfigurationFileController.h"

IMPLEMENT_APP(Warehouse)

Warehouse::~Warehouse() {
	delete rit;
}

bool Warehouse::OnInit() {
	wxDisableAsserts();

	wxSocketBase::Initialize();
	wxInitAllImageHandlers();
	wxXmlResource::Get()->InitAllHandlers();
	wxXmlResource::Get()->AddHandler(new ToolbarXmlResourceHandler);

	rit = new RuntimeInformationType(
		argv[0],
		RESOURCE_PATH,
		USER_CONFIGURATION_FILE
	);

	wxLocale *locale = new wxLocale(wxLANGUAGE_GERMAN);//, wxLOCALE_CONV_ENCODING);
	wxLocale::AddCatalogLookupPathPrefix(rit->GRP(_N("languages"), wxEmptyString));
	locale->AddCatalog(_N("Warehouse"));
	rit->SetLocale(locale);

	if(!UserConfigurationFileController::Load(
		rit->GetUserConfigurationFileEntity(),
		rit->GetUserConfigurationFileWithPath()
	)) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			NULL,
			_("Could not load or initiate configuration file. Please correct the permissions."),
			_("Load configuration file")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return false;
	}
	if(ConnectToDatabase(rit) != OK)
		return false;

	LoginDialog *loginDialog = new LoginDialog(NULL, -1, rit);
	loginDialog->Center();
	SetTopWindow(loginDialog);
	const bool loggedIn = loginDialog->ShowModal() == wxID_OK;
	rit->SetDemoVersion(loginDialog->IsDemoVersion());
	loginDialog->Destroy();
	if(loggedIn) {
		BusyInfo *busyInfo = new BusyInfo(NULL);

		rit->SetLog(new DSLog(new DSPostgreSQLQuery(rit->GetDatabase())), true);

		rit->Log(false, _N("Warehouse"), _("The user has logged in"));

		if(!UserConfigurationFileController::Save(
			rit->GetUserConfigurationFileEntity(),
			rit->GetUserConfigurationFileWithPath(),
			UserConfigurationFileController::ALL
		)) {
			rit->Log(true, _N("Warehouse"), _("Could not save configuration file. Please correct the permissions. You will be logged in now."));
			wxMessageDialog *messageDialog = new wxMessageDialog(
				NULL,
				_("Could not save configuration file. Please correct the permissions. You will be logged in now."),
				_("Save configuration file")
			);
			messageDialog->Center();
			messageDialog->ShowModal();
			messageDialog->Destroy();
		} else {
			rit->Log(false, _N("Warehouse"), _("Configuration file saved after Login."));
		}

		WarehouseFrame *warehouseFrame = new WarehouseFrame(
			NULL,
			-1,
			rit,
			rit->GetUserConfigurationFileEntity()->GetMainWindowPreferences()->GetPosition(),
			rit->GetUserConfigurationFileEntity()->GetMainWindowPreferences()->GetSize()
		);
		warehouseFrame->Show();
		SetTopWindow(warehouseFrame);

		delete busyInfo;

		return true;
	}

	return false;
}

unsigned int Warehouse::ConnectToDatabase(RuntimeInformationType *rit) {
	bool connected = false;
	bool canceled  = false;

	do {
		rit->GetEnterprisesDatabase()->SetHostname(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetHostname());
		rit->GetEnterprisesDatabase()->SetDatabase(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetDatabase());
		rit->GetEnterprisesDatabase()->SetUsername(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetUsername());
		rit->GetEnterprisesDatabase()->SetPassword(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetPassword());
		rit->GetEnterprisesDatabase()->SetPort(rit->GetUserConfigurationFileEntity()->GetDatabase()->GetPort());

		connected = rit->GetEnterprisesDatabase()->Connect();
		if(!connected) {
			wxMessageDialog *messageDialog = new wxMessageDialog(
				NULL,
				_("Could not connect to the database. Please check data in the following dialog."),
				_("Database error")
			);
			messageDialog->Center();
			messageDialog->ShowModal();
			messageDialog->Destroy();
			
			
			ConfigurationDialog *c = new ConfigurationDialog(NULL, -1, rit);
			c->Center();
			if(c->ShowModal() == wxID_CANCEL) {
				canceled = true;
				connected = true;
			}
			c->Destroy();
		}
	} while(!connected);

	if(canceled)
		return CANCEL;
	return OK;
}
