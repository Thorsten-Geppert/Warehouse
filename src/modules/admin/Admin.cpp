#include "Admin.h"
#include "AdminFrame.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include "../lib/UserConfigurationFileEntity.h"
#include "../lib/UserConfigurationFileController.h"

#include "Configuration.h"

IMPLEMENT_APP(Admin)

Admin::Admin() {
	db = new DSPostgreSQLDatabase;
}

Admin::~Admin() {
	if(db->IsConnected())
		db->Disconnect();
	delete db;
	db = NULL;
}

bool Admin::OnInit() {
	wxDisableAsserts();

	wxInitAllImageHandlers();

	wxLocale *locale = new wxLocale(wxLANGUAGE_GERMAN);//, wxLOCALE_CONV_ENCODING);
	wxLocale::AddCatalogLookupPathPrefix(RESOURCE_PATH);
	locale->AddCatalog(_N("Admin"));
	
	UserConfigurationFileEntity userConfigurationFileEntity;
	if(!UserConfigurationFileController::Load(
		&userConfigurationFileEntity,
		wxStandardPaths::Get().GetUserConfigDir() + wxFileName::GetPathSeparator() + USER_CONFIGURATION_FILE
	)) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			NULL,
			_(
				"Could not open configuration file. Please use Warehouse "
				"to create the configuration file or copy the .DynSoftWarehouse.ini "
				"to this computer or use this software on a computer with installed "
				"Warehouse."
			),
			_("Configuration file error")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}

	db->SetHostname(userConfigurationFileEntity.GetDatabase()->GetHostname());
	db->SetDatabase(userConfigurationFileEntity.GetDatabase()->GetDatabase());
	db->SetUsername(userConfigurationFileEntity.GetDatabase()->GetUsername());
	db->SetPassword(userConfigurationFileEntity.GetDatabase()->GetPassword());
	db->SetPort(userConfigurationFileEntity.GetDatabase()->GetPort());

	if(!db->Connect()) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			NULL,
			_("Could not connect do database. Aborting startup."),
			_("Configuration file error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}

	wxMessageDialog *messageDialog = new wxMessageDialog(
		NULL,
		_("Please notice: you use this software on your own risk! Do you agree?"),
		_("Information"),
		wxYES_NO
	);
	messageDialog->Center();
	const bool ok = messageDialog->ShowModal() == wxID_YES;
	messageDialog->Destroy();

	if(!ok)
		return false;


	AdminFrame *adminFrame = new AdminFrame(db);
	adminFrame->Center();
	adminFrame->Show();

	SetTopWindow(adminFrame);
	
	return true;
}
