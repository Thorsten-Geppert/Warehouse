#include "ClientEditFrame.h"
#include "../lib/ClientController.h"
#include "../lib/ListEntryController.h"
#include "../lib/AddressEntity.h"
#include "../lib/ContactPersonEntity.h"
#include "../lib/AddressType.h"
#include "../lib/NameType.h"
#include "../lib/PhoneType.h"
#include "../lib/BankConnectionEntity.h"
#include "../lib/BankConnectionEntities.h"
#include "../lib/ReminderType.h"
#include "../lib/EnterprisePreferencesController.h"
#include "../lib/EnterprisePreferencesEntity.h"
#include "../lib/KeyValueDatabaseLibrary.h"
#include "../lib/JasperReportsLibrary.h"
#include "PrintPreviewDialog.h"
#include "ContactPersonEditDialog.h"
#include "AddressEditDialog.h"
#include "YesNoDialog.h"
#include "BankConnectionEditDialog.h"
#include "RemindersDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ClientEditFrame, wxFrame)
	EVT_CLOSE(ClientEditFrame::OnClose)
	EVT_COMBOBOX(XRCID("formOfOrganizationComboBox"), ClientEditFrame::ChangeFormOfOrganizationEvent)
	EVT_MENU(CLOSE_EVENT, ClientEditFrame::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, ClientEditFrame::SaveKeyEvent)
	EVT_MENU(EDITED_INFORMATIONS_EVENT, ClientEditFrame::ShowEditedInformationsKeyEvent)
	EVT_MENU(HIDDEN_INFORMATIONS_EVENT, ClientEditFrame::ShowHiddenInformationsKeyEvent)
	EVT_MENU(PRINT_EVENT, ClientEditFrame::PrintKeyEvent)
	EVT_MENU(PRINT_PREVIEW_EVENT, ClientEditFrame::PrintPreviewKeyEvent)
	EVT_MENU(ORDERS_EVENT, ClientEditFrame::OrdersKeyEvent)
	EVT_MENU(REMINDERS_EVENT, ClientEditFrame::RemindersKeyEvent)
	EVT_BUTTON(XRCID("addAddressButton"), ClientEditFrame::AddressAddEvent)
	EVT_BUTTON(XRCID("removeAddressButton"), ClientEditFrame::AddressRemoveEvent)
	EVT_BUTTON(XRCID("changeAddressButton"), ClientEditFrame::AddressChangeEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("addressesListCtrl"), ClientEditFrame::AddressEditEvent)
	EVT_BUTTON(XRCID("addContactPersonButton"), ClientEditFrame::ContactPersonAddEvent)
	EVT_BUTTON(XRCID("removeContactPersonButton"), ClientEditFrame::ContactPersonRemoveEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("contactPersonsListCtrl"), ClientEditFrame::ContactPersonEditEvent)
	EVT_MENU(ADDRESS_ADD_EVENT, ClientEditFrame::AddressAddKeyEvent)
	EVT_MENU(ADDRESS_REMOVE_EVENT, ClientEditFrame::AddressRemoveKeyEvent)
	EVT_MENU(ADDRESS_CHANGE_EVENT, ClientEditFrame::AddressChangeKeyEvent)
	EVT_MENU(CONTACT_PERSON_ADD_EVENT, ClientEditFrame::ContactPersonAddKeyEvent)
	EVT_MENU(CONTACT_PERSON_REMOVE_EVENT, ClientEditFrame::ContactPersonRemoveKeyEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("bankConnectionsListCtrl"), ClientEditFrame::BankConnectionEditEvent)
	EVT_BUTTON(XRCID("addBankConnectionButton"), ClientEditFrame::BankConnectionAddEvent)
	EVT_BUTTON(XRCID("removeBankConnectionButton"), ClientEditFrame::BankConnectionRemoveEvent)
	EVT_MENU(BANK_CONNECTION_ADD_EVENT, ClientEditFrame::BankConnectionAddKeyEvent)
	EVT_MENU(BANK_CONNECTION_REMOVE_EVENT, ClientEditFrame::BankConnectionRemoveKeyEvent)
END_EVENT_TABLE()

