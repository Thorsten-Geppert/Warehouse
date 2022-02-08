#include "PreferencesModulePanel.h"

#include <wx/xrc/xmlres.h>
#include <wx/mimetype.h>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/dirdlg.h>
#include <DSCasts.h>
#include <DSDateTime.h>
#include <DSTextCtrl.h>
#include <DSDebug.h>

#include "../lib/EnterprisePreferencesController.h"
#include "../lib/EnterprisePreferencesEntity.h"
#include "../lib/CommonPreferencesController.h"
#include "../lib/CommonPreferencesEntity.h"
#include "../lib/UserController.h"
#include "../lib/PermissionLibrary.h"
#include "../lib/ListEntryEntity.h"
#include "../lib/ListEntryController.h"
#include "../lib/UpdateType.h"
#include "../lib/UpdateLibrary.h"
#include "../lib/ExportLibrary.h"
#include "../lib/ExportType.h"
#include "../lib/BankController.h"
#include "../lib/TemplateController.h"
#include "../lib/TemplateEntity.h"
#include "../lib/StockController.h"
#include "../lib/AddressType.h"
#include "../lib/KeyValueDatabaseLibrary.h"

#include "UpdateDialog.h"
#include "Configuration.h"
#include "UserDialog.h"
#include "InformationDialog.h"
#include "LogDialog.h"
#include "ExportDialog.h"
#include "BanksFullFindDialog.h"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"
#include "ProgressDialog.h"
#include "YesNoDialog.h"
#include "TemplateEditDialog.h"
#include "StockDialog.h"
#include "ReportGeneratorFrame.h"
#include "TextInputDialog.h"

BEGIN_EVENT_TABLE(PreferencesModulePanel, ModulePanel)
	EVT_BUTTON(XRCID("bankFindButton"), PreferencesModulePanel::BankFindEvent)
	EVT_BUTTON(XRCID("saveEnterprisePreferencesButton"), PreferencesModulePanel::SaveEnterprisePreferencesEvent)
	EVT_BUTTON(XRCID("saveCommonPreferencesButton"), PreferencesModulePanel::SaveCommonPreferencesEvent)
	EVT_BUTTON(XRCID("addUserButton"), PreferencesModulePanel::AddUserEvent)
	EVT_BUTTON(XRCID("removeUserButton"), PreferencesModulePanel::RemoveUserEvent)
	EVT_BUTTON(XRCID("exportUsersButton"), PreferencesModulePanel::ExportUsersEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("userListCtrl"), PreferencesModulePanel::EditUserEvent)
	EVT_LISTBOX(XRCID("fillInDataTablesListBox"), PreferencesModulePanel::ChangeTableEntryEvent)
	EVT_BUTTON(XRCID("fillInDataSaveEntryButton"), PreferencesModulePanel::SaveTableEntryEvent)
	EVT_TEXT_ENTER(XRCID("fillInDataEntryTextCtrl"), PreferencesModulePanel::SaveTableEntryEvent)
	EVT_BUTTON(XRCID("fillInDataRemoveEntryButton"), PreferencesModulePanel::RemoveTableEntryEvent)
	EVT_BUTTON(XRCID("checkUpdatesButton"), PreferencesModulePanel::CheckForUpdatesEvent)
	EVT_MENU(LOG_EVENT, PreferencesModulePanel::LogKeyEvent)
	EVT_MENU(CONTACT_EVENT, PreferencesModulePanel::ContactKeyEvent)
	EVT_MENU(BUG_EVENT, PreferencesModulePanel::BugKeyEvent)
	EVT_MENU(INFORMATION_EVENT, PreferencesModulePanel::InformationKeyEvent)
	EVT_MENU(HELP_EVENT, PreferencesModulePanel::HelpKeyEvent)
	EVT_BUTTON(XRCID("fillInDataBankFindButton"), PreferencesModulePanel::FindBankEvent)
	EVT_BUTTON(XRCID("fillInDataBankSaveButton"), PreferencesModulePanel::SaveBankEvent)
	EVT_BUTTON(XRCID("fillInDataBankImportButton"), PreferencesModulePanel::ImportBankEvent)
	EVT_BUTTON(XRCID("fillInDataBankExportButton"), PreferencesModulePanel::ExportBankEvent)
	EVT_BUTTON(XRCID("fillInDataBankRemoveButton"), PreferencesModulePanel::RemoveBankEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("fillInDataBankListCtrl"), PreferencesModulePanel::EditBankEvent)
	EVT_TEXT_ENTER(XRCID("fillInDataBankTextCtrl"), PreferencesModulePanel::SaveBankEvent)
	EVT_TEXT_ENTER(XRCID("fillInDataBankCodeTextCtrl"), PreferencesModulePanel::SaveBankEvent)
	EVT_BUTTON(XRCID("textTemplateAddButton"), PreferencesModulePanel::TextTemplateAddEvent)
	EVT_BUTTON(XRCID("textTemplateRemoveButton"), PreferencesModulePanel::TextTemplateRemoveEvent)
	EVT_BUTTON(XRCID("textTemplateEditButton"), PreferencesModulePanel::TextTemplateEditEvent)
	EVT_BUTTON(XRCID("textTemplateRefreshButton"), PreferencesModulePanel::TextTemplateRefreshEvent)
	EVT_LISTBOX(XRCID("textTemplateListBox"), PreferencesModulePanel::TextTemplateSelectEvent)
	EVT_LISTBOX_DCLICK(XRCID("textTemplateListBox"), PreferencesModulePanel::TextTemplateEditEvent)
	EVT_BUTTON(XRCID("addStockButton"), PreferencesModulePanel::StockAddEvent)
	EVT_BUTTON(XRCID("removeStockButton"), PreferencesModulePanel::StockRemoveEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("stockListCtrl"), PreferencesModulePanel::StockEditEvent)
	EVT_BUTTON(XRCID("reportGeneratorStartButton"), PreferencesModulePanel::ReportGeneratorStartEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("advancedPreferencesListCtrl"), PreferencesModulePanel::AdvancedPreferencesEditEvent)
	EVT_BUTTON(XRCID("reportPathWindowsBrowseButton"), PreferencesModulePanel::ReportPathBrowseEvent)
	EVT_BUTTON(XRCID("reportPathUnixBrowseButton"), PreferencesModulePanel::ReportPathBrowseEvent)
	EVT_BUTTON(XRCID("reportPathMacOSBrowseButton"), PreferencesModulePanel::ReportPathBrowseEvent)
	EVT_BUTTON(XRCID("jreHomeWindowsBrowseButton"), PreferencesModulePanel::JreHomeBrowseEvent)
	EVT_BUTTON(XRCID("jreHomeUnixBrowseButton"), PreferencesModulePanel::JreHomeBrowseEvent)
	EVT_BUTTON(XRCID("jreHomeMacOSBrowseButton"), PreferencesModulePanel::JreHomeBrowseEvent)
END_EVENT_TABLE()

