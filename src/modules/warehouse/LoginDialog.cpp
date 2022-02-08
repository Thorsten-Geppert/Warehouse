#include "LoginDialog.h"
#include <wx/xrc/xmlres.h>
#include <wx/mimetype.h>
#include "../lib/EnterpriseEntity.h"
#include "../lib/EnterpriseController.h"
#include "../lib/UserEntity.h"
#include "../lib/UserController.h"
#include "../lib/UserConfigurationFileController.h"
#include "../lib/UpdateLibrary.h"
#include "../../../../DSLicenceGenerator/src/modules/lib/LicenceLibrary.h"
#include "EnterpriseConfigurationDialog.h"
#include "TextInputDialog.h"
#include "Configuration.h"
#include "InformationDialog.h"
#include "DemoVersionDialog.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(LoginDialog, wxDialog)
	EVT_BUTTON(XRCID("licenceButton"), LoginDialog::LicenceEvent)
	EVT_BUTTON(XRCID("cancelButton"), LoginDialog::CancelEvent)
	EVT_BUTTON(XRCID("loginButton"), LoginDialog::LoginEvent)
	EVT_BUTTON(XRCID("enterpriseConfigurationBitmapButton"), LoginDialog::ConfigureEnterprisesEvent)
	EVT_BUTTON(XRCID("informationButton"), LoginDialog::LoginInformationEvent)
	EVT_BUTTON(XRCID("helpButton"), LoginDialog::LoginHelpEvent)
END_EVENT_TABLE()

LoginDialog::LoginDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->demoVersion            = true;
	this->rit                    = rit;
	this->lastSelectedEnterprise = rit->GetUserConfigurationFileEntity()->GetLastEnterprise();

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("LoginDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("LoginDialog"));

	SetTitle(TITLE);

	loginPanel              = XRCCTRL(*this, "loginPanel", wxPanel);
	loginBoxSizer           = static_cast<wxBoxSizer *>(loginPanel->GetSizer());
	loginLogoStaticBitmap   = XRCCTRL(*this, "loginLogoStaticBitmap", wxStaticBitmap);
	enterpriseComboBox      = XRCCTRL(*this, "enterpriseComboBox", wxComboBox);
	usernameTextCtrl        = XRCCTRL(*this, "usernameTextCtrl", wxTextCtrl);
	passwordTextCtrl        = XRCCTRL(*this, "passwordTextCtrl", wxTextCtrl);
	informationButton       = XRCCTRL(*this, "informationButton", wxButton);
	helpButton              = XRCCTRL(*this, "helpButton", wxButton);
	loginBoxSizer->SetSizeHints(this);

	loginLogoStaticBitmap->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(LoginDialog::DynSoftEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_CTRL,   (int) 'w', XRCID("cancelButton"));
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'q', XRCID("cancelButton"));
	acceleratorEntries[2].Set(wxACCEL_NORMAL, WXK_ESCAPE, XRCID("cancelButton"));
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	LoadEnterprises();

	usernameTextCtrl->SetValue(rit->GetUserConfigurationFileEntity()->GetLastUsername());
	if(usernameTextCtrl->GetValue().IsEmpty())
		usernameTextCtrl->SetFocus();
	else
		passwordTextCtrl->SetFocus();

	ICON();
}

LoginDialog::~LoginDialog() {
	UnloadEnterprises();
}

void LoginDialog::LicenceEvent(wxCommandEvent &event) {
	Licence();
}

void LoginDialog::CancelEvent(wxCommandEvent &event) {
	Cancel();
}

void LoginDialog::LoginEvent(wxCommandEvent &event) {
	Login();
}

void LoginDialog::ConfigureEnterprisesEvent(wxCommandEvent &event) {
	ConfigureEnterprises();
}

void LoginDialog::LoginInformationEvent(wxCommandEvent &event) {
	LoginInformation();
}