ClientEditFrame::ClientEditFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	ClientEntity *clientEntity,
	GuiCallbackInterface *callbackInterface
) : bind(
	clientEntity
), changed(
	true
), clientOrdersDialog(
	NULL
) {
	SetId(id);
	this->rit                      = rit;
	this->callbackInterface        = callbackInterface;
	this->clientId                 = clientEntity ? clientEntity->Get(_N("clientId"))->ToInt() : 0;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ClientEditFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("ClientEditFrame"));
	mainPanel                            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton                    = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	closeToolbarButton                   = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	editedInformationsToolbarButton      = XRCCTRL(*this, "editedInformationsToolbarButton", ToolbarButton);
	hiddenInformationsToolbarButton      = XRCCTRL(*this, "hiddenInformationsToolbarButton", ToolbarButton);
	printToolbarButton                   = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	printPreviewToolbarButton            = XRCCTRL(*this, "printPreviewToolbarButton", ToolbarButton);
	ordersToolbarButton                  = XRCCTRL(*this, "ordersToolbarButton", ToolbarButton);
	remindersToolbarButton               = XRCCTRL(*this, "remindersToolbarButton", ToolbarButton);
	clientIdTextCtrl                     = XRCCTRL(*this, "clientIdTextCtrl", DynSoft::DSTextCtrl);
	formOfOrganizationComboBox           = XRCCTRL(*this, "formOfOrganizationComboBox", DynSoft::DSComboBox);
	circularLetterTextCtrl               = XRCCTRL(*this, "circularLetterTextCtrl", DynSoft::DSTextCtrl);
	salutationComboBox                   = XRCCTRL(*this, "salutationComboBox", DynSoft::DSComboBox);
	titleComboBox                        = XRCCTRL(*this, "titleComboBox", DynSoft::DSComboBox);
	firstnameStaticText                  = XRCCTRL(*this, "firstnameStaticText", wxStaticText);
	firstnameTextCtrl                    = XRCCTRL(*this, "firstnameTextCtrl", DynSoft::DSTextCtrl);
	name1TextCtrl                        = XRCCTRL(*this, "name1TextCtrl", DynSoft::DSTextCtrl);
	name2TextCtrl                        = XRCCTRL(*this, "name2TextCtrl", DynSoft::DSTextCtrl);
	shortNameTextCtrl                    = XRCCTRL(*this, "shortNameTextCtrl", DynSoft::DSTextCtrl);
	addressTextCtrl                      = XRCCTRL(*this, "addressTextCtrl", DynSoft::DSTextCtrl);
	numberTextCtrl                       = XRCCTRL(*this, "numberTextCtrl", DynSoft::DSTextCtrl);
	zipcodeTextCtrl                      = XRCCTRL(*this, "zipcodeTextCtrl", DynSoft::DSTextCtrl);
	cityTextCtrl                         = XRCCTRL(*this, "cityTextCtrl", DynSoft::DSTextCtrl);
	federalStateComboBox                 = XRCCTRL(*this, "federalStateComboBox", DynSoft::DSComboBox);
	landComboBox                         = XRCCTRL(*this, "landComboBox", DynSoft::DSComboBox);
	name3TextCtrl                        = XRCCTRL(*this, "name3TextCtrl", DynSoft::DSTextCtrl);
	name4TextCtrl                        = XRCCTRL(*this, "name4TextCtrl", DynSoft::DSTextCtrl);
	telephone1CountryTextCtrl            = XRCCTRL(*this, "telephone1CountryTextCtrl", DynSoft::DSTextCtrl);
	telephone1PrefixTextCtrl             = XRCCTRL(*this, "telephone1PrefixTextCtrl", DynSoft::DSTextCtrl);
	telephone1NumberTextCtrl             = XRCCTRL(*this, "telephone1NumberTextCtrl", DynSoft::DSTextCtrl);
	telephone2CountryTextCtrl            = XRCCTRL(*this, "telephone2CountryTextCtrl", DynSoft::DSTextCtrl);
	telephone2PrefixTextCtrl             = XRCCTRL(*this, "telephone2PrefixTextCtrl", DynSoft::DSTextCtrl);
	telephone2NumberTextCtrl             = XRCCTRL(*this, "telephone2NumberTextCtrl", DynSoft::DSTextCtrl);
	telephone3CountryTextCtrl            = XRCCTRL(*this, "telephone3CountryTextCtrl", DynSoft::DSTextCtrl);
	telephone3PrefixTextCtrl             = XRCCTRL(*this, "telephone3PrefixTextCtrl", DynSoft::DSTextCtrl);
	telephone3NumberTextCtrl             = XRCCTRL(*this, "telephone3NumberTextCtrl", DynSoft::DSTextCtrl);
	fax1CountryTextCtrl                  = XRCCTRL(*this, "fax1CountryTextCtrl", DynSoft::DSTextCtrl);
	fax1PrefixTextCtrl                   = XRCCTRL(*this, "fax1PrefixTextCtrl", DynSoft::DSTextCtrl);
	fax1NumberTextCtrl                   = XRCCTRL(*this, "fax1NumberTextCtrl", DynSoft::DSTextCtrl);
	fax2CountryTextCtrl                  = XRCCTRL(*this, "fax2CountryTextCtrl", DynSoft::DSTextCtrl);
	fax2PrefixTextCtrl                   = XRCCTRL(*this, "fax2PrefixTextCtrl", DynSoft::DSTextCtrl);
	fax2NumberTextCtrl                   = XRCCTRL(*this, "fax2NumberTextCtrl", DynSoft::DSTextCtrl);
	fax3CountryTextCtrl                  = XRCCTRL(*this, "fax3CountryTextCtrl", DynSoft::DSTextCtrl);
	fax3PrefixTextCtrl                   = XRCCTRL(*this, "fax3PrefixTextCtrl", DynSoft::DSTextCtrl);
	fax3NumberTextCtrl                   = XRCCTRL(*this, "fax3NumberTextCtrl", DynSoft::DSTextCtrl);
	mobile1CountryTextCtrl               = XRCCTRL(*this, "mobile1CountryTextCtrl", DynSoft::DSTextCtrl);
	mobile1PrefixTextCtrl                = XRCCTRL(*this, "mobile1PrefixTextCtrl", DynSoft::DSTextCtrl);
	mobile1NumberTextCtrl                = XRCCTRL(*this, "mobile1NumberTextCtrl", DynSoft::DSTextCtrl);
	mobile2CountryTextCtrl               = XRCCTRL(*this, "mobile2CountryTextCtrl", DynSoft::DSTextCtrl);
	mobile2PrefixTextCtrl                = XRCCTRL(*this, "mobile2PrefixTextCtrl", DynSoft::DSTextCtrl);
	mobile2NumberTextCtrl                = XRCCTRL(*this, "mobile2NumberTextCtrl", DynSoft::DSTextCtrl);
	email1TextCtrl                       = XRCCTRL(*this, "email1TextCtrl", DynSoft::DSTextCtrl);
	email2TextCtrl                       = XRCCTRL(*this, "email2TextCtrl", DynSoft::DSTextCtrl);
	internet1TextCtrl                    = XRCCTRL(*this, "internet1TextCtrl", DynSoft::DSTextCtrl);
	internet2TextCtrl                    = XRCCTRL(*this, "internet2TextCtrl", DynSoft::DSTextCtrl);
	informationsTextCtrl                 = XRCCTRL(*this, "informationsTextCtrl", DynSoft::DSTextCtrl);
	addressesListCtrl                    = XRCCTRL(*this, "addressesListCtrl", DynSoft::DSListCtrl);
	contactPersonsListCtrl               = XRCCTRL(*this, "contactPersonsListCtrl", DynSoft::DSListCtrl);
	taxNumberTextCtrl                    = XRCCTRL(*this, "taxNumberTextCtrl", DynSoft::DSTextCtrl);
	salesTaxIdentificationNumberTextCtrl = XRCCTRL(*this, "salesTaxIdentificationNumberTextCtrl", DynSoft::DSTextCtrl);
	discountTextCtrl                     = XRCCTRL(*this, "discountTextCtrl", DynSoft::DSTextCtrl);
	liableToTaxCheckBox                  = XRCCTRL(*this, "liableToTaxCheckBox", DynSoft::DSCheckBox);
	bankConnectionsListCtrl              = XRCCTRL(*this, "bankConnectionsListCtrl", DynSoft::DSListCtrl);

	ownClientIdStaticText                = XRCCTRL(*this, "ownClientIdStaticText", wxStaticText);
	ownClientIdTextCtrl                  = XRCCTRL(*this, "ownClientIdTextCtrl", DynSoft::DSTextCtrl);
	kindComboBox                         = XRCCTRL(*this, "kindComboBox", DynSoft::DSComboBox);
	groupComboBox                        = XRCCTRL(*this, "groupComboBox", DynSoft::DSComboBox);
	sectorComboBox                       = XRCCTRL(*this, "sectorComboBox", DynSoft::DSComboBox);
	birthdayDayComboBox                  = XRCCTRL(*this, "birthdayDayComboBox", DynSoft::DSComboBox);
	birthdayMonthComboBox                = XRCCTRL(*this, "birthdayMonthComboBox", DynSoft::DSComboBox);
	birthdayYearComboBox                 = XRCCTRL(*this, "birthdayYearComboBox", DynSoft::DSComboBox);
	activeCheckBox                       = XRCCTRL(*this, "activeCheckBox", DynSoft::DSCheckBox);
	conditionComboBox                    = XRCCTRL(*this, "conditionComboBox", DynSoft::DSComboBox);
	monitionableCheckBox                 = XRCCTRL(*this, "monitionableCheckBox", DynSoft::DSCheckBox);

	KeyValueDatabaseLibrary keyValueLibrary(rit, _N("advanced_preferences"));
	const wxString mediaDefault(keyValueLibrary.Get(_N("clients.media.default_description")).ToString());
	wxPanel *tmpMediaPanel               = XRCCTRL(*this, "mediaPanel", wxPanel);
	mediaBoxSizer                        = static_cast<wxBoxSizer *>(tmpMediaPanel->GetSizer());
	mediaPanel                           = new MediaPanel(tmpMediaPanel, -1, rit, _N("linked_files_clients"), clientEntity ? clientEntity->GetLinkedFileEntities() : NULL, &changed, mediaDefault);
	mediaBoxSizer->Add(mediaPanel, 1, wxEXPAND | wxALL, 5);

	wxPanel *tmpMoreDataPanel           = XRCCTRL(*this, "moreDataPanel", wxPanel);
	moreDataBoxSizer                    = static_cast<wxBoxSizer *>(tmpMoreDataPanel->GetSizer());
	moreDataPanel                       = new MoreDataPanel(tmpMoreDataPanel, -1, rit, clientEntity->GetKeysValuesType(), &changed);
	moreDataBoxSizer->Add(moreDataPanel, 1, wxEXPAND | wxALL, 5);

	addressesListCtrl->InsertColumn(0, _("Description"), wxLIST_FORMAT_LEFT, 150);
	addressesListCtrl->InsertColumn(1, _("Name"), wxLIST_FORMAT_LEFT, 150);
	addressesListCtrl->InsertColumn(2, _("Address"), wxLIST_FORMAT_LEFT, 150);
	addressesListCtrl->InsertColumn(3, _("Zipcode"), wxLIST_FORMAT_LEFT, 80);
	addressesListCtrl->InsertColumn(4, _("City"), wxLIST_FORMAT_LEFT, 120);
	addressesListCtrl->InsertColumn(5, _("Federal state"), wxLIST_FORMAT_LEFT, 120);
	addressesListCtrl->InsertColumn(6, _("Land"), wxLIST_FORMAT_LEFT, 120);

	contactPersonsListCtrl->InsertColumn(0, _("Department"), wxLIST_FORMAT_LEFT, 100);
	contactPersonsListCtrl->InsertColumn(1, _("Function"), wxLIST_FORMAT_LEFT, 100);
	contactPersonsListCtrl->InsertColumn(2, _("Name"), wxLIST_FORMAT_LEFT, 150);
	contactPersonsListCtrl->InsertColumn(3, _("Email"), wxLIST_FORMAT_LEFT, 110);
	contactPersonsListCtrl->InsertColumn(4, _("Telephone"), wxLIST_FORMAT_LEFT, 120);
	contactPersonsListCtrl->InsertColumn(5, _("Mobile"), wxLIST_FORMAT_LEFT, 120);
	contactPersonsListCtrl->InsertColumn(6, _("Fax"), wxLIST_FORMAT_LEFT, 120);

	bankConnectionsListCtrl->InsertColumn(0, _("Active"), wxLIST_FORMAT_LEFT, 50);
	bankConnectionsListCtrl->InsertColumn(1, _("Description"), wxLIST_FORMAT_LEFT, 100);
	bankConnectionsListCtrl->InsertColumn(2, _("Bank"), wxLIST_FORMAT_LEFT, 100);
	bankConnectionsListCtrl->InsertColumn(3, _("BIC"), wxLIST_FORMAT_LEFT, 110);
	bankConnectionsListCtrl->InsertColumn(4, _("IBAN"), wxLIST_FORMAT_LEFT, 110);
	bankConnectionsListCtrl->InsertColumn(5, _("Owner"), wxLIST_FORMAT_LEFT, 120);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, ClientEditFrame::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, ClientEditFrame::SaveEvent);
	CONNECT_TOOLBARBUTTON(editedInformationsToolbarButton, ClientEditFrame::ShowEditedInformationsEvent);
	CONNECT_TOOLBARBUTTON(hiddenInformationsToolbarButton, ClientEditFrame::ShowHiddenInformationsEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, ClientEditFrame::PrintEvent);
	CONNECT_TOOLBARBUTTON(printPreviewToolbarButton, ClientEditFrame::PrintPreviewEvent);
	CONNECT_TOOLBARBUTTON(ordersToolbarButton, ClientEditFrame::OrdersEvent);
	CONNECT_TOOLBARBUTTON(remindersToolbarButton, ClientEditFrame::RemindersEvent);

	wxAcceleratorEntry acceleratorEntries[16];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,                 WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,                   (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,                   (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,                   (int) 'i',  EDITED_INFORMATIONS_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,                   (int) 'h',  HIDDEN_INFORMATIONS_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL,                   (int) 'p',  PRINT_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL,                   (int) 'o',  ORDERS_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL | wxACCEL_ALT,     (int) 'p',  PRINT_PREVIEW_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL | wxACCEL_ALT,     (int) 'a',  ADDRESS_ADD_EVENT);
	acceleratorEntries[9].Set(wxACCEL_CTRL | wxACCEL_ALT,     (int) 'r',  ADDRESS_REMOVE_EVENT);
	acceleratorEntries[10].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'c',  ADDRESS_CHANGE_EVENT);
	acceleratorEntries[11].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'g',  CONTACT_PERSON_ADD_EVENT);
	acceleratorEntries[12].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'h',  CONTACT_PERSON_REMOVE_EVENT);
	acceleratorEntries[13].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'b',  BANK_CONNECTION_ADD_EVENT);
	acceleratorEntries[14].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'x',  BANK_CONNECTION_REMOVE_EVENT);
	acceleratorEntries[15].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'w',  REMINDERS_EVENT);
	wxAcceleratorTable acceleratorTable(16, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Binding
	bind.Bind(_N("clientId"), clientIdTextCtrl);
	bind.Bind(_N("formOfOrganization"), formOfOrganizationComboBox, DSBind::SELECTION);
	bind.Bind(_N("circularLetter"), circularLetterTextCtrl);
	bind.Bind(_N("salutation"), salutationComboBox, DSBind::VALUE);
	bind.Bind(_N("title"), titleComboBox, DSBind::VALUE);
	bind.Bind(_N("firstname"), firstnameTextCtrl);
	bind.Bind(_N("name1"), name1TextCtrl);
	bind.Bind(_N("name2"), name2TextCtrl);
	bind.Bind(_N("name3"), name3TextCtrl);
	bind.Bind(_N("name4"), name4TextCtrl);
	bind.Bind(_N("shortName"), shortNameTextCtrl);
	bind.Bind(_N("address"), addressTextCtrl);
	bind.Bind(_N("number"), numberTextCtrl);
	bind.Bind(_N("zipcode"), zipcodeTextCtrl);
	bind.Bind(_N("city"), cityTextCtrl);
	bind.Bind(_N("federalState"), federalStateComboBox, DSBind::VALUE);
	bind.Bind(_N("land"), landComboBox, DSBind::VALUE);
	bind.Bind(_N("telephone1Country"), telephone1CountryTextCtrl);
	bind.Bind(_N("telephone1Prefix"), telephone1PrefixTextCtrl);
	bind.Bind(_N("telephone1Number"), telephone1NumberTextCtrl);
	bind.Bind(_N("telephone2Country"), telephone2CountryTextCtrl);
	bind.Bind(_N("telephone2Prefix"), telephone2PrefixTextCtrl);
	bind.Bind(_N("telephone2Number"), telephone2NumberTextCtrl);
	bind.Bind(_N("telephone3Country"), telephone3CountryTextCtrl);
	bind.Bind(_N("telephone3Prefix"), telephone3PrefixTextCtrl);
	bind.Bind(_N("telephone3Number"), telephone3NumberTextCtrl);
	bind.Bind(_N("fax1Country"), fax1CountryTextCtrl);
	bind.Bind(_N("fax1Prefix"), fax1PrefixTextCtrl);
	bind.Bind(_N("fax1Number"), fax1NumberTextCtrl);
	bind.Bind(_N("fax2Country"), fax2CountryTextCtrl);
	bind.Bind(_N("fax2Prefix"), fax2PrefixTextCtrl);
	bind.Bind(_N("fax2Number"), fax2NumberTextCtrl);
	bind.Bind(_N("fax3Country"), fax3CountryTextCtrl);
	bind.Bind(_N("fax3Prefix"), fax3PrefixTextCtrl);
	bind.Bind(_N("fax3Number"), fax3NumberTextCtrl);
	bind.Bind(_N("mobile1Country"), mobile1CountryTextCtrl);
	bind.Bind(_N("mobile1Prefix"), mobile1PrefixTextCtrl);
	bind.Bind(_N("mobile1Number"), mobile1NumberTextCtrl);
	bind.Bind(_N("mobile2Country"), mobile2CountryTextCtrl);
	bind.Bind(_N("mobile2Prefix"), mobile2PrefixTextCtrl);
	bind.Bind(_N("mobile2Number"), mobile2NumberTextCtrl);
	bind.Bind(_N("email1"), email1TextCtrl);
	bind.Bind(_N("email2"), email2TextCtrl);
	bind.Bind(_N("internet1"), internet1TextCtrl);
	bind.Bind(_N("internet2"), internet2TextCtrl);
	bind.Bind(_N("informations"), informationsTextCtrl);
	bind.Bind(_N("taxNumber"), taxNumberTextCtrl);
	bind.Bind(_N("salesTaxIdentificationNumber"), salesTaxIdentificationNumberTextCtrl);
	bind.Bind(_N("discount"), discountTextCtrl);
	bind.Bind(_N("liableToTax"), liableToTaxCheckBox);

	bind.Bind(_N("ownClientId"), ownClientIdTextCtrl);
	bind.Bind(_N("kind"), kindComboBox, DSBind::VALUE);
	bind.Bind(_N("group"), groupComboBox, DSBind::VALUE);
	bind.Bind(_N("sector"), sectorComboBox, DSBind::VALUE);
	bind.Bind(_N("birthdayDay"), birthdayDayComboBox, DSBind::VALUE);
	bind.Bind(_N("birthdayMonth"), birthdayMonthComboBox, DSBind::VALUE);
	bind.Bind(_N("birthdayYear"), birthdayYearComboBox, DSBind::VALUE);
	bind.Bind(_N("active"), activeCheckBox);
	bind.Bind(_N("condition"), conditionComboBox, DSBind::VALUE);
	bind.Bind(_N("monitionable"), monitionableCheckBox);

	// Filling
	formOfOrganizationComboBox->Append(_("natural"));
	formOfOrganizationComboBox->Append(_("legal"));

	kindComboBox->Append(_N(""));
	kindComboBox->Append(_("Client"));
	kindComboBox->Append(_("Distributor"));
	kindComboBox->Append(_("Client/Distributor"));

	birthdayDayComboBox->Append(wxEmptyString);
	birthdayMonthComboBox->Append(wxEmptyString);
	birthdayYearComboBox->Append(wxEmptyString);

	wxString birth;
	for(unsigned int i = 1; i <= 31; i++) {
		birth = DSCasts::ToString(i);
		if(i <= 12)
			birthdayMonthComboBox->Append(birth);
		birthdayDayComboBox->Append(birth);
	}

	const int year = wxDateTime::Now().GetYear();
	for(unsigned int i = year; i >= 1800; i--)
		birthdayYearComboBox->Append(DSCasts::ToString(i));


	ListEntryController::FillComboBox(rit, titleComboBox,        _N("titles"),       _N("title"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, salutationComboBox,   _N("salutations"),  _N("salutation"),   ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, landComboBox,         _N("lands"),        _N("land"),         ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, federalStateComboBox, _N("federalstate"), _N("federalstate"), ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, groupComboBox,        _N("groups"),       _N("group"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, sectorComboBox,       _N("sectors"),      _N("sector"),       ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, conditionComboBox,    _N("conditions"),   _N("condition"),    ListEntryController::FILL_NEW, true);

	if(clientEntity) {
		rit->Block(_N("clients"), clientId, this);
		firstname = clientEntity->Get(_N("firstname"))->ToString();
		bind.Push();
		discountTextCtrl->SetValue(clientEntity->Get(_N("discount"))->ToCurrency().ToString());
		if(clientId == 0) {
			clientIdTextCtrl->SetValue(_("<automatic>"));
			activeCheckBox->SetValue(true);
			monitionableCheckBox->SetValue(true);
		} else {
			ownClientIdTextCtrl->Disable();
			kindComboBox->Disable();
		}

		AddressEntities *addressEntities = clientEntity->GetAddresses();
		unsigned int count = addressEntities->GetCount();
		for(unsigned int i = 0; i < count; i++)
			AddOrChangeToAddressesListCtrl(addressEntities->Get(i));

		ContactPersonEntities *contactPersonEntities = clientEntity->GetContactPersons();
		count = contactPersonEntities->GetCount();
		for(unsigned int i = 0; i < count; i++)
			AddOrChangeToContactPersonsListCtrl(contactPersonEntities->Get(i));

		BankConnectionEntities *bankConnectionEntities = clientEntity->GetBankConnectionEntities();
		count = bankConnectionEntities->GetCount();
		for(unsigned int i = 0; i < count; i++)
			AddOrChangeToBankConnectionsListCtrl(bankConnectionEntities->Get(i));

		oldOwnClientId = clientEntity->Get(_N("ownClientId"))->ToString();

		kindComboBox->SetFocus();
	} else {
		ownClientIdTextCtrl->SetFocus();
	}
	
	ChangeFormOfOrganization();

	clientIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	discountTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);

	clientIdTextCtrl->SetChange(&changed);
	ownClientIdTextCtrl->SetChange(&changed);
	formOfOrganizationComboBox->SetChange(&changed);
	circularLetterTextCtrl->SetChange(&changed);
	salutationComboBox->SetChange(&changed);
	titleComboBox->SetChange(&changed);
	firstnameTextCtrl->SetChange(&changed);
	name1TextCtrl->SetChange(&changed);
	name2TextCtrl->SetChange(&changed);
	shortNameTextCtrl->SetChange(&changed);
	addressTextCtrl->SetChange(&changed);
	numberTextCtrl->SetChange(&changed);
	zipcodeTextCtrl->SetChange(&changed);
	cityTextCtrl->SetChange(&changed);
	federalStateComboBox->SetChange(&changed);
	landComboBox->SetChange(&changed);
	name3TextCtrl->SetChange(&changed);
	name4TextCtrl->SetChange(&changed);
	telephone1CountryTextCtrl->SetChange(&changed);
	telephone1PrefixTextCtrl->SetChange(&changed);
	telephone1NumberTextCtrl->SetChange(&changed);
	telephone2CountryTextCtrl->SetChange(&changed);
	telephone2PrefixTextCtrl->SetChange(&changed);
	telephone2NumberTextCtrl->SetChange(&changed);
	telephone3CountryTextCtrl->SetChange(&changed);
	telephone3PrefixTextCtrl->SetChange(&changed);
	telephone3NumberTextCtrl->SetChange(&changed);
	fax1CountryTextCtrl->SetChange(&changed);
	fax1PrefixTextCtrl->SetChange(&changed);
	fax1NumberTextCtrl->SetChange(&changed);
	fax2CountryTextCtrl->SetChange(&changed);
	fax2PrefixTextCtrl->SetChange(&changed);
	fax2NumberTextCtrl->SetChange(&changed);
	fax3CountryTextCtrl->SetChange(&changed);
	fax3PrefixTextCtrl->SetChange(&changed);
	fax3NumberTextCtrl->SetChange(&changed);
	mobile1CountryTextCtrl->SetChange(&changed);
	mobile1PrefixTextCtrl->SetChange(&changed);
	mobile1NumberTextCtrl->SetChange(&changed);
	mobile2CountryTextCtrl->SetChange(&changed);
	mobile2PrefixTextCtrl->SetChange(&changed);
	mobile2NumberTextCtrl->SetChange(&changed);
	email1TextCtrl->SetChange(&changed);
	email2TextCtrl->SetChange(&changed);
	internet1TextCtrl->SetChange(&changed);
	internet2TextCtrl->SetChange(&changed);
	informationsTextCtrl->SetChange(&changed);
	taxNumberTextCtrl->SetChange(&changed);
	discountTextCtrl->SetChange(&changed);
	liableToTaxCheckBox->SetChange(&changed);
	sectorComboBox->SetChange(&changed);
	groupComboBox->SetChange(&changed);
	kindComboBox->SetChange(&changed);
	birthdayDayComboBox->SetChange(&changed);
	birthdayMonthComboBox->SetChange(&changed);
	birthdayYearComboBox->SetChange(&changed);
	activeCheckBox->SetChange(&changed);
	monitionableCheckBox->SetChange(&changed);

	this->editedInformationsDialog = new EditedInformationsDialog(this, -1, rit, clientEntity);
	this->editedInformationsDialog->Show(false);

	this->hiddenValue = new wxString;
	if(clientEntity)
		*hiddenValue = clientEntity->Get(_N("hiddenInformation"))->ToString();

	this->hiddenInformationsDialog = new TextEditDialog(this, -1, rit, &changed, hiddenValue);
	this->hiddenInformationsDialog->Show(false);

	changed = false;

	mediaBoxSizer->RecalcSizes();
	moreDataBoxSizer->RecalcSizes();

	//
	EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());
	EnterprisePreferencesController enterprisePreferencesController(rit->GetDatabase());
	enterprisePreferencesController.Get();
	if(enterprisePreferencesController.Run()) {
		enterprisePreferencesController.Get(&enterprisePreferencesEntity);
		enterprisePreferencesController.Reset();
		
		mandatoryOwnClientId = enterprisePreferencesEntity.Get(_N("mandatoryClientsOwnClientId"))->ToBool();
		if(mandatoryOwnClientId)
			ownClientIdStaticText->SetForegroundColour(MANDATORY_FIELD_COLOUR);
	}

	clientIdTextCtrl->Disable();

	ICON();
}