PreferencesModulePanel::PreferencesModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("PreferencesModulePanel")
) {
	actualBankId = NULL;

	// Toolbar
	informationToolbarButton               = XRCCTRL(*this, "informationToolbarButton", ToolbarButton);
	logToolbarButton                       = XRCCTRL(*this, "logToolbarButton", ToolbarButton);
	helpToolbarButton                      = XRCCTRL(*this, "helpToolbarButton", ToolbarButton);
	//CONTACT: bugToolbarButton                       = XRCCTRL(*this, "bugToolbarButton", ToolbarButton);
	//CONTACT: contactToolbarButton                   = XRCCTRL(*this, "contactToolbarButton", ToolbarButton);

	// Informations
	connectionTextCtrl                     = XRCCTRL(*this, "connectionTextCtrl", wxTextCtrl);

	// System
	systemTextCtrl                         = XRCCTRL(*this, "systemTextCtrl", wxTextCtrl);

	// Tree
	optionTreebook                         = XRCCTRL(*this, "optionTreebook", wxTreebook);

	// Enterprise Preferences
	enterprisePreferencesId                = 0;
	companyTextCtrl                        = XRCCTRL(*this, "companyTextCtrl", wxTextCtrl);
	executiveTextCtrl                      = XRCCTRL(*this, "executiveTextCtrl", wxTextCtrl);
	address1TextCtrl                       = XRCCTRL(*this, "address1TextCtrl", wxTextCtrl);
	address2TextCtrl                       = XRCCTRL(*this, "address2TextCtrl", wxTextCtrl);
	zipcodeTextCtrl                        = XRCCTRL(*this, "zipcodeTextCtrl", wxTextCtrl);
	cityTextCtrl                           = XRCCTRL(*this, "cityTextCtrl", wxTextCtrl);
	salesTaxIdentificationNumberTextCtrl   = XRCCTRL(*this, "salesTaxIdentificationNumberTextCtrl", wxTextCtrl);
	taxNumberTextCtrl                      = XRCCTRL(*this, "taxNumberTextCtrl", wxTextCtrl);
	salesTaxIndicatorTextCtrl              = XRCCTRL(*this, "salesTaxIndicatorTextCtrl", wxTextCtrl);
	telephoneTextCtrl                      = XRCCTRL(*this, "telephoneTextCtrl", wxTextCtrl);
	faxTextCtrl                            = XRCCTRL(*this, "faxTextCtrl", wxTextCtrl);
	emailTextCtrl                          = XRCCTRL(*this, "emailTextCtrl", wxTextCtrl);
	webTextCtrl                            = XRCCTRL(*this, "webTextCtrl", wxTextCtrl);
	bankTextCtrl                           = XRCCTRL(*this, "bankTextCtrl", wxTextCtrl);
	bankCodeTextCtrl                       = XRCCTRL(*this, "bankCodeTextCtrl", DynSoft::DSTextCtrl);
	holderTextCtrl                         = XRCCTRL(*this, "holderTextCtrl", wxTextCtrl);
	accountNumberTextCtrl                  = XRCCTRL(*this, "accountNumberTextCtrl", DynSoft::DSTextCtrl);
	bicTextCtrl                            = XRCCTRL(*this, "bicTextCtrl", DynSoft::DSTextCtrl);
	ibanTextCtrl                           = XRCCTRL(*this, "ibanTextCtrl", DynSoft::DSTextCtrl);
	mandatoryClientsOwnClientIdCheckBox    = XRCCTRL(*this, "mandatoryClientsOwnClientIdCheckBox", wxCheckBox);
	mandatoryProductsArticleNumberCheckBox = XRCCTRL(*this, "mandatoryProductsArticleNumberCheckBox", wxCheckBox);

	bankCodeTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	accountNumberTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	// Common Preferences
	commonPreferencesId           = 0;
	minorValueAddedTaxTextCtrl    = XRCCTRL(*this, "minorValueAddedTaxTextCtrl", DynSoft::DSTextCtrl);
	majorValueAddedTaxTextCtrl    = XRCCTRL(*this, "majorValueAddedTaxTextCtrl", DynSoft::DSTextCtrl);
	monitionStatesSpinCtrl        = XRCCTRL(*this, "monitionStatesSpinCtrl", wxSpinCtrl);
	monitionPriceTextCtrl         = XRCCTRL(*this, "monitionPriceTextCtrl", DynSoft::DSTextCtrl);
	reportUseCheckBox             = XRCCTRL(*this, "reportUseCheckBox", DynSoft::DSCheckBox);
	reportPathWindowsTextCtrl     = XRCCTRL(*this, "reportPathWindowsTextCtrl", DynSoft::DSTextCtrl);
	reportPathUnixTextCtrl        = XRCCTRL(*this, "reportPathUnixTextCtrl", DynSoft::DSTextCtrl);
	reportPathMacOSTextCtrl       = XRCCTRL(*this, "reportPathMacOSTextCtrl", DynSoft::DSTextCtrl);
	reportPathWindowsBrowseButton = XRCCTRL(*this, "reportPathWindowsBrowseButton", wxButton);
	reportPathUnixBrowseButton    = XRCCTRL(*this, "reportPathUnixBrowseButton", wxButton);
	reportPathMacOSBrowseButton   = XRCCTRL(*this, "reportPathMacOSBrowseButton", wxButton);
	jreHomeWindowsTextCtrl        = XRCCTRL(*this, "jreHomeWindowsTextCtrl", DynSoft::DSTextCtrl);
	jreHomeUnixTextCtrl           = XRCCTRL(*this, "jreHomeUnixTextCtrl", DynSoft::DSTextCtrl);
	jreHomeMacOSTextCtrl          = XRCCTRL(*this, "jreHomeMacOSTextCtrl", DynSoft::DSTextCtrl);
	jreHomeWindowsBrowseButton    = XRCCTRL(*this, "jreHomeWindowsBrowseButton", wxButton);
	jreHomeUnixBrowseButton       = XRCCTRL(*this, "jreHomeUnixBrowseButton", wxButton);
	jreHomeMacOSBrowseButton      = XRCCTRL(*this, "jreHomeMacOSBrowseButton", wxButton);

	minorValueAddedTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	majorValueAddedTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	monitionPriceTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);

	// Advanced Preferences
	advancedPreferencesListCtrl = XRCCTRL(*this, "advancedPreferencesListCtrl", DynSoft::DSListCtrl);

	advancedPreferencesListCtrl->InsertColumn(0, _("Key"), wxLIST_FORMAT_LEFT, 400);
	advancedPreferencesListCtrl->InsertColumn(1, _("Value"), wxLIST_FORMAT_LEFT, 400);

	// Version
	versionStaticText = XRCCTRL(*this, "versionStaticText", wxStaticText);
	versionStaticText->SetLabel(_(" Version ") + VERSION);

	// User management
	userListCtrl = XRCCTRL(*this, "userListCtrl", DynSoft::DSListCtrl);
	
	userListCtrl->InsertColumn(0, _("Username"), wxLIST_FORMAT_LEFT, 150);
	userListCtrl->InsertColumn(1, _("Real name"), wxLIST_FORMAT_LEFT, 150);
	userListCtrl->InsertColumn(2, _("E-Mail"), wxLIST_FORMAT_LEFT, 150);
	userListCtrl->InsertColumn(3, _("Remind"), wxLIST_FORMAT_LEFT, 150);
	userListCtrl->InsertColumn(4, _("Status"), wxLIST_FORMAT_LEFT, 350);

	// Fill in data
	fillInDataTablesListBox       = XRCCTRL(*this, "fillInDataTablesListBox", wxListBox);
	fillInDataEntryStaticText     = XRCCTRL(*this, "fillInDataEntryStaticText", wxStaticText);
	fillInDataEntryTextCtrl       = XRCCTRL(*this, "fillInDataEntryTextCtrl", wxTextCtrl);
	fillInDataEntriesListBox      = XRCCTRL(*this, "fillInDataEntriesListBox", wxListBox);
	fillInDataSaveEntryButton     = XRCCTRL(*this, "fillInDataSaveEntryButton", wxButton);
	fillInDataRemoveEntryButton   = XRCCTRL(*this, "fillInDataRemoveEntryButton", wxButton);

	fillInDataTablesListBox->Append(_("Titles"));
	fillInDataTablesListBox->Append(_("Salutations"));
	fillInDataTablesListBox->Append(_("Lands"));
	fillInDataTablesListBox->Append(_("Conditions"));
	fillInDataTablesListBox->Append(_("Delivery"));
	fillInDataTablesListBox->Append(_("Federal states"));
	fillInDataTablesListBox->Append(_("Product groups"));
	fillInDataTablesListBox->Append(_("Product packages"));
	fillInDataTablesListBox->Append(_("Distributors"));
	fillInDataTablesListBox->Append(_("Manufacturers"));
	fillInDataTablesListBox->Append(_("Publishers"));
	fillInDataTablesListBox->Append(_("Departments"));
	fillInDataTablesListBox->Append(_("Functions"));
	fillInDataTablesListBox->Append(_("Descriptions"));
	fillInDataTablesListBox->Append(_("Linked media description"));
	fillInDataTablesListBox->Append(_("Groups"));
	fillInDataTablesListBox->Append(_("Sectors"));

	// Bank
	fillInDataBankTextCtrl       = XRCCTRL(*this, "fillInDataBankTextCtrl", wxTextCtrl);
	fillInDataBankCodeTextCtrl   = XRCCTRL(*this, "fillInDataBankCodeTextCtrl", DynSoft::DSTextCtrl);
	fillInDataBankBicTextCtrl    = XRCCTRL(*this, "fillInDataBankBicTextCtrl", DynSoft::DSTextCtrl);
	fillInDataBankFindButton     = XRCCTRL(*this, "fillInDataBankFindButton", wxButton);
	fillInDataBankSaveButton     = XRCCTRL(*this, "fillInDataBankSaveButton", wxButton);
	fillInDataBankListCtrl       = XRCCTRL(*this, "fillInDataBankListCtrl", DynSoft::DSListCtrl);
	fillInDataBankImportButton   = XRCCTRL(*this, "fillInDataBankImportButton", wxButton);
	fillInDataBankExportButton   = XRCCTRL(*this, "fillInDataBankExportButton", wxButton);
	fillInDataBankRemoveButton   = XRCCTRL(*this, "fillInDataBankRemoveButton", wxButton);

	fillInDataBankListCtrl->InsertColumn(0, _("Bank"), wxLIST_FORMAT_LEFT, 400);
	fillInDataBankListCtrl->InsertColumn(1, _("Bank code"), wxLIST_FORMAT_RIGHT, 200);
	fillInDataBankListCtrl->InsertColumn(2, _("BIC"), wxLIST_FORMAT_LEFT, 200);

	fillInDataBankCodeTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	// Text templates
	textTemplateListBox       = XRCCTRL(*this, "textTemplateListBox", wxListBox);
	textTemplateTextCtrl      = XRCCTRL(*this, "textTemplateTextCtrl", wxTextCtrl);
	textTemplateRemoveButton  = XRCCTRL(*this, "textTemplateRemoveButton", wxButton);
	textTemplateEditButton    = XRCCTRL(*this, "textTemplateEditButton", wxButton);
	textTemplateAddButton     = XRCCTRL(*this, "textTemplateAddButton", wxButton);
	textTemplateRefreshButton = XRCCTRL(*this, "textTemplateRefreshButton", wxButton);

	TextTemplateRefresh();

	// Stock management
	stockListCtrl     = XRCCTRL(*this, "stockListCtrl", DynSoft::DSListCtrl);
	removeStockButton = XRCCTRL(*this, "removeStockButton", wxButton);
	addStockButton    = XRCCTRL(*this, "addStockButton", wxButton);

	stockListCtrl->InsertColumn(0, _("Name"), wxLIST_FORMAT_LEFT, 200);
	stockListCtrl->InsertColumn(1, _("Place"), wxLIST_FORMAT_LEFT, 300);
	stockListCtrl->InsertColumn(2, _("Active"), wxLIST_FORMAT_LEFT, 80);
	
	// Reports
	reportGeneratorStartButton = XRCCTRL(*this, "reportGeneratorStartButton", wxButton);

	// Events
	CONNECT_TOOLBARBUTTON(informationToolbarButton, PreferencesModulePanel::InformationEvent);
	CONNECT_TOOLBARBUTTON(logToolbarButton, PreferencesModulePanel::LogEvent);
	CONNECT_TOOLBARBUTTON(helpToolbarButton, PreferencesModulePanel::HelpEvent);
	//CONTACT: CONNECT_TOOLBARBUTTON(bugToolbarButton, PreferencesModulePanel::BugEvent);
	//CONTACT: CONNECT_TOOLBARBUTTON(contactToolbarButton, PreferencesModulePanel::ContactEvent);

	wxAcceleratorEntry acceleratorEntries[5];
	acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 'l',  LOG_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, (int) 'k',  CONTACT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL, (int) 'b',  BUG_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL, (int) 'i',  INFORMATION_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL, (int) 'h',  HELP_EVENT);
	wxAcceleratorTable acceleratorTable(5, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// General
	bugDialog            = new BugDialog(this, -1, rit);
	contactDialog        = new ContactDialog(this, -1, rit);

	// Fill
	wxString connection;
	connection += _("Main connection\n\n");
	connection += _("    Hostname: ") + rit->GetEnterprisesDatabase()->GetHostname() + _N("\n");
	connection += _("    Database: ") + rit->GetEnterprisesDatabase()->GetDatabase() + _N("\n");
	connection += _("    Username: ") + rit->GetEnterprisesDatabase()->GetUsername() + _N("\n");
	connection += _("        Port: ") + rit->GetEnterprisesDatabase()->GetPort() + _N("\n");
	connection += _("\n\nEnterprise connection\n\n");
	connection += _("    Hostname: ") + rit->GetDatabase()->GetHostname() + _N("\n");
	connection += _("    Database: ") + rit->GetDatabase()->GetDatabase() + _N("\n");
	connection += _("    Username: ") + rit->GetDatabase()->GetUsername() + _N("\n");
	connection += _("        Port: ") + rit->GetDatabase()->GetPort();

	connectionTextCtrl->SetValue(connection);

	wxLongLong diskSpace, freeDiskSpace;
	wxGetDiskSpace(wxGetCwd(), &diskSpace, &freeDiskSpace);
	diskSpace = diskSpace / 1024 / 1024 / 1024;
	freeDiskSpace = freeDiskSpace / 1024 / 1024 / 1024;
	wxString freeMemoryString(_("n/a"));
	wxLongLong freeMemory = wxGetFreeMemory();
	if(freeMemory >= 0) {
		freeMemory = freeMemory / 1024 / 1024;
		freeMemoryString = freeMemory.ToString() + _N(" MB");
	}
	int displayWidth = 0;
	int displayHeight = 0;
	wxDisplaySize(&displayWidth, &displayHeight);

	wxString system;
	system += _("System informations\n\n");
	system += _("                      Date: ") + wxNow() + _N("\n");
	system += _("          Operating System: ") + wxGetOsDescription() + _N("\n");
	system += _("Operating System directory: ") + wxGetOSDirectory() + _N("\n");
	system += _("                Disk space: ") + diskSpace.ToString() + _N(" GB \n");
	system += _("           Free disk space: ") + freeDiskSpace.ToString() + _N(" GB\n");
	system += _("               Free memory: ") + freeMemoryString + _N("\n");
	system += _("                Process id: ") + DSCasts::ToString(wxGetProcessId()) + _N("\n");
	system += _("                   User id: ") + wxGetUserId() + _N("\n");
	system += _("                  Username: ") + wxGetUserName() + _N("\n");
	system += _("            Home directory: ") + wxGetHomeDir() + _N("\n");
	system += _("         Working directory: ") + wxGetCwd() + _N("\n");
	system += _("                  Hostname: ") + wxGetHostName() + _N("\n");
	system += _("             Full hostname: ") + wxGetFullHostName() + _N("\n");
	system += _("              Display size: ") + DSCasts::ToString(displayWidth) + _N(" x ") + DSCasts::ToString(displayHeight) + _N("\n");

	systemTextCtrl->SetValue(system);

	AdvancedPreferencesLoad();
}