void LoginDialog::Licence() {
	wxString licenceNumber(DSCasts::ToString(rit->GetUserConfigurationFileEntity()->GetLicenceNumber()));
	if(licenceNumber == _N("0"))
		licenceNumber.Clear();

	TextInputDialog *licenceInputDialog = new TextInputDialog(
		this,
		-1,
		rit,
		_("Licence number"),
		_("Please insert your licence number:"),
		true,
		licenceNumber
	);
	licenceInputDialog->Center();

	if(licenceInputDialog->ShowModal() == wxID_OK) {
		rit->GetUserConfigurationFileEntity()->SetLicenceNumber(DSCasts::ToUnsignedInt(licenceInputDialog->GetValue()));
		if(!UserConfigurationFileController::Save(
			rit->GetUserConfigurationFileEntity(),
			rit->GetUserConfigurationFileWithPath(),
			UserConfigurationFileController::ALL
		)) {
			rit->Log(true, _N("Warehouse"), _("Could not save the licence number to the configuration file. Please contact your administrator."));
			wxMessageDialog *messageDialog = new wxMessageDialog(
				this,
				_("Could not save the licence number to the configuration file. Please contact your administrator."),
				_("Save licence number")
			);
			messageDialog->Center();
			messageDialog->ShowModal();
			messageDialog->Destroy();
		} else {
			rit->Log(false, _N("Warehouse"), _("Licence number saved."));
		}
	}

	licenceInputDialog->Destroy();
}

void LoginDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void LoginDialog::DynSoftEvent(wxMouseEvent &event) {
	DynSoft();
}

void LoginDialog::LoginHelpEvent(wxCommandEvent &event) {
	LoginHelp();
}