ClientEditFrame::~ClientEditFrame() {
	editedInformationsDialog->Destroy();
	hiddenInformationsDialog->Destroy();
	if(hiddenValue)
		delete hiddenValue;
}

// Events
void ClientEditFrame::OnClose(wxCloseEvent &event) {
	bool ok = true;
	if(changed) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to save the changes?"),
			_("Close"),
			_("Save"),
			_("Close")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES)
			ok = Save();
		yesNoDialog->Destroy();
	}

	if(ok) {
		ClientEntity *clientEntity = static_cast<ClientEntity *>(bind.GetEntity());
		delete clientEntity;
		rit->Unblock(_N("clients"), clientId);

		Destroy();
		event.Skip();
	} else {
		event.Veto();
	}
}

void ClientEditFrame::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void ClientEditFrame::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void ClientEditFrame::PrintEvent(wxMouseEvent &event) {
	Print();
	SKIP();
}

void ClientEditFrame::PrintPreviewEvent(wxMouseEvent &event) {
	Print(true);
	SKIP();
}

void ClientEditFrame::ChangeFormOfOrganizationEvent(wxCommandEvent &event) {
	ChangeFormOfOrganization();
}

void ClientEditFrame::ShowHiddenInformationsEvent(wxMouseEvent &event) {
	ShowHiddenInformations();
	SKIP();
}