PreferencesModulePanel::~PreferencesModulePanel() {
	userListCtrl->CleanUp();
	fillInDataBankListCtrl->CleanUp();
	TextTemplateCleanUp();
	stockListCtrl->CleanUp();
	advancedPreferencesListCtrl->CleanUp();
}

void PreferencesModulePanel::Load() {
	EnterprisePreferencesController enterprisePreferencesController(rit->GetDatabase());
	enterprisePreferencesController.Get();
	if(enterprisePreferencesController.Run()) {
		EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());
		enterprisePreferencesController.Get(&enterprisePreferencesEntity);
		enterprisePreferencesController.Reset();

		enterprisePreferencesId = enterprisePreferencesEntity.Get(_N("id"))->ToUnsignedInt();
		companyTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("company"))->ToString());
		executiveTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("executive"))->ToString());
		address1TextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("address1"))->ToString());
		address2TextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("address2"))->ToString());
		zipcodeTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("zipcode"))->ToString());
		cityTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("city"))->ToString());
		salesTaxIdentificationNumberTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("salesTaxIdentificationNumber"))->ToString());
		taxNumberTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("taxNumber"))->ToString());
		salesTaxIndicatorTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("salesTaxIndicator"))->ToString());
		telephoneTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("telephone"))->ToString());
		faxTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("fax"))->ToString());
		emailTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("email"))->ToString());
		webTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("web"))->ToString());
		bankTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("bank"))->ToString());
		bankCodeTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("bankCode"))->ToString());
		holderTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("holder"))->ToString());
		accountNumberTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("accountNumber"))->ToString());
		bicTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("bic"))->ToString());
		ibanTextCtrl->SetValue(enterprisePreferencesEntity.Get(_N("iban"))->ToString());
		mandatoryProductsArticleNumberCheckBox->SetValue(enterprisePreferencesEntity.Get(_N("mandatoryProductsArticleNumber"))->ToBool());
		mandatoryClientsOwnClientIdCheckBox->SetValue(enterprisePreferencesEntity.Get(_N("mandatoryClientsOwnClientId"))->ToBool());
	}

	CommonPreferencesController commonPreferencesController(rit->GetDatabase());
	commonPreferencesController.Get();
	if(commonPreferencesController.Run()) {
		CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());
		commonPreferencesController.Get(&commonPreferencesEntity);
		commonPreferencesController.Reset();

		commonPreferencesId = commonPreferencesEntity.Get(_N("id"))->ToUnsignedInt();
		minorValueAddedTaxTextCtrl->SetValue(commonPreferencesEntity.Get(_N("minorValueAddedTax"))->ToCurrency().ToString());
		majorValueAddedTaxTextCtrl->SetValue(commonPreferencesEntity.Get(_N("majorValueAddedTax"))->ToCurrency().ToString());
		monitionStatesSpinCtrl->SetValue(commonPreferencesEntity.Get(_N("monitionStates"))->ToInt());
		monitionPriceTextCtrl->SetValue(commonPreferencesEntity.Get(_N("monitionPrice"))->ToCurrency().ToString());
		reportUseCheckBox->SetValue(commonPreferencesEntity.Get(_N("reportUse"))->ToBool());
		reportPathWindowsTextCtrl->SetValue(commonPreferencesEntity.Get(_N("reportPathWindows"))->ToString());
		reportPathUnixTextCtrl->SetValue(commonPreferencesEntity.Get(_N("reportPathUnix"))->ToString());
		reportPathMacOSTextCtrl->SetValue(commonPreferencesEntity.Get(_N("reportPathMacOS"))->ToString());
		jreHomeWindowsTextCtrl->SetValue(commonPreferencesEntity.Get(_N("jreHomeWindows"))->ToString());
		jreHomeUnixTextCtrl->SetValue(commonPreferencesEntity.Get(_N("jreHomeUnix"))->ToString());
		jreHomeMacOSTextCtrl->SetValue(commonPreferencesEntity.Get(_N("jreHomeMacOS"))->ToString());
	}

	UserEntity *userEntity = NULL;
	UserController userController(rit);
	userController.Get();
	while(userController.Run()) {
		userEntity = new UserEntity(rit->GetDatabase());
		userController.Get(userEntity);
		AppendUser(userEntity);
	}

	StockEntity *stockEntity = NULL;
	StockController stockController(rit);
	stockController.Get();
	while(stockController.Run()) {
		stockEntity = new StockEntity(rit);
		stockController.Get(stockEntity);
		AddOrChangeToStocksListCtrl(stockEntity);
	}

	EnableFillInData(false);

	optionTreebook->SetFocus();
}