void LoginDialog::Login() {
	// Licence stuff - not useful in Demo-Versions
	const unsigned int licenceNumber = LicenceLibrary::CreateLicenceNumber(
		LicenceLibrary::GenerateHardwareKey(NAME)
	);

	if(licenceNumber != rit->GetUserConfigurationFileEntity()->GetLicenceNumber()) {
		rit->Log(true, _N("Warehouse"), _("You are running a demo version of Warehouse. Please fill in your correct licence number."));

		DemoVersionDialog *demoVersionDialog = new DemoVersionDialog(
			this,
			-1,
			rit,
			0
		);
		demoVersionDialog->Center();
		demoVersionDialog->ShowModal();
		demoVersionDialog->Destroy();

		demoVersion = true;
	} else {
		rit->Log(false, _N("Warehouse"), _("Licence number checked successfully."));

		demoVersion = false;
	}

	EnterpriseEntity *enterprise = NULL;
	if(enterpriseComboBox->GetCount() > 0) {
		enterprise = static_cast<EnterpriseEntity *>(
			enterpriseComboBox->GetClientData(enterpriseComboBox->GetSelection())
		);
	}
	
	if(!enterprise) {
		rit->Log(true, _N("Login"), _("The user had not selected an enterprise."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please select the enterprise you want to login."),
			_("Login")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	}

	UpdateLibrary updateLibrary;
	wxString version;
	if(!updateLibrary.GetVersion(*enterprise, &version)) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not determine version. Aborting."),
			_("Login")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

#ifdef VERSION_CHECK
	if(version != VERSION) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("This is version: ") + VERSION + _(". The database version is ") + version + _(". Could not start Warehouse. Please update your enterprise database with Warehouse Admin."),
			_("Login")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}
#endif

	DSPostgreSQLDatabase *db = rit->GetDatabase();
	db->SetHostname(enterprise->Get(_N("hostname"))->ToString());
	db->SetDatabase(enterprise->Get(_N("database"))->ToString());
	db->SetUsername(enterprise->Get(_N("username"))->ToString());
	db->SetPassword(enterprise->Get(_N("password"))->ToString());
	db->SetPort(enterprise->Get(_N("port"))->ToString());
	
	if(!db->Connect()) {
		rit->Log(true, _N("Login"), _("Could not connect to database: ") + db->GetHostname() + _(". Cannot login."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not connect to the enterprise: Database error. Please check the connection data."),
			_("Login")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	} else {
		rit->Log(false, _N("Login"), _("Connected to database: ") + db->GetHostname() + _("."));
		rit->SetEnterpriseName(enterprise->Get(_N("company"))->ToString());
	}

	const wxString username(usernameTextCtrl->GetValue());
	const wxString password(passwordTextCtrl->GetValue());

	UserController userController(rit);
	if(!userController.Login(username, password)) {
		rit->Log(true, _N("Login"), _("The user ") + username + _(" could not been logged in. Wrong username and/or password."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("You could not be logged in. Wrong username and/or password."),
			_("Login")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		db->Disconnect();

		passwordTextCtrl->SelectAll();

		return;
	}

	rit->Log(false, _N("Login"), _("The user ") + username + _(" is logged in."));

	rit->GetUserConfigurationFileEntity()->SetLastUsername(username);
	rit->GetUserConfigurationFileEntity()->SetLastEnterprise(enterprise->Get(_N("company"))->ToString());

	EndModal(wxID_OK);
}

void LoginDialog::ConfigureEnterprises() {
	EnterpriseConfigurationDialog *enterpriseConfigurationDialog = new EnterpriseConfigurationDialog(
		this,
		-1,
		rit
	);
	enterpriseConfigurationDialog->Center();
	enterpriseConfigurationDialog->ShowModal();
	enterpriseConfigurationDialog->Destroy();
	ReloadEnterprises();
}

unsigned int LoginDialog::LoadEnterprises() {
	unsigned int count = 0;
	EnterpriseEntity *enterprise = NULL;
	EnterpriseController enterpriseController(rit->GetEnterprisesDatabase());
	enterpriseController.Get();
	while(enterpriseController.Run()) {
		enterprise = new EnterpriseEntity(rit->GetEnterprisesDatabase());
		enterpriseController.Get(enterprise);
		enterpriseComboBox->Append(
			enterprise->Get(_N("company"))->ToString(),
			enterprise
		);
		count++;
	}

	if(!enterpriseComboBox->SetStringSelection(lastSelectedEnterprise))
		if(enterpriseComboBox->GetCount() > 0)
			enterpriseComboBox->SetSelection(0);

	return count;
}

unsigned int LoginDialog::UnloadEnterprises() {
	lastSelectedEnterprise = enterpriseComboBox->GetValue();
	EnterpriseEntity *enterprise = NULL;
	const unsigned int count = enterpriseComboBox->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		enterprise = static_cast<EnterpriseEntity *>(enterpriseComboBox->GetClientData(i));
		delete enterprise;
	}
	enterpriseComboBox->Clear();

	return count;
}

unsigned int LoginDialog::ReloadEnterprises() {
	UnloadEnterprises();
	return LoadEnterprises();
}

void LoginDialog::DynSoft() {
	wxLaunchDefaultBrowser(GUG_IT_URL);
}

void LoginDialog::LoginInformation() {
	InformationDialog *informationDialog = new InformationDialog(this, -1, rit);
	informationDialog->Center();
	informationDialog->ShowModal();
	informationDialog->Destroy();
}

bool LoginDialog::IsDemoVersion() {
	return demoVersion;
}

void LoginDialog::LoginHelp() {
	const wxString handbook(rit->GRP(_N("doc"), _N("Handbook.pdf")));
	if(!wxFileExists(handbook)) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not find ") + handbook + _(". Please contact us to get the file."),
			_("Help")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}
	
	wxMimeTypesManager manager;
	wxFileType *type = manager.GetFileTypeFromExtension(_N("pdf"));
	wxString command;
	if(type && type->GetOpenCommand(&command, handbook)) {
		wxExecute(command);
	} else {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not find pdf reader. Please look for ") + handbook + _(" on you computer and open it manually or install a pdf reader."),
			_("Help")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}
}