void ClientEditFrame::ShowEditedInformationsEvent(wxMouseEvent &event) {
	ShowEditedInformations();
	SKIP();
}

void ClientEditFrame::OrdersEvent(wxMouseEvent &event) {
	Orders();
	SKIP();
}

void ClientEditFrame::RemindersEvent(wxMouseEvent &event) {
	Reminders();
	SKIP();
}

void ClientEditFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void ClientEditFrame::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void ClientEditFrame::PrintKeyEvent(wxCommandEvent &event) {
	Print();
}

void ClientEditFrame::PrintPreviewKeyEvent(wxCommandEvent &event) {
	Print(true);
}

void ClientEditFrame::ShowHiddenInformationsKeyEvent(wxCommandEvent &event) {
	ShowHiddenInformations();
}

void ClientEditFrame::ShowEditedInformationsKeyEvent(wxCommandEvent &event) {
	ShowEditedInformations();
}

void ClientEditFrame::OrdersKeyEvent(wxCommandEvent &event) {
	Orders();
}

void ClientEditFrame::RemindersKeyEvent(wxCommandEvent &event) {
	Reminders();
}

void ClientEditFrame::AddressAddEvent(wxCommandEvent &event) {
	AddressAdd();
}

void ClientEditFrame::AddressAddKeyEvent(wxCommandEvent &event) {
	AddressAdd();
}