void PreferencesModulePanel::Unload() {
	userListCtrl->CleanUp();
	stockListCtrl->CleanUp();
}

void PreferencesModulePanel::SaveEnterprisePreferencesEvent(wxCommandEvent &event) {
	SaveEnterprisePreferences();
}

void PreferencesModulePanel::SaveCommonPreferencesEvent(wxCommandEvent &event) {
	SaveCommonPreferences();
}

void PreferencesModulePanel::AddUserEvent(wxCommandEvent &event) {
	AddUser();
}

void PreferencesModulePanel::EditUserEvent(wxListEvent &event) {
	EditUser();
}

void PreferencesModulePanel::RemoveUserEvent(wxCommandEvent &event) {
	RemoveUser();
}

void PreferencesModulePanel::InformationEvent(wxMouseEvent &event) {
	SKIP();
	Information();
}

void PreferencesModulePanel::LogEvent(wxMouseEvent &event) {
	Log();
	SKIP();
}

void PreferencesModulePanel::HelpEvent(wxMouseEvent &event) {
	Help();
	SKIP();
}

void PreferencesModulePanel::BugEvent(wxMouseEvent &event) {
	Bug();
	SKIP();
}

void PreferencesModulePanel::ContactEvent(wxMouseEvent &event) {
	Contact();
	SKIP();
}

void PreferencesModulePanel::LogKeyEvent(wxCommandEvent &event) {
	Log();
}

void PreferencesModulePanel::ContactKeyEvent(wxCommandEvent &event) {
	Contact();
}

void PreferencesModulePanel::BugKeyEvent(wxCommandEvent &event) {
	Bug();
}

void PreferencesModulePanel::InformationKeyEvent(wxCommandEvent &event) {
	Information();
}

void PreferencesModulePanel::HelpKeyEvent(wxCommandEvent &event) {
	Help();
}

void PreferencesModulePanel::ChangeTableEntryEvent(wxCommandEvent &event) {
	ChangeTableEntry();
}

void PreferencesModulePanel::SaveTableEntryEvent(wxCommandEvent &event) {
	SaveTableEntry();
}

void PreferencesModulePanel::RemoveTableEntryEvent(wxCommandEvent &event) {
	RemoveTableEntry();
}

void PreferencesModulePanel::CheckForUpdatesEvent(wxCommandEvent &event) {
	CheckForUpdates();
}

void PreferencesModulePanel::ExportUsersEvent(wxCommandEvent &event) {
	ExportUsers();
}

void PreferencesModulePanel::FindBankEvent(wxCommandEvent &event) {
	FindBank(
		fillInDataBankTextCtrl->GetValue(),
		fillInDataBankCodeTextCtrl->GetValue(),
		fillInDataBankBicTextCtrl->GetValue()
	);
}

void PreferencesModulePanel::SaveBankEvent(wxCommandEvent &event) {
	SaveBank();
}

void PreferencesModulePanel::ImportBankEvent(wxCommandEvent &event) {
	ImportBank();
}

void PreferencesModulePanel::ExportBankEvent(wxCommandEvent &event) {
	ExportBank();
}

void PreferencesModulePanel::RemoveBankEvent(wxCommandEvent &event) {
	RemoveBank(static_cast<unsigned int *>(fillInDataBankListCtrl->GetSingleSelectionPointer()));
}

void PreferencesModulePanel::BankFindEvent(wxCommandEvent &event) {
	BankFind();
}

void PreferencesModulePanel::EditBankEvent(wxListEvent &event) {
	EditBank(static_cast<unsigned int *>(fillInDataBankListCtrl->GetSingleSelectionPointer()));
}

void PreferencesModulePanel::TextTemplateRemoveEvent(wxCommandEvent &event) {
	TextTemplateRemove(textTemplateListBox->GetSelection());
}

void PreferencesModulePanel::TextTemplateSelectEvent(wxCommandEvent &event) {
	TextTemplateSelect(event.GetSelection());
}

void PreferencesModulePanel::TextTemplateEditEvent(wxCommandEvent &event) {
	TextTemplateEdit(event.GetSelection());
}

void PreferencesModulePanel::TextTemplateAddEvent(wxCommandEvent &event) {
	TextTemplateAdd();
}

void PreferencesModulePanel::TextTemplateRefreshEvent(wxCommandEvent &event) {
	TextTemplateRefresh();
}

void PreferencesModulePanel::StockAddEvent(wxCommandEvent &event) {
	StockAdd();
}

void PreferencesModulePanel::StockEditEvent(wxListEvent &event) {
	StockEdit();
}

void PreferencesModulePanel::StockRemoveEvent(wxCommandEvent &event) {
	StockRemove();
}

void PreferencesModulePanel::ReportGeneratorStartEvent(wxCommandEvent &event) {
	ReportGeneratorStart();
}

void PreferencesModulePanel::AdvancedPreferencesEditEvent(wxListEvent &event) {
	AdvancedPreferencesEdit(static_cast<KeyValueType *>(advancedPreferencesListCtrl->GetSingleSelectionPointer()));
}

void PreferencesModulePanel::ReportPathBrowseEvent(wxCommandEvent &event) {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to change preferences."), this))
		return;

	wxTextCtrl *pathTextCtrl = NULL;

	wxButton *pressedButton = static_cast<wxButton *>(event.GetEventObject());
	if(pressedButton == reportPathWindowsBrowseButton)
		pathTextCtrl = reportPathWindowsTextCtrl;
	else if(pressedButton == reportPathUnixBrowseButton)
		pathTextCtrl = reportPathUnixTextCtrl;
	else if(pressedButton == reportPathMacOSBrowseButton)
		pathTextCtrl = reportPathMacOSTextCtrl;
	else
		pathTextCtrl = NULL;

	ReportPathBrowse(pathTextCtrl);
}

void PreferencesModulePanel::JreHomeBrowseEvent(wxCommandEvent &event) {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to change preferences."), this))
		return;

	wxTextCtrl *jreHomeTextCtrl = NULL;

	wxButton *pressedButton = static_cast<wxButton *>(event.GetEventObject());
	if(pressedButton == jreHomeWindowsBrowseButton)
		jreHomeTextCtrl = jreHomeWindowsTextCtrl;
	else if(pressedButton == jreHomeUnixBrowseButton)
		jreHomeTextCtrl = jreHomeUnixTextCtrl;
	else if(pressedButton == jreHomeMacOSBrowseButton)
		jreHomeTextCtrl = jreHomeMacOSTextCtrl;
	else
		jreHomeTextCtrl = NULL;

	JreHomeBrowse(jreHomeTextCtrl);
}

void PreferencesModulePanel::SaveEnterprisePreferences() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to change preferences."), this))
		return;

	EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());

	enterprisePreferencesEntity.SetValue(_N("id"), enterprisePreferencesId);
	enterprisePreferencesEntity.SetValue(_N("company"), companyTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("executive"), executiveTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("address1"), address1TextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("address2"), address2TextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("zipcode"), zipcodeTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("city"), cityTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("salesTaxIdentificationNumber"), salesTaxIdentificationNumberTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("taxNumber"), taxNumberTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("salesTaxIndicator"), salesTaxIndicatorTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("telephone"), telephoneTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("fax"), faxTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("email"), emailTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("web"), webTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("bank"), bankTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("bankCode"), bankCodeTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("holder"), holderTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("accountNumber"), accountNumberTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("bic"), bicTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("iban"), ibanTextCtrl->GetValue());
	enterprisePreferencesEntity.SetValue(_N("mandatoryProductsArticleNumber"), mandatoryProductsArticleNumberCheckBox->GetValue());
	enterprisePreferencesEntity.SetValue(_N("mandatoryClientsOwnClientId"), mandatoryClientsOwnClientIdCheckBox->GetValue());

	if(
		enterprisePreferencesEntity.Get(_N("company"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("address1"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("zipcode"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("city"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("bank"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("bankCode"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("holder"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("accountNumber"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("bic"))->ToString().IsEmpty() ||
		enterprisePreferencesEntity.Get(_N("iban"))->ToString().IsEmpty()
	) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill out the following fields: company, address1, zipcode, city, sales tax identification number, bank, bank code, bic, iban, holder and account number."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	EnterprisePreferencesController enterprisePreferencesController(rit->GetDatabase());
	if(enterprisePreferencesController.InsertOrUpdate(&enterprisePreferencesEntity)) {
		rit->Log(false, _N("PreferencesModulePanel"), _("Preferences are updated."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Preferences are updated."),
			_("Preferences")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		enterprisePreferencesId = enterprisePreferencesEntity.Get(_N("id"))->ToUnsignedInt();
	} else {
		rit->Log(true, _N("PreferencesModulePanel"), _("Preferences could not been updated."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Preferences could not been updated."),
			_("Preferences")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}
}

void PreferencesModulePanel::SaveCommonPreferences() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to change preferences."), this))
		return;

	CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());

	commonPreferencesEntity.SetValue(_N("id"), commonPreferencesId);
	commonPreferencesEntity.SetValue(_N("minorValueAddedTax"), DSCasts::ToDouble(minorValueAddedTaxTextCtrl->GetValue()));
	commonPreferencesEntity.SetValue(_N("majorValueAddedTax"), DSCasts::ToDouble(majorValueAddedTaxTextCtrl->GetValue()));
	commonPreferencesEntity.SetValue(_N("monitionStates"), monitionStatesSpinCtrl->GetValue());
	commonPreferencesEntity.SetValue(_N("monitionPrice"), DSCasts::ToDouble(monitionPriceTextCtrl->GetValue()));
	commonPreferencesEntity.SetValue(_N("reportUse"), reportUseCheckBox->GetValue());
	commonPreferencesEntity.SetValue(_N("reportPathWindows"), reportPathWindowsTextCtrl->GetValue());
	commonPreferencesEntity.SetValue(_N("reportPathUnix"), reportPathUnixTextCtrl->GetValue());
	commonPreferencesEntity.SetValue(_N("reportPathMacOS"), reportPathMacOSTextCtrl->GetValue());
	commonPreferencesEntity.SetValue(_N("jreHomeWindows"), jreHomeWindowsTextCtrl->GetValue());
	commonPreferencesEntity.SetValue(_N("jreHomeUnix"), jreHomeUnixTextCtrl->GetValue());
	commonPreferencesEntity.SetValue(_N("jreHomeMacOS"), jreHomeMacOSTextCtrl->GetValue());

	if(
		commonPreferencesEntity.Get(_N("minorValueAddedTax"))->ToString().IsEmpty() ||
		commonPreferencesEntity.Get(_N("majorValueAddedTax"))->ToString().IsEmpty() ||
		(commonPreferencesEntity.Get(_N("monitionStates"))->ToInt() < 1 || commonPreferencesEntity.Get(_N("monitionStates"))->ToInt() > 10)
	) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill out the following fields: minor value, major value, monition states. Monition states must be between one an ten."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	CommonPreferencesController commonPreferencesController(rit->GetDatabase());
	if(commonPreferencesController.InsertOrUpdate(&commonPreferencesEntity)) {
		rit->Log(false, _N("PreferencesModulePanel"), _("Common preferences are updated."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Preferences are updated"),
			_("Preferences")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();

		commonPreferencesId = commonPreferencesEntity.Get(_N("id"))->ToUnsignedInt();
	} else {
		rit->Log(true, _N("PreferencesModulePanel"), _("Common preferences could not been updated."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Preferences could not been updated"),
			_("Preferences")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}
}

void PreferencesModulePanel::AddUser() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to add users."), this))
		return;

	UserDialog *userDialog = new UserDialog(this, -1, rit);
	userDialog->Center();
	if(userDialog->ShowModal() == wxID_OK)
		AppendUser(userDialog->GetUser());
	userDialog->Destroy();
}

void PreferencesModulePanel::EditUser() {
	UserEntity *userEntity = reinterpret_cast<UserEntity *>(userListCtrl->GetSingleSelectionPointer());
	if(!userEntity) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please select the user you want to edit."),
			_("Edit")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	} else {
		UserDialog *userDialog = new UserDialog(this, -1, rit, userEntity);
		userDialog->Center();
		if(userDialog->ShowModal() == wxID_OK)
			AppendUser(userDialog->GetUser());
		userDialog->Destroy();
	}
}

void PreferencesModulePanel::RemoveUser() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to remove users."), this))
		return;

	UserEntity *userEntity = reinterpret_cast<UserEntity *>(userListCtrl->GetSingleSelectionPointer());
	if(!userEntity) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please select the user you want to remove."),
			_("Remove")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	} else {
		if(rit->GetUserConfigurationFileEntity()->GetLastUsername() == userEntity->Get(_N("username"))->ToString()) {
			wxMessageDialog *messageDialog = new wxMessageDialog(
				this,
				_("You cannot remove yourself."),
				_("Edit")
			);
			messageDialog->Center();
			messageDialog->ShowModal();
			messageDialog->Destroy();
			return;
		}
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you realy want to remove the selected user?"),
			_("Remove"),
			_("Remove"),
			_("Cancel")
		);
		if(yesNoDialog->ShowModal() == wxID_YES) {
			UserController userController(rit);
			if(!userController.Delete(userEntity)) {
				wxMessageDialog *messageDialog = new wxMessageDialog(
					this,
					_("The user could not be deleted."),
					_("Edit")
				);
				messageDialog->Center();
				messageDialog->ShowModal();
				messageDialog->Destroy();
				rit->Log(true, _N("PreferencesModulePanel"), _("The user ") + userEntity->Get(_N("username"))->ToString() + _(" could not been deleted."));
			} else {
				rit->Log(false, _N("PreferencesModulePanel"), _("The user ") + userEntity->Get(_N("username"))->ToString() + _(" is deleted."));
				userListCtrl->DeleteItem(userListCtrl->GetPosition(reinterpret_cast<wxUIntPtr *>(userEntity)));
				delete userEntity;
				userEntity = NULL;
			}
		}
		yesNoDialog->Destroy();
	}
}

void PreferencesModulePanel::AppendUser(UserEntity *userEntity) {
	DSListCtrl::DSListCtrlRow *row = new DSListCtrl::DSListCtrlRow(reinterpret_cast<wxUIntPtr *>(userEntity));
	row->Add(userEntity->Get(_N("username"))->ToString());
	row->Add(userEntity->Get(_N("realName"))->ToString());
	row->Add(userEntity->Get(_N("email"))->ToString());
	row->Add(userEntity->Get(_N("openToRemind"))->ToBool() ? _("yes") : _("no"));
	row->Add(PermissionLibrary::ToString(userEntity->Get(_N("status"))->ToUnsignedInt()));
	userListCtrl->Set(row);
}

void PreferencesModulePanel::Information() {
	InformationDialog *informationDialog = new InformationDialog(this, -1, rit);
	informationDialog->Center();
	informationDialog->ShowModal();
	informationDialog->Destroy();
}

void PreferencesModulePanel::Log() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to view log."), this))
		return;

	LogDialog *logDialog = new LogDialog(this, -1, rit);
	logDialog->Center();
	logDialog->ShowModal();
	logDialog->Destroy();
}

void PreferencesModulePanel::Help() {
	const wxString handbook(rit->GRP(_N("doc"), _N("Handbook.pdf")));
	if(!wxFile::Exists(handbook)) {
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

void PreferencesModulePanel::Bug() {
	bugDialog->Center();
	bugDialog->ShowModal();
}

void PreferencesModulePanel::Contact() {
	contactDialog->Center();
	contactDialog->ShowModal();
}

void PreferencesModulePanel::ChangeTableEntry() {
	const int selection = fillInDataTablesListBox->GetSelection();
	if(selection == wxNOT_FOUND) {
		EnableFillInData(false);
	} else {
		EnableFillInData(true);
		
		wxString table, field;
		GetFillInDataTable(&table, &field);

		if(!table.IsEmpty() && !field.IsEmpty()) {
			ListEntryController listEntryController(rit, table, field);
			fillInDataEntriesListBox->Set(listEntryController.ToArrayString());
		}
	}
}

void PreferencesModulePanel::GetFillInDataTable(wxString *table, wxString *field) {
	if(!table || !field)
		return;

	switch(fillInDataTablesListBox->GetSelection()) {
		case  0: *table = _N("titles");                    *field = _N("title");                    break;
		case  1: *table = _N("salutations");               *field = _N("salutation");               break;
		case  2: *table = _N("lands");                     *field = _N("land");                     break;
		case  3: *table = _N("conditions");                *field = _N("condition");                break;
		case  4: *table = _N("delivery");                  *field = _N("delivery");                 break;
		case  5: *table = _N("federalstate");              *field = _N("federalstate");             break;
		case  6: *table = _N("product_groups");            *field = _N("group");                    break;
		case  7: *table = _N("product_packages");          *field = _N("package");                  break;
		case  8: *table = _N("distributors");              *field = _N("distributor");              break;
		case  9: *table = _N("manufacturers");             *field = _N("manufacturer");             break;
		case 10: *table = _N("publishers");                *field = _N("publisher");                break;
		case 11: *table = _N("departments");               *field = _N("department");               break;
		case 12: *table = _N("functions");                 *field = _N("function");                 break;
		case 13: *table = _N("descriptions");              *field = _N("description");              break;
		case 14: *table = _N("linked_media_descriptions"); *field = _N("linked_media_description"); break;
		case 15: *table = _N("groups");                    *field = _N("group");                    break;
		case 16: *table = _N("sectors");                   *field = _N("sector");                   break;
	}
}

void PreferencesModulePanel::EnableFillInData(const bool enable, const bool withTableListBox) {
	if(withTableListBox)
		fillInDataTablesListBox->Enable(enable);
	fillInDataEntryStaticText->Enable(enable);
	fillInDataEntryTextCtrl->Enable(enable);
	fillInDataEntriesListBox->Enable(enable);
	fillInDataSaveEntryButton->Enable(enable);
	fillInDataRemoveEntryButton->Enable(enable);

	fillInDataEntryTextCtrl->Clear();
	fillInDataEntriesListBox->Clear();
}

void PreferencesModulePanel::SaveTableEntry() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to save entry."), this))
		return;

	wxString table, field;
	GetFillInDataTable(&table, &field);

	if(table.IsEmpty() || field.IsEmpty()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not remove entry. Unkown error (no table or no field)."),
			_("Remove")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return;
	}

	ListEntryEntity listEntryEntity(rit, table, field);
	listEntryEntity.SetValue(field, fillInDataEntryTextCtrl->GetValue());
	ListEntryController listEntryController(rit, table, field);
	if(!listEntryController.Insert(&listEntryEntity, false)) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not save entry. Maybe it allready exists."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		fillInDataEntriesListBox->Append(listEntryEntity.Get(field)->ToString());
		fillInDataEntryTextCtrl->Clear();
		fillInDataEntryTextCtrl->SetFocus();
	}
}

void PreferencesModulePanel::RemoveTableEntry() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission."), this))
		return;

	const int selection = fillInDataEntriesListBox->GetSelection();
	if(selection == wxNOT_FOUND) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the entry you want to remove."),
			_("Remove")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return;
	}

	wxString table, field;
	GetFillInDataTable(&table, &field);

	if(table.IsEmpty() || field.IsEmpty()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not remove entry. Unkown error (no table or no field)."),
			_("Remove")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to remove the selected entry?"),
		_("Remove"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		ListEntryEntity listEntryEntity(rit, table, field);
		listEntryEntity.SetValue(field, fillInDataEntriesListBox->GetStringSelection());
		ListEntryController listEntryController(rit, table, field);
		if(!listEntryController.Delete(&listEntryEntity)) {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not remove selected entry."),
				_("Remove")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		} else {
			fillInDataEntriesListBox->Delete(selection);
		}
	}
	yesNoDialog->Destroy();
}

void PreferencesModulePanel::CheckForUpdates() {
	wxString message;

	UpdateLibrary updateLibrary(UPDATE_URL);
	UpdateType *updateType = updateLibrary.CheckForUpdates(VERSION, &message);

	if(updateType) {
		UpdateDialog *updateDialog = new UpdateDialog(this, -1, rit, updateType);
		updateDialog->ShowModal();
		delete updateType;
	} else {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			message.IsEmpty() ? wxString(_("No updates found.")) : message,
			_("Check for updates")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}
}

void PreferencesModulePanel::ExportUsers() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to export."), this))
		return;

	ExportType exportType;
	ExportDialog *exportDialog = new ExportDialog(this, -1, rit, &exportType);
	exportDialog->Center();

	DSFile file;
	while(exportDialog->ShowModal() == wxID_OK) {
		exportDialog->TransferToExportType();

		if(file.Open(exportType.GetFilename(), _N("w"))) {
			file.Close();

			const unsigned int count = userListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, false);

			for(unsigned int i = 0; i < count; i++)
				exportLibrary.ExportAdd(static_cast<DSEntity *>(userListCtrl->GetPointer(i, 0)));

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Error writing file. Aborting."),
				_("Export")
			);
			errorMessageDialog->Center();

			if(exportLibrary.ExportStart()) {
				if(exportLibrary.ExportRun()) {
					if(!exportLibrary.ExportStop())
						errorMessageDialog->ShowModal();
				} else {
					errorMessageDialog->ShowModal();
				}
			} else {
				errorMessageDialog->ShowModal();
			}

			errorMessageDialog->Destroy();

			break;
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("File is not writeable. Please check file permissions."),
				_("Export")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	exportDialog->Destroy();
}

void PreferencesModulePanel::FindBank(const wxString &bank, const wxString &bankCode, const wxString &bic) {
	if(actualBankId) {
		EnableBankFields(false, NULL);
	} else {
		fillInDataBankListCtrl->CleanUp();

		BankEntity findBankEntity(rit);
		findBankEntity.SetValue(_N("bank"), bank);
		findBankEntity.SetValue(_N("bankCode"), bankCode);
		findBankEntity.SetValue(_N("bic"), bic);
		BankEntity *bankEntity = NULL;
		BankController bankController(rit);
		bankController.Find(&findBankEntity);

		ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, bankController.GetCount());
		progressDialog->Center();
		progressDialog->Start();

		unsigned int i = 0;
		while(bankController.Run()) {
			bankEntity = new BankEntity(rit);
			bankController.Get(bankEntity);

			AddOrChangeToBanksListCtrl(bankEntity, true);

			progressDialog->SetValue(++i);
			if(progressDialog->IsCanceled())
				break;
		}

		progressDialog->Stop();
	}
}

void PreferencesModulePanel::SaveBank() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to save bank."), this))
		return;

	const wxString bank(fillInDataBankTextCtrl->GetValue());
	const wxString bankCode(fillInDataBankCodeTextCtrl->GetValue());
	const wxString bic(fillInDataBankBicTextCtrl->GetValue());

	if(bank.IsEmpty() || bankCode.IsEmpty()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill out bank and bank code."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BankEntity bankEntity(rit);
	if(actualBankId)
		bankEntity.SetValue(_N("bankId"), *actualBankId);
	bankEntity.SetValue(_N("bank"), bank);
	bankEntity.SetValue(_N("bankCode"), bankCode);
	bankEntity.SetValue(_N("bic"), bic);

	BankController bankController(rit);
	const bool ok = actualBankId ? bankController.Update(&bankEntity) : bankController.Insert(&bankEntity);
	if(!ok) {
		rit->Log(true, _N("PreferencesModulePanel"), _("The bank could not been saved."), actualBankId ? wxString(_("Bank id: ") + DSCasts::ToString(*actualBankId)) : wxString(_("New entry.")));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not save bank. Database error."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		rit->Log(false, _N("PreferencesModulePanel"), _("The bank is saved."), actualBankId ? (_("Bank id: ") + DSCasts::ToString(*actualBankId)) : wxString(_("New entry.")));
		AddOrChangeToBanksListCtrl(&bankEntity, !actualBankId);
		EnableBankFields(false, NULL);
	}
}

void PreferencesModulePanel::ImportBank() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to import banks."), this))
		return;

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Please use only the original txt files from ") + BANK_URL + _(". Other files are not supported.\n\n") +
		_("Do you really want to import file? All your changes will be overwritten."),
		_("Hint"),
		_("Import"),
		_("Cancel")
	);
	yesNoDialog->Center();
	bool ok = yesNoDialog->ShowModal() == wxID_YES;
	yesNoDialog->Destroy();

	if(!ok)
		return;
	
	fillInDataBankListCtrl->CleanUp();
	
	wxFileDialog *fileDialog = new wxFileDialog(
		this,
		_("Choose the bank file."),
		bankDefaultDirectory,
		wxEmptyString,
		_N("*.txt"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);
	fileDialog->Center();
	ok = fileDialog->ShowModal() == wxID_OK;
	bankDefaultDirectory = fileDialog->GetPath();
	fileDialog->Destroy();

	if(!ok)
		return;
	
	BankController bankController(rit);
	bankController.ImportSetFilename(bankDefaultDirectory);

	if(!bankController.ImportStart()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not read file."),
			_("Import")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		if(!bankController.DeleteAll()) {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("An error occured: Could not delete all banks from database. Aborting."),
				_("Import")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		} else {
			wxString line;
			const unsigned int count = bankController.ImportGetCount();

			ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, count);
			progressDialog->Center();
			progressDialog->Start();
			
			for(unsigned int i = 0; i < count; i++) {
				if(bankController.ImportRun(bankController.ImportGetLine(i))) {
					progressDialog->SetValue(i + 1);
					if(progressDialog->IsCanceled())
						break;
				} else {
					wxMessageDialog *errorMessageDialog = new wxMessageDialog(
						this,
						_("An error occured: Could save entry to database. Aborting."),
						_("Import")
					);
					errorMessageDialog->Center();
					errorMessageDialog->ShowModal();
					errorMessageDialog->Destroy();

					break;
				}
			}
			progressDialog->Stop();
		}

		if(!bankController.ImportStop()) {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not close file. Maybe the import went wrong."),
				_("Import")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
}

void PreferencesModulePanel::ExportBank() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to export banks."), this))
		return;

	ExportType exportType;
	ExportDialog *exportDialog = new ExportDialog(this, -1, rit, &exportType);
	exportDialog->Center();

	DSFile file;
	while(exportDialog->ShowModal() == wxID_OK) {
		exportDialog->TransferToExportType();

		if(file.Open(exportType.GetFilename(), _N("w"))) {
			file.Close();

			const unsigned int count = fillInDataBankListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, true);

			for(unsigned int i = 0; i < count; i++) {
				exportLibrary.ExportAdd(BankController::GetDirect(
					rit,
					*static_cast<unsigned int *>(fillInDataBankListCtrl->GetPointer(i, 0))
				));
			}

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Error writing file. Aborting."),
				_("Export")
			);
			errorMessageDialog->Center();

			if(exportLibrary.ExportStart()) {
				if(exportLibrary.ExportRun()) {
					if(!exportLibrary.ExportStop())
						errorMessageDialog->ShowModal();
				} else {
					errorMessageDialog->ShowModal();
				}
			} else {
				errorMessageDialog->ShowModal();
			}

			errorMessageDialog->Destroy();

			break;
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("File is not writeable. Please check file permissions."),
				_("Export")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	exportDialog->Destroy();
}

void PreferencesModulePanel::RemoveBank(unsigned int *id) {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to remove bank."), this))
		return;

	if(!id) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select an entry."),
			_("Edit bank")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to remove this entry?"),
		_("Remove bank"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		BankEntity bankEntity(rit);
		bankEntity.SetValue(_N("bankId"), *id);
		BankController bankController(rit);
		if(!bankController.Delete(&bankEntity)) {
			rit->Log(true, _N("PreferencesModulePanel"), _("The bank is not removed."), id ? wxString(_("Bank id: ") + DSCasts::ToString(*id)) : wxString(_("Unknown")));
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not remove bank. Database error."),
				_("Remove bank")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		} else {
			rit->Log(false, _N("PreferencesModulePanel"), _("The bank is removed."), id ? wxString(_("Bank id: ") + DSCasts::ToString(*id)) : wxString(_("Unkown")));
			fillInDataBankListCtrl->DeleteItem(fillInDataBankListCtrl->GetPosition(id));
			delete id;
			EnableBankFields(false, NULL);
		}
	}
	yesNoDialog->Destroy();
}

void PreferencesModulePanel::BankFind() {
	BanksFullFindDialog *banksFullFindDialog = new BanksFullFindDialog(
		this,
		-1,
		rit
	);
	banksFullFindDialog->Center();

	if(banksFullFindDialog->ShowModal() == wxID_OK) {
		BankEntity *bankEntity = banksFullFindDialog->GetBankEntity(); // Will be destroyed automatically
		if(bankEntity) {
			bankTextCtrl->SetValue(bankEntity->Get(_N("bank"))->ToString());
			bankCodeTextCtrl->SetValue(bankEntity->Get(_N("bankCode"))->ToString());
			bicTextCtrl->SetValue(bankEntity->Get(_N("bic"))->ToString());
		}
	}

	banksFullFindDialog->Destroy();
}

void PreferencesModulePanel::AddOrChangeToBanksListCtrl(BankEntity *bankEntity, const bool justAppend) {
	unsigned int *foundBankId = NULL;
	const unsigned int bankId = bankEntity->Get(_N("bankId"))->ToUnsignedInt();

	if(!justAppend) {
		const unsigned int count = fillInDataBankListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			foundBankId = static_cast<unsigned int *>(fillInDataBankListCtrl->GetPointer(i, 0));
			if(foundBankId && *foundBankId == bankId)
				break;
			foundBankId = NULL;
		}
	}

	if(!foundBankId && !justAppend)
		return;

	if(!foundBankId)
		foundBankId = new unsigned int(bankId);

	DSListCtrl::DSListCtrlRow *fillInDataBankListCtrlRow = new DSListCtrl::DSListCtrlRow(foundBankId);
	fillInDataBankListCtrlRow->Add(bankEntity->Get(_N("bank"))->ToString());
	fillInDataBankListCtrlRow->Add(bankEntity->Get(_N("bankCode"))->ToString());
	fillInDataBankListCtrlRow->Add(bankEntity->Get(_N("bic"))->ToString());
	fillInDataBankListCtrl->Set(fillInDataBankListCtrlRow, true);
}

void PreferencesModulePanel::EditBank(unsigned int *id) {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to edit bank."), this))
		return;

	if(!id) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select an entry."),
			_("Edit bank")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	BankController bankController(rit);
	bankController.Get(DSCasts::ToString(*id));
	if(bankController.Run()) {
		rit->Log(false, _N("PreferencesModulePanel"), _("The bank is in edit mode now."), id ? wxString(_("Bank id: ") + DSCasts::ToString(*id)) : wxString(_("Unknown")));
		BankEntity bankEntity(rit);
		bankController.Get(&bankEntity);
		bankController.Reset();

		fillInDataBankTextCtrl->SetValue(bankEntity.Get(_N("bank"))->ToString());
		fillInDataBankCodeTextCtrl->SetValue(bankEntity.Get(_N("bankCode"))->ToString());
		fillInDataBankBicTextCtrl->SetValue(bankEntity.Get(_N("bic"))->ToString());

		EnableBankFields(true, id);
	} else {
		rit->Log(true, _N("PreferencesModulePanel"), _("The bank is not been edited."), id ? wxString(_("Bank id: ") + DSCasts::ToString(*id)) : wxString(_("Unknown")));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not find selected entry in database."),
			_("Edit bank")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		fillInDataBankTextCtrl->Clear();
		fillInDataBankCodeTextCtrl->Clear();
		fillInDataBankBicTextCtrl->Clear();

		EnableBankFields(false, NULL);
	}
}

void PreferencesModulePanel::EnableBankFields(const bool edit, unsigned int *id) {
	fillInDataBankListCtrl->Enable(!edit);
	fillInDataBankImportButton->Enable(!edit);
	fillInDataBankRemoveButton->Enable(!edit);
	fillInDataBankFindButton->SetLabel(edit ? _("Abort") : _("Find"));

	if(!edit) {
		fillInDataBankTextCtrl->Clear();
		fillInDataBankCodeTextCtrl->Clear();
		fillInDataBankBicTextCtrl->Clear();
	}

	this->actualBankId = id;
}

void PreferencesModulePanel::TextTemplateSelect(const int selection) {
	const unsigned int *templateId = static_cast<const unsigned int *>(textTemplateListBox->GetClientData(selection));
	if(templateId) {
		TemplateController templateController(rit);
		templateController.Get(DSCasts::ToString(*templateId));
		if(templateController.Run()) {
			TemplateEntity templateEntity(rit);
			templateController.Get(&templateEntity);
			templateController.Reset();
			textTemplateTextCtrl->SetValue(templateEntity.Get(_N("content"))->ToString());
		}
	} else {
		textTemplateTextCtrl->Clear();
	}
}

void PreferencesModulePanel::TextTemplateRemove(const int selection) {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to remove text templates."), this))
		return;

	const unsigned int *templateId = static_cast<const unsigned int *>(textTemplateListBox->GetClientData(selection));
	if(templateId) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you really want to remove this entry?"),
			_("Remove bank"),
			_("Remove"),
			_("Cancel")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES) {
			TemplateEntity templateEntity(rit);
			templateEntity.SetValue(_N("templateId"), *templateId);
			TemplateController templateController(rit);
			if(templateController.Delete(&templateEntity)) {
				rit->Log(false, _N("PreferencesModulePanel"), _("Text template removed."), templateEntity.Get(_N("name"))->ToString());
				TextTemplateRefresh();
			} else {
				rit->Log(true, _N("PreferencesModulePanel"), _("Text template could not been removed."), templateEntity.Get(_N("name"))->ToString());
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not remove entry. Database error."),
					_("Database error")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			}
		}
		yesNoDialog->Destroy();
	}
}