void ClientEditFrame::AddressRemoveEvent(wxCommandEvent &event) {
	AddressRemove();
}

void ClientEditFrame::AddressRemoveKeyEvent(wxCommandEvent &event) {
	AddressRemove();
}

void ClientEditFrame::AddressChangeEvent(wxCommandEvent &event) {
	AddressChange();
}

void ClientEditFrame::AddressChangeKeyEvent(wxCommandEvent &event) {
	AddressChange();
}

void ClientEditFrame::AddressEditEvent(wxListEvent &event) {
	AddressEdit();
}

void ClientEditFrame::ContactPersonAddEvent(wxCommandEvent &event) {
	ContactPersonAdd();
}

void ClientEditFrame::ContactPersonAddKeyEvent(wxCommandEvent &event) {
	ContactPersonAdd();
}

void ClientEditFrame::ContactPersonRemoveEvent(wxCommandEvent &event) {
	ContactPersonRemove();
}

void ClientEditFrame::ContactPersonRemoveKeyEvent(wxCommandEvent &event) {
	ContactPersonRemove();
}

void ClientEditFrame::ContactPersonEditEvent(wxListEvent &event) {
	ContactPersonEdit();
}

void ClientEditFrame::BankConnectionEditEvent(wxListEvent &event) {
	BankConnectionEdit();
}

void ClientEditFrame::BankConnectionAddEvent(wxCommandEvent &event) {
	BankConnectionAdd();
}

void ClientEditFrame::BankConnectionRemoveEvent(wxCommandEvent &event) {
	BankConnectionRemove();
}