void PreferencesModulePanel::TextTemplateEdit(const int selection) {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to edit text templates."), this))
		return;

	const unsigned int *templateId = static_cast<const unsigned int *>(textTemplateListBox->GetClientData(selection));
	if(templateId) {
		TemplateEditDialog *templateEditDialog = new TemplateEditDialog(this, -1, rit, *templateId);
		templateEditDialog->Center();
		if(templateEditDialog->ShowModal() == wxID_OK)
			TextTemplateRefresh();
		templateEditDialog->Destroy();
	}
}

void PreferencesModulePanel::TextTemplateAdd() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to add text templates."), this))
		return;

	TemplateEditDialog *templateEditDialog = new TemplateEditDialog(this, -1, rit);
	templateEditDialog->Center();
	if(templateEditDialog->ShowModal() == wxID_OK)
		TextTemplateRefresh();
	templateEditDialog->Destroy();
}

void PreferencesModulePanel::TextTemplateRefresh() {
	const unsigned int *templateId = static_cast<const unsigned int *>(textTemplateListBox->GetClientData(textTemplateListBox->GetSelection()));

	TextTemplateCleanUp();

	unsigned int i = 0;
	TemplateEntity templateEntity(rit);
	TemplateController templateController(rit);
	templateController.Get();
	while(templateController.Run()) {
		templateController.Get(&templateEntity);
		textTemplateListBox->Append(
			templateEntity.Get(_N("name"))->ToString(),
			new int(templateEntity.Get(_N("templateId"))->ToUnsignedInt())
		);

		if(templateId && *templateId == templateEntity.Get(_N("templateId"))->ToUnsignedInt()) {
			textTemplateListBox->SetSelection(i);
			TextTemplateSelect(i);
		}

		i++;
	}
}

void PreferencesModulePanel::TextTemplateCleanUp() {
	unsigned int *templateId = NULL;
	const unsigned int count = textTemplateListBox->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		templateId = static_cast<unsigned int *>(textTemplateListBox->GetClientData(i));
		if(templateId)
			delete templateId;
	}

	textTemplateListBox->Clear();
}

void PreferencesModulePanel::StockAdd() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to add stocks."), this))
		return;

	StockDialog *stockDialog = new StockDialog(this, -1, rit, NULL);
	stockDialog->Center();
	if(stockDialog->ShowModal() == wxID_OK)
		AddOrChangeToStocksListCtrl(stockDialog->GetStock());
	stockDialog->Destroy();
}

void PreferencesModulePanel::StockEdit() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to edit stocks."), this))
		return;

	StockEntity *stockEntity = reinterpret_cast<StockEntity *>(stockListCtrl->GetSingleSelectionPointer());

	if(!stockEntity) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please select the stock you want to edit."),
			_("Edit")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	} else {
		StockDialog *stockDialog = new StockDialog(this, -1, rit, stockEntity);
		stockDialog->Center();
		if(stockDialog->ShowModal() == wxID_OK)
			AddOrChangeToStocksListCtrl(stockEntity);
		stockDialog->Destroy();
	}
}