void ClientEditFrame::BankConnectionAddKeyEvent(wxCommandEvent &event) {
	BankConnectionAdd();
}

void ClientEditFrame::BankConnectionRemoveKeyEvent(wxCommandEvent &event) {
	BankConnectionRemove();
}

// Methods
void ClientEditFrame::Close() {
	wxWindow::Close();
}

bool ClientEditFrame::Save() {
	clientIdTextCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to save this user."), this))
		return false;

	if((mandatoryOwnClientId && ownClientIdTextCtrl->GetValue().IsEmpty()) || name1TextCtrl->IsEmpty()) {
		rit->Log(true, _N("ClientEditFrame"), _("The client is not saved."), _("No name (Name 1) given."));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The following fields are necessary: Name 1") + (mandatoryOwnClientId ? wxString(_(", Own customer number")) : wxString()),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	ClientController clientController(rit);
	if(!ownClientIdTextCtrl->GetValue().IsEmpty() && clientController.ExistsOwnClientId(ownClientIdTextCtrl->GetValue(), oldOwnClientId)) {
		rit->Log(true, _N("ClientEditFrame"), _("The client is not saved."), _("The own customer id exists."));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The client is not saved. The own customer id exists."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	ClientEntity *clientEntity = static_cast<ClientEntity *>(bind.GetEntity());
	if(!clientEntity) {
		rit->Log(true, _N("ClientEditFrame"), _("The client is not saved."), _("Null Pointer."));
		return false;
	}

	bind.Pull();
	clientEntity->SetValue(_N("clientId"), clientId);
	DSCurrency price(clientEntity->Get(_N("discount"))->ToCurrency());
	clientEntity->SetValue(_N("discount"), price);
	clientEntity->SetValue(_N("hiddenInformation"), hiddenInformationsDialog->GetValue());

	bool ok = false;
	ok = clientController.Save(clientEntity, &removedAddresses, &removedContactPersons);

	if(ok) {
		clientId = clientEntity->Get(_N("clientId"))->ToInt();
		clientController.Get(DSCasts::ToString(clientId)); // Autofelder refreshen
		clientController.Run();
		clientController.Get(clientEntity, true);
		clientController.Reset();

		ownClientIdTextCtrl->Disable();
		oldOwnClientId = clientEntity->Get(_N("ownClientId"))->ToString();

		DSCurrency price(clientEntity->Get(_N("discount"))->ToCurrency());
		clientEntity->SetValue(_N("discount"), price);
		bind.Push();

		if(callbackInterface)
			callbackInterface->Callback(static_cast<void *>(clientEntity));

		rit->Block(_N("clients"), clientId, this);

		changed = false;

		rit->Log(false, _N("ClientEditFrame"), _("The client is saved."), _("The client with client id \"") + DSCasts::ToString(clientId) + _N("\" is saved."));

		kindComboBox->Disable();

		editedInformationsDialog->Update();
	} else {
		rit->Log(true, _N("ClientEditFrame"), _("The client is not saved."), _("Database error."));

		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not save."),
			_("Save client")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}

	return ok;
}

void ClientEditFrame::ChangeFormOfOrganization() {
	const bool enabled = formOfOrganizationComboBox->GetSelection() == 0;

	firstnameStaticText->Enable(enabled);
	firstnameTextCtrl->Enable(enabled);

	if(!enabled) {
		firstname = firstnameTextCtrl->GetValue();
		firstnameTextCtrl->Clear();
	} else {
		firstnameTextCtrl->SetValue(firstname);
	}
}

void ClientEditFrame::ShowHiddenInformations() {
	hiddenInformationsDialog->Center();
	hiddenInformationsDialog->ResetValue();
	hiddenInformationsDialog->Show(!hiddenInformationsDialog->IsShown());
}

void ClientEditFrame::ShowEditedInformations() {
	editedInformationsDialog->Get(!editedInformationsDialog->IsShown());
}

void ClientEditFrame::Print(const bool preview) {
	clientIdTextCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to print."), this))
		return;
	
	if(changed) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Save first this entry and press print again. Only saved entries can be printed."),
			_("Print")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	if(!JasperReportsLibrary::DoReport(
		this,
		rit,
		_N("clients"),
		clientId,
		preview ? JasperReportsController::SHOW : JasperReportsController::PRINT
	)) {
		ClientController clientController(rit);
		HtmlPrintoutLibrary *htmlPrintoutLibrary = clientController.Print(static_cast<ClientEntity *>(bind.GetEntity()));
		
		if(preview) {
			PrintPreviewDialog *printPreviewDialog = new PrintPreviewDialog(this, -1, rit, htmlPrintoutLibrary);
			printPreviewDialog->Center();
			printPreviewDialog->ShowModal();
			printPreviewDialog->Destroy();
		} else {
			wxPrinter printer;
			printer.Print(this, htmlPrintoutLibrary, true);
		}

		delete htmlPrintoutLibrary;
	}
}

void ClientEditFrame::Orders() {
	clientIdTextCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to open orders."), this))
		return;
	
	if(changed || clientId == 0) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Save first this entry and press orders again."),
			_("Orders")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	if(!clientOrdersDialog) {
		clientOrdersDialog = new ClientOrdersDialog(this, -1, rit, clientId, false);
		clientOrdersDialog->Center();
	}

	clientOrdersDialog->Show(!clientOrdersDialog->IsShown());
}

void ClientEditFrame::AddressAdd() {
	if(!rit->Access(A_WRITE, _("You have no permission to add a new entry."), this))
		return;

	AddressEntity *addressEntity         = new AddressEntity(rit);
	AddressEditDialog *addressEditDialog = new AddressEditDialog(
		this,
		-1,
		rit,
		addressEntity
	);
	addressEditDialog->Center();
	if(addressEditDialog->ShowModal() == wxID_OK) {
		ClientEntity *clientEntity = static_cast<ClientEntity *>(bind.GetEntity());
		clientEntity->GetAddresses()->Add(addressEntity);
		AddOrChangeToAddressesListCtrl(addressEntity);

		changed = true;
	} else {
		delete addressEntity;
	}
	addressEditDialog->Destroy();
}

void ClientEditFrame::AddressRemove() {
	if(!rit->Access(A_DELETE, _("You have no permission to delete this entry."), this))
		return;

	AddressEntity *addressEntity = static_cast<AddressEntity *>(addressesListCtrl->GetSingleSelectionPointer());
	if(!addressEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to remove."),
			_("Remove")
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
		_("Do you really want to remove the selected entry?"),
		_("Remove"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		if(addressEntity->Get(_N("addressId"))->ToUnsignedInt() != 0)
			removedAddresses.Add(addressEntity->Get(_N("addressId"))->ToString());
		addressesListCtrl->DeleteItem(addressesListCtrl->GetPosition(addressEntity));
		ClientEntity *clientEntity = static_cast<ClientEntity *>(bind.GetEntity());
		clientEntity->GetAddresses()->Remove(addressEntity, true);

		changed = true;
	}
	yesNoDialog->Destroy();
}

void ClientEditFrame::AddressChange() {
	if(!rit->Access(A_WRITE, _("You have no permission to change an address."), this))
		return;

	AddressEntity *addressEntity = static_cast<AddressEntity *>(addressesListCtrl->GetSingleSelectionPointer());
	if(!addressEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to change."),
			_("Remove")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return;
	}

	AddressEntity tmpAddressEntity(rit);
	tmpAddressEntity.SetValue(_N("salutation"), salutationComboBox->GetValue());
	tmpAddressEntity.SetValue(_N("title"), titleComboBox->GetValue());
	tmpAddressEntity.SetValue(_N("firstname"), firstnameTextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("name1"), name1TextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("name2"), name2TextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("name3"), name3TextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("name4"), name4TextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("address"), addressTextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("number"), numberTextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("zipcode"), zipcodeTextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("city"), cityTextCtrl->GetValue());
	tmpAddressEntity.SetValue(_N("federalState"), federalStateComboBox->GetValue());
	tmpAddressEntity.SetValue(_N("land"), landComboBox->GetValue());

	salutationComboBox->SetValue(addressEntity->Get(_N("salutation"))->ToString());
	titleComboBox->SetValue(addressEntity->Get(_N("title"))->ToString());
	firstnameTextCtrl->SetValue(addressEntity->Get(_N("firstname"))->ToString());
	name1TextCtrl->SetValue(addressEntity->Get(_N("name1"))->ToString());
	name2TextCtrl->SetValue(addressEntity->Get(_N("name2"))->ToString());
	name3TextCtrl->SetValue(addressEntity->Get(_N("name3"))->ToString());
	name4TextCtrl->SetValue(addressEntity->Get(_N("name4"))->ToString());
	addressTextCtrl->SetValue(addressEntity->Get(_N("address"))->ToString());
	numberTextCtrl->SetValue(addressEntity->Get(_N("number"))->ToString());
	zipcodeTextCtrl->SetValue(addressEntity->Get(_N("zipcode"))->ToString());
	cityTextCtrl->SetValue(addressEntity->Get(_N("city"))->ToString());
	federalStateComboBox->SetValue(addressEntity->Get(_N("federalState"))->ToString());
	landComboBox->SetValue(addressEntity->Get(_N("land"))->ToString());

	addressEntity->SetValue(_N("salutation"), tmpAddressEntity.Get(_N("salutation"))->ToString());
	addressEntity->SetValue(_N("title"), tmpAddressEntity.Get(_N("title"))->ToString());
	addressEntity->SetValue(_N("firstname"), tmpAddressEntity.Get(_N("firstname"))->ToString());
	addressEntity->SetValue(_N("name1"), tmpAddressEntity.Get(_N("name1"))->ToString());
	addressEntity->SetValue(_N("name2"), tmpAddressEntity.Get(_N("name2"))->ToString());
	addressEntity->SetValue(_N("name3"), tmpAddressEntity.Get(_N("name3"))->ToString());
	addressEntity->SetValue(_N("name4"), tmpAddressEntity.Get(_N("name4"))->ToString());
	addressEntity->SetValue(_N("address"), tmpAddressEntity.Get(_N("address"))->ToString());
	addressEntity->SetValue(_N("number"), tmpAddressEntity.Get(_N("number"))->ToString());
	addressEntity->SetValue(_N("zipcode"), tmpAddressEntity.Get(_N("zipcode"))->ToString());
	addressEntity->SetValue(_N("city"), tmpAddressEntity.Get(_N("city"))->ToString());
	addressEntity->SetValue(_N("federalState"), tmpAddressEntity.Get(_N("federalState"))->ToString());
	addressEntity->SetValue(_N("land"), tmpAddressEntity.Get(_N("land"))->ToString());
	
	AddOrChangeToAddressesListCtrl(addressEntity);

	changed = true;
}

void ClientEditFrame::AddressEdit() {
	if(!rit->Access(A_READ, _("You have no permission to edit this entry."), this))
		return;

	AddressEntity *addressEntity         = static_cast<AddressEntity *>(addressesListCtrl->GetSingleSelectionPointer());
	AddressEditDialog *addressEditDialog = new AddressEditDialog(
		this,
		-1,
		rit,
		addressEntity,
		rit->Access(A_WRITE)
	);
	addressEditDialog->Center();
	if(addressEditDialog->ShowModal() == wxID_OK)
		AddOrChangeToAddressesListCtrl(addressEntity);
	addressEditDialog->Destroy();
}

void ClientEditFrame::ContactPersonAdd() {
	if(!rit->Access(A_WRITE, _("You have no permission to add a new entry."), this))
		return;

	ContactPersonEntity *contactPersonEntity         = new ContactPersonEntity(rit);
	ContactPersonEditDialog *contactPersonEditDialog = new ContactPersonEditDialog(
		this,
		-1,
		rit,
		contactPersonEntity
	);
	contactPersonEditDialog->Center();
	if(contactPersonEditDialog->ShowModal() == wxID_OK) {
		ClientEntity *clientEntity = static_cast<ClientEntity *>(bind.GetEntity());
		clientEntity->GetContactPersons()->Add(contactPersonEntity);
		AddOrChangeToContactPersonsListCtrl(contactPersonEntity);

		changed = true;
	} else {
		delete contactPersonEntity;
	}
	contactPersonEditDialog->Destroy();
}

void ClientEditFrame::ContactPersonRemove() {
	if(!rit->Access(A_DELETE, _("You have no permission to remove this entry."), this))
		return;

	ContactPersonEntity *contactPersonEntity = static_cast<ContactPersonEntity *>(contactPersonsListCtrl->GetSingleSelectionPointer());
	if(!contactPersonEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to remove."),
			_("Remove")
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
		_("Do you really want to remove the selected entry?"),
		_("Remove"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		if(contactPersonEntity->Get(_N("contactPersonId"))->ToUnsignedInt() != 0)
			removedContactPersons.Add(contactPersonEntity->Get(_N("contactPersonId"))->ToString());
		contactPersonsListCtrl->DeleteItem(contactPersonsListCtrl->GetPosition(contactPersonEntity));
		ClientEntity *clientEntity = static_cast<ClientEntity *>(bind.GetEntity());
		clientEntity->GetContactPersons()->Remove(contactPersonEntity, true);

		changed = true;
	}
	yesNoDialog->Destroy();
}

void ClientEditFrame::ContactPersonEdit() {
	if(!rit->Access(A_READ, _("You have no permission to edit this entry."), this))
		return;

	ContactPersonEntity *contactPersonEntity         = static_cast<ContactPersonEntity *>(contactPersonsListCtrl->GetSingleSelectionPointer());
	ContactPersonEditDialog *contactPersonEditDialog = new ContactPersonEditDialog(
		this,
		-1,
		rit,
		contactPersonEntity,
		rit->Access(A_WRITE)
	);
	contactPersonEditDialog->Center();
	if(contactPersonEditDialog->ShowModal() == wxID_OK) {
		AddOrChangeToContactPersonsListCtrl(contactPersonEntity);
		changed = true;
	}
	contactPersonEditDialog->Destroy();
}

void ClientEditFrame::BankConnectionEdit() {
	if(!rit->Access(A_READ, _("You have no permission to edit this entry."), this))
		return;

	BankConnectionEntity *bankConnectionEntity         = static_cast<BankConnectionEntity *>(bankConnectionsListCtrl->GetSingleSelectionPointer());
	BankConnectionEditDialog *bankConnectionEditDialog = new BankConnectionEditDialog(
		this,
		-1,
		rit,
		bankConnectionEntity
	);
	bankConnectionEditDialog->Center();
	if(bankConnectionEditDialog->ShowModal() == wxID_OK) {
		AddOrChangeToBankConnectionsListCtrl(bankConnectionEntity);
		changed = true;
	}
	bankConnectionEditDialog->Destroy();
}

void ClientEditFrame::BankConnectionAdd() {
	if(!rit->Access(A_WRITE, _("You have no permission to add a new entry."), this))
		return;

	BankConnectionEntity *bankConnectionEntity         = new BankConnectionEntity(rit);
	BankConnectionEditDialog *bankConnectionEditDialog = new BankConnectionEditDialog(
		this,
		-1,
		rit,
		bankConnectionEntity
	);
	bankConnectionEditDialog->Center();
	if(bankConnectionEditDialog->ShowModal() == wxID_OK) {
		ClientEntity *clientEntity = static_cast<ClientEntity *>(bind.GetEntity());
		clientEntity->GetBankConnectionEntities()->Add(bankConnectionEntity);
		AddOrChangeToBankConnectionsListCtrl(bankConnectionEntity);

		changed = true;
	} else {
		delete bankConnectionEntity;
	}
	bankConnectionEditDialog->Destroy();
}

void ClientEditFrame::BankConnectionRemove() {
	if(!rit->Access(A_DELETE, _("You have no permission to remove this entry."), this))
		return;

	BankConnectionEntity *bankConnectionEntity = static_cast<BankConnectionEntity *>(bankConnectionsListCtrl->GetSingleSelectionPointer());
	if(!bankConnectionEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to remove."),
			_("Remove")
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
		_("Do you really want to remove the selected entry?"),
		_("Remove"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		ClientEntity *clientEntity = static_cast<ClientEntity *>(bind.GetEntity());
		if(bankConnectionEntity->Get(_N("bankConnectionId"))->ToUnsignedInt() != 0)
			clientEntity->GetBankConnectionEntities()->GetRemoved()->Add(bankConnectionEntity->Get(_N("bankConnectionId"))->ToString());
		bankConnectionsListCtrl->DeleteItem(bankConnectionsListCtrl->GetPosition(bankConnectionEntity));
		clientEntity->GetBankConnectionEntities()->Remove(bankConnectionEntity, true);
		changed = true;
	}
	yesNoDialog->Destroy();
}

void ClientEditFrame::AddOrChangeToAddressesListCtrl(AddressEntity *addressEntity) {
	if(!addressEntity)
		return;

	DSListCtrl::DSListCtrlRow *addressesListCtrlRow = new DSListCtrl::DSListCtrlRow(addressEntity);
	addressesListCtrlRow->Add(addressEntity->Get(_N("description"))->ToString());
	addressesListCtrlRow->Add(NameType(
		addressEntity->Get(_N("salutation"))->ToString(),
		addressEntity->Get(_N("title"))->ToString(),
		addressEntity->Get(_N("firstname"))->ToString(),
		addressEntity->Get(_N("name1"))->ToString(),
		addressEntity->Get(_N("name2"))->ToString(),
		addressEntity->Get(_N("name3"))->ToString(),
		addressEntity->Get(_N("name4"))->ToString()
	).GetConcatedFullName(_N(", ")));
	addressesListCtrlRow->Add(AddressType::GetAddressAndNumber(
		addressEntity->Get(_N("address"))->ToString(),
		addressEntity->Get(_N("number"))->ToString()
	));
	addressesListCtrlRow->Add(addressEntity->Get(_N("zipcode"))->ToString());
	addressesListCtrlRow->Add(addressEntity->Get(_N("city"))->ToString());
	addressesListCtrlRow->Add(addressEntity->Get(_N("federalState"))->ToString());
	addressesListCtrlRow->Add(addressEntity->Get(_N("land"))->ToString());

	addressesListCtrl->Set(addressesListCtrlRow, true);

	changed = true;
}

void ClientEditFrame::AddOrChangeToContactPersonsListCtrl(ContactPersonEntity *contactPersonEntity) {
	if(!contactPersonEntity)
		return;

	DSListCtrl::DSListCtrlRow *contactPersonsListCtrlRow = new DSListCtrl::DSListCtrlRow(contactPersonEntity);
	contactPersonsListCtrlRow->Add(contactPersonEntity->Get(_N("department"))->ToString());
	contactPersonsListCtrlRow->Add(contactPersonEntity->Get(_N("function"))->ToString());
	contactPersonsListCtrlRow->Add(NameType(
		contactPersonEntity->Get(_N("salutation"))->ToString(),
		contactPersonEntity->Get(_N("title"))->ToString(),
		contactPersonEntity->Get(_N("firstname"))->ToString(),
		contactPersonEntity->Get(_N("lastname"))->ToString(),
		wxEmptyString,
		wxEmptyString,
		wxEmptyString
	).GetConcatedFullName(_N(", ")));
	contactPersonsListCtrlRow->Add(contactPersonEntity->Get(_N("email"))->ToString());
	contactPersonsListCtrlRow->Add(PhoneType(
		contactPersonEntity->Get(_N("telephoneCountry"))->ToString(),
		contactPersonEntity->Get(_N("telephonePrefix"))->ToString(),
		contactPersonEntity->Get(_N("telephoneNumber"))->ToString()
	).ToString());
	contactPersonsListCtrlRow->Add(PhoneType(
		contactPersonEntity->Get(_N("mobileCountry"))->ToString(),
		contactPersonEntity->Get(_N("mobilePrefix"))->ToString(),
		contactPersonEntity->Get(_N("mobileNumber"))->ToString()
	).ToString());
	contactPersonsListCtrlRow->Add(PhoneType(
		contactPersonEntity->Get(_N("faxCountry"))->ToString(),
		contactPersonEntity->Get(_N("faxPrefix"))->ToString(),
		contactPersonEntity->Get(_N("faxNumber"))->ToString()
	).ToString());

	contactPersonsListCtrl->Set(contactPersonsListCtrlRow, true);

	changed = true;
}

void ClientEditFrame::AddOrChangeToBankConnectionsListCtrl(BankConnectionEntity *bankConnectionEntity) {
	if(!bankConnectionEntity)
		return;

	DSListCtrl::DSListCtrlRow *bankConnectionsListCtrlRow = new DSListCtrl::DSListCtrlRow(bankConnectionEntity);
	bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("active"))->ToBool() ? _("yes") : _("no"));
	bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("description"))->ToString());
	bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("bank"))->ToString());
	bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("bic"))->ToString());
	bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("iban"))->ToString());
	bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("bankOwner"))->ToString());

	bankConnectionsListCtrl->Set(bankConnectionsListCtrlRow, true);

	changed = true;
}

void ClientEditFrame::Reminders() {
	if(!rit->Access(A_READ, _("You have no permission to open reminders."), this))
		return;

	if(clientId == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please save first."),
			_("Reminders")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	RemindersDialog *remindersDialog = new RemindersDialog(
		this,
		-1,
		rit,
		ReminderType::CLIENTS,
		clientId
	);
	remindersDialog->Center();
	remindersDialog->ShowModal();
	remindersDialog->Destroy();
}