void PreferencesModulePanel::StockRemove() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to remove stocks."), this))
		return;

	StockEntity *stockEntity = reinterpret_cast<StockEntity *>(stockListCtrl->GetSingleSelectionPointer());

	if(!stockEntity) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please select the stock you want to remove."),
			_("Remove")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	} else {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you realy want to remove the selected stock?"),
			_("Remove"),
			_("Remove"),
			_("Cancel")
		);
		if(yesNoDialog->ShowModal() == wxID_YES) {
			StockController stockController(rit);
			if(!stockController.Delete(stockEntity)) {
				wxMessageDialog *messageDialog = new wxMessageDialog(
					this,
					_("The stock could not be deleted."),
					_("Edit")
				);
				messageDialog->Center();
				messageDialog->ShowModal();
				messageDialog->Destroy();
				rit->Log(true, _N("PreferencesModulePanel"), _("The stock ") + stockEntity->Get(_N("name"))->ToString() + _(" could not been deleted."));
			} else {
				rit->Log(false, _N("PreferencesModulePanel"), _("The stock ") + stockEntity->Get(_N("name"))->ToString() + _(" is deleted."));
				stockListCtrl->DeleteItem(stockListCtrl->GetPosition(reinterpret_cast<wxUIntPtr *>(stockEntity)));
				delete stockEntity;
				stockEntity = NULL;
			}
		}
		yesNoDialog->Destroy();
	}
}

void PreferencesModulePanel::AddOrChangeToStocksListCtrl(StockEntity *stockEntity) {
	DSListCtrl::DSListCtrlRow *stockListCtrlRow = new DSListCtrl::DSListCtrlRow(stockEntity);
	stockListCtrlRow->Add(stockEntity->Get(_N("name"))->ToString());
	stockListCtrlRow->Add(
		AddressType(
			stockEntity->Get(_N("address"))->ToString(),
			stockEntity->Get(_N("number"))->ToString(),
			stockEntity->Get(_N("zipcode"))->ToString(),
			stockEntity->Get(_N("city"))->ToString(),
			wxEmptyString,
			wxEmptyString
		).GetConcatedAddress(_N(", "), false, false)
	);
	stockListCtrlRow->Add(stockEntity->Get(_N("active"))->ToBool() ? _("yes") : _("no"));
	stockListCtrl->Set(stockListCtrlRow, true);
}

void PreferencesModulePanel::ReportGeneratorStart() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to start Report Generator."), this))
		return;

	ReportGeneratorFrame *reportGeneratorFrame = new ReportGeneratorFrame(this, -1, rit);
	reportGeneratorFrame->Center();
	
	reportGeneratorFrame->Show();
}

void PreferencesModulePanel::AdvancedPreferencesEdit(KeyValueType *keyValue) {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to edit keys and values."), this))
		return;

	if(!keyValue) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please select the key you want to edit."),
			_("Edit")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();

		return;
	}

	TextInputDialog *textInputDialog = new TextInputDialog(
		this,
		-1,
		rit,
		_("Change value"),
		_("Change value"),
		true,
		keyValue->GetValue()
	);
	textInputDialog->Center();
	if(textInputDialog->ShowModal() == wxID_OK) {
		KeyValueDatabaseLibrary advancedPreferences(rit, _N("advanced_preferences"));

		if(!advancedPreferences.Set(keyValue->GetKey(), textInputDialog->GetValue())) {
			wxMessageDialog *messageDialog = new wxMessageDialog(
				this,
				_("Could not save value to database. Database error."),
				_("Edit")
			);
			messageDialog->Center();
			messageDialog->ShowModal();
			messageDialog->Destroy();

			rit->Log(true, _N("PreferencesModulePanel"), _("Could not save value to database. Database error."), _N("Key: ") + keyValue->GetKey() + _N(", old value: ") + keyValue->GetValue() + _N(", new value: ") + textInputDialog->GetValue());
		} else {
			rit->Log(false, _N("PreferencesModulePanel"), _("Saved value to database."), _N("Key: ") + keyValue->GetKey() + _N(", old value: ") + keyValue->GetValue() + _N(", new value: ") + textInputDialog->GetValue());
		}

		AdvancedPreferencesLoad();
	}
	textInputDialog->Destroy();
}

void PreferencesModulePanel::AdvancedPreferencesLoad() {
	KeyValueType *selectedKeyValue = static_cast<KeyValueType *>(advancedPreferencesListCtrl->GetSingleSelectionPointer());
	wxString selectedKey = selectedKeyValue ? selectedKeyValue->GetKey() : wxString();

	advancedPreferencesListCtrl->CleanUp();

	KeyValueDatabaseLibrary advancedPreferences(rit, _N("advanced_preferences"));

	unsigned int selection = 0;
	KeyValueType *keyValue = NULL;
	wxString key, value;
	DSListCtrl::DSListCtrlRow *advancedPreferencesListCtrlRow = NULL;
	KeysValuesType keysValues = advancedPreferences.Get();
	const unsigned int count = keysValues.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		keyValue = keysValues.Get(i);
		key      = keyValue->GetKey();
		value    = keyValue->GetValue();

		advancedPreferencesListCtrlRow = new DSListCtrl::DSListCtrlRow(new KeyValueType(key, value));
		advancedPreferencesListCtrlRow->Add(rit->Translate(key));
		advancedPreferencesListCtrlRow->Add(value);
		advancedPreferencesListCtrl->Append(advancedPreferencesListCtrlRow, true);

		if(key == selectedKey)
			selection = i;
	}

	advancedPreferencesListCtrl->SetSelection(selection);
}

void PreferencesModulePanel::ReportPathBrowse(wxTextCtrl *pathTextCtrl) {
	if(pathTextCtrl) {
		wxDirDialog *reportDirDialog = new wxDirDialog(
			this,
			_("Choose the report directory"),
			pathTextCtrl->GetValue()
		);
		reportDirDialog->Center();
		if(reportDirDialog->ShowModal() == wxID_OK)
			pathTextCtrl->SetValue(reportDirDialog->GetPath());
		reportDirDialog->Destroy();
	}
}

void PreferencesModulePanel::JreHomeBrowse(wxTextCtrl *jreHomeTextCtrl) {
	if(jreHomeTextCtrl) {
		wxDirDialog *jreHomeDirDialog = new wxDirDialog(
			this,
			_("Choose the JRE directory"),
			jreHomeTextCtrl->GetValue()
		);
		jreHomeDirDialog->Center();
		if(jreHomeDirDialog->ShowModal() == wxID_OK)
			jreHomeTextCtrl->SetValue(jreHomeDirDialog->GetPath());
		jreHomeDirDialog->Destroy();
	}
}
