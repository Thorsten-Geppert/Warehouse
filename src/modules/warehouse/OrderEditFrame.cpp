#include "OrderEditFrame.h"
#include <DSCurrency.h>
#include "../lib/OrderController.h"
#include "../lib/OrderPositionEntity.h"
#include "../lib/OrderPositionEntities.h"
#include "../lib/ListEntryController.h"
#include "../lib/CalculationLibrary.h"
#include "../lib/ProductController.h"
#include "../lib/ClientController.h"
#include "../lib/ConstantsLibrary.h"
#include "../lib/Tools.h"
#include "../lib/AddressEntity.h"
#include "../lib/AddressEntities.h"
#include "../lib/BankConnectionEntity.h"
#include "../lib/ReminderType.h"
#include "../lib/KeyValueDatabaseLibrary.h"
#include "../lib/JasperReportsLibrary.h"
#include "TemplateSelectDialog.h"
#include "OrderPositionEditDialog.h"
#include "ClientsFullFindDialog.h"
#include "PrintPreviewDialog.h"
#include "BanksFullFindDialog.h"
#include "TranslatedAddressEditDialog.h"
#include "YesNoDialog.h"
#include "Configuration.h"
#include "AddressSelectDialog.h"
#include "BankConnectionSelectDialog.h"
#include "RemindersDialog.h"
#include "PayedDialog.h"
#include "InvoiceDialog.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(OrderEditFrame, wxFrame)
	EVT_CLOSE(OrderEditFrame::OnClose)
	EVT_LIST_ITEM_ACTIVATED(XRCID("orderPositionsListCtrl"), OrderEditFrame::DirectEditPositionEvent)
	EVT_BUTTON(XRCID("billAddressTransferButton"), OrderEditFrame::TransferAddressEvent)
	EVT_BUTTON(XRCID("shippingAddressTransferButton"), OrderEditFrame::TransferAddressEvent)
	EVT_BUTTON(XRCID("transferBankButton"), OrderEditFrame::TransferBankEvent)

	EVT_BUTTON(XRCID("downPositionButton"), OrderEditFrame::DownPositionEvent)
	EVT_BUTTON(XRCID("upPositionButton"), OrderEditFrame::UpPositionEvent)
	EVT_BUTTON(XRCID("removePositionButton"), OrderEditFrame::RemovePositionEvent)
	EVT_BUTTON(XRCID("editPositionButton"), OrderEditFrame::EditPositionEvent)
	EVT_BUTTON(XRCID("addPositionButton"), OrderEditFrame::AddPositionEvent)

	EVT_BUTTON(XRCID("downLinkedOrderButton"), OrderEditFrame::DownLinkedOrderEvent)
	EVT_BUTTON(XRCID("upLinkedOrderButton"), OrderEditFrame::UpLinkedOrderEvent)
	EVT_BUTTON(XRCID("unlinkOrderButton"), OrderEditFrame::UnlinkOrderEvent)
	EVT_BUTTON(XRCID("linkOrderButton"), OrderEditFrame::LinkOrderEvent)
	EVT_BUTTON(XRCID("negatePositionPricesButton"), OrderEditFrame::NegatePositionPricesEvent)

	EVT_BUTTON(XRCID("shippingAddressTranslateEditButton"), OrderEditFrame::AddressTranslateEvent)
	EVT_BUTTON(XRCID("billAddressTranslateEditButton"), OrderEditFrame::AddressTranslateEvent)

	EVT_MENU(DOWN_LINKED_ORDER_EVENT, OrderEditFrame::DownLinkedOrderKeyEvent)
	EVT_MENU(UP_LINKED_ORDER_EVENT, OrderEditFrame::UpLinkedOrderKeyEvent)
	EVT_MENU(UNLINK_ORDER_EVENT, OrderEditFrame::UnlinkOrderKeyEvent)
	EVT_MENU(LINK_ORDER_EVENT, OrderEditFrame::LinkOrderKeyEvent)

	EVT_MENU(CLOSE_EVENT, OrderEditFrame::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, OrderEditFrame::SaveKeyEvent)
	EVT_MENU(EDITED_INFORMATIONS_EVENT, OrderEditFrame::ShowEditedInformationsKeyEvent)
	EVT_MENU(FIND_CLIENT_EVENT, OrderEditFrame::FindClientKeyEvent)
	EVT_MENU(DOWN_POSITION_EVENT, OrderEditFrame::DownPositionKeyEvent)
	EVT_MENU(UP_POSITION_EVENT, OrderEditFrame::UpPositionKeyEvent)
	EVT_MENU(ADD_POSITION_EVENT, OrderEditFrame::AddPositionKeyEvent)
	EVT_MENU(EDIT_POSITION_EVENT, OrderEditFrame::EditPositionKeyEvent)
	EVT_MENU(REMOVE_POSITION_EVENT, OrderEditFrame::RemovePositionKeyEvent)
	EVT_MENU(INVOICING_EVENT, OrderEditFrame::InvoicingKeyEvent)
	EVT_MENU(PRINT_EVENT, OrderEditFrame::PrintKeyEvent)
	EVT_MENU(PRINT_PREVIEW_EVENT, OrderEditFrame::PrintPreviewKeyEvent)
	EVT_MENU(PAYED_EVENT, OrderEditFrame::PayedKeyEvent)
	EVT_MENU(NEGATE_POSITION_PRICES_EVENT, OrderEditFrame::NegatePositionPricesKeyEvent)
	EVT_TEXT(XRCID("clientIdTextCtrl"), OrderEditFrame::ClientIdEvent)
	EVT_CHECKBOX(XRCID("liableToTaxCheckBox"), OrderEditFrame::RecalcPositionsEvent)
	EVT_COMBOBOX(XRCID("taxRateComboBox"), OrderEditFrame::RecalcPositionsEvent)
	EVT_MENU(PRINT_NORMAL_EVENT, OrderEditFrame::PrintOutEvent)
	EVT_MENU(PRINT_DELIVERY_EVENT, OrderEditFrame::PrintOutEvent)
	EVT_MENU(REMINDERS_EVENT, OrderEditFrame::RemindersKeyEvent)
	EVT_BUTTON(XRCID("findBankButton"), OrderEditFrame::BankFindEvent)
END_EVENT_TABLE()

OrderEditFrame::OrderEditFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	OrderEntity *orderEntity,
	GuiCallbackInterface *callbackInterface,
	const bool deleteOrderEntity
) : bind(
	orderEntity
) {
	changed                 = false;
	clientId                = 0;
	clientOrdersDialog      = NULL;
	loaded                  = false;
	SetId(id);
	this->rit               = rit;
	this->callbackInterface = callbackInterface;
	this->preview           = false;
	this->deleteOrderEntity = deleteOrderEntity;
	orderId                 = orderEntity ? orderEntity->Get(_N("orderId"))->ToUnsignedInt() : 0;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("OrderEditFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("OrderEditFrame"));

	mainPanel                            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton                    = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	findClientToolbarButton              = XRCCTRL(*this, "findClientToolbarButton", ToolbarButton);
	closeToolbarButton                   = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	editedInformationsToolbarButton      = XRCCTRL(*this, "editedInformationsToolbarButton", ToolbarButton);
	invoicingToolbarButton               = XRCCTRL(*this, "invoicingToolbarButton", ToolbarButton);
	printToolbarButton                   = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	remindersToolbarButton               = XRCCTRL(*this, "remindersToolbarButton", ToolbarButton);
	printPreviewToolbarButton            = XRCCTRL(*this, "printPreviewToolbarButton", ToolbarButton);
	payedToolbarButton                   = XRCCTRL(*this, "payedToolbarButton", ToolbarButton);
	mainToolbar                          = XRCCTRL(*this, "mainToolbar", Toolbar);
	kindComboBox                         = XRCCTRL(*this, "kindComboBox", DynSoft::DSComboBox);
	orderIdTextCtrl                      = XRCCTRL(*this, "orderIdTextCtrl", DynSoft::DSTextCtrl);
	basedOnTextCtrl                      = XRCCTRL(*this, "basedOnTextCtrl", DynSoft::DSTextCtrl);
	clientIdTextCtrl                     = XRCCTRL(*this, "clientIdTextCtrl", DynSoft::DSTextCtrl);
	ownClientIdTextCtrl                  = XRCCTRL(*this, "ownClientIdTextCtrl", DynSoft::DSTextCtrl);
	taxNumberTextCtrl                    = XRCCTRL(*this, "taxNumberTextCtrl", DynSoft::DSTextCtrl);
	salesTaxIdentificationNumberTextCtrl = XRCCTRL(*this, "salesTaxIdentificationNumberTextCtrl", DynSoft::DSTextCtrl);
	taxRateComboBox                      = XRCCTRL(*this, "taxRateComboBox", DynSoft::DSComboBox);
	dateDatePickerCtrl                   = XRCCTRL(*this, "dateDatePickerCtrl", DynSoft::DSDatePickerCtrl);
	ownerTextCtrl                        = XRCCTRL(*this, "ownerTextCtrl", DynSoft::DSTextCtrl);
	negatePositionPricesButton           = XRCCTRL(*this, "negatePositionPricesButton", wxButton);
	billAddressSalutationComboBox        = XRCCTRL(*this, "billAddressSalutationComboBox", DynSoft::DSComboBox);
	billAddressTitleComboBox             = XRCCTRL(*this, "billAddressTitleComboBox", DynSoft::DSComboBox);
	billAddressFirstnameTextCtrl         = XRCCTRL(*this, "billAddressFirstnameTextCtrl", DynSoft::DSTextCtrl);
	billAddressName1TextCtrl             = XRCCTRL(*this, "billAddressName1TextCtrl", DynSoft::DSTextCtrl);
	billAddressName2TextCtrl             = XRCCTRL(*this, "billAddressName2TextCtrl", DynSoft::DSTextCtrl);
	billAddressName3TextCtrl             = XRCCTRL(*this, "billAddressName3TextCtrl", DynSoft::DSTextCtrl);
	billAddressName4TextCtrl             = XRCCTRL(*this, "billAddressName4TextCtrl", DynSoft::DSTextCtrl);
	billAddressAddressTextCtrl           = XRCCTRL(*this, "billAddressAddressTextCtrl", DynSoft::DSTextCtrl);
	billAddressNumberTextCtrl            = XRCCTRL(*this, "billAddressNumberTextCtrl", DynSoft::DSTextCtrl);
	billAddressZipcodeTextCtrl           = XRCCTRL(*this, "billAddressZipcodeTextCtrl", DynSoft::DSTextCtrl);
	billAddressCityTextCtrl              = XRCCTRL(*this, "billAddressCityTextCtrl", DynSoft::DSTextCtrl);
	billAddressFederalStateComboBox      = XRCCTRL(*this, "billAddressFederalStateComboBox", DynSoft::DSComboBox);
	billAddressLandComboBox              = XRCCTRL(*this, "billAddressLandComboBox", DynSoft::DSComboBox);
	billAddressTransferButton            = XRCCTRL(*this, "billAddressTransferButton", wxButton);
	billAddressTranslateEditButton       = XRCCTRL(*this, "billAddressTranslateEditButton", wxButton);
	billAddressTranslatedStaticImage     = XRCCTRL(*this, "billAddressTranslatedStaticImage", wxStaticBitmap);
	shippingAddressSalutationComboBox    = XRCCTRL(*this, "shippingAddressSalutationComboBox", DynSoft::DSComboBox);
	shippingAddressTitleComboBox         = XRCCTRL(*this, "shippingAddressTitleComboBox", DynSoft::DSComboBox);
	shippingAddressFirstnameTextCtrl     = XRCCTRL(*this, "shippingAddressFirstnameTextCtrl", DynSoft::DSTextCtrl);
	shippingAddressName1TextCtrl         = XRCCTRL(*this, "shippingAddressName1TextCtrl", DynSoft::DSTextCtrl);
	shippingAddressName2TextCtrl         = XRCCTRL(*this, "shippingAddressName2TextCtrl", DynSoft::DSTextCtrl);
	shippingAddressName3TextCtrl         = XRCCTRL(*this, "shippingAddressName3TextCtrl", DynSoft::DSTextCtrl);
	shippingAddressName4TextCtrl         = XRCCTRL(*this, "shippingAddressName4TextCtrl", DynSoft::DSTextCtrl);
	shippingAddressAddressTextCtrl       = XRCCTRL(*this, "shippingAddressAddressTextCtrl", DynSoft::DSTextCtrl);
	shippingAddressNumberTextCtrl        = XRCCTRL(*this, "shippingAddressNumberTextCtrl", DynSoft::DSTextCtrl);
	shippingAddressZipcodeTextCtrl       = XRCCTRL(*this, "shippingAddressZipcodeTextCtrl", DynSoft::DSTextCtrl);
	shippingAddressCityTextCtrl          = XRCCTRL(*this, "shippingAddressCityTextCtrl", DynSoft::DSTextCtrl);
	shippingAddressFederalStateComboBox  = XRCCTRL(*this, "shippingAddressFederalStateComboBox", DynSoft::DSComboBox);
	shippingAddressLandComboBox          = XRCCTRL(*this, "shippingAddressLandComboBox", DynSoft::DSComboBox);
	shippingAddressTransferButton        = XRCCTRL(*this, "shippingAddressTransferButton", wxButton);
	shippingAddressTranslateEditButton   = XRCCTRL(*this, "shippingAddressTranslateEditButton", wxButton);
	shippingAddressTranslatedStaticImage = XRCCTRL(*this, "shippingAddressTranslatedStaticImage", wxStaticBitmap);
	positionsPanel                       = XRCCTRL(*this, "positionsPanel", wxPanel);
	positionsBoxSizer                    = static_cast<wxBoxSizer *>(positionsPanel->GetSizer());
	orderPositionsListCtrl               = XRCCTRL(*this, "orderPositionsListCtrl", DSListCtrl);
	downPositionButton                   = XRCCTRL(*this, "downPositionButton", wxButton);
	upPositionButton                     = XRCCTRL(*this, "upPositionButton", wxButton);
	removePositionButton                 = XRCCTRL(*this, "removePositionButton", wxButton);
	editPositionButton                   = XRCCTRL(*this, "editPositionButton", wxButton);
	addPositionButton                    = XRCCTRL(*this, "addPositionButton", wxButton);
	linkedOrdersPanel                    = XRCCTRL(*this, "linkedOrdersPanel", wxPanel);
	linkedOrdersBoxSizer                 = static_cast<wxBoxSizer *>(linkedOrdersPanel->GetSizer());
	linkedOrdersListCtrl                 = XRCCTRL(*this, "linkedOrdersListCtrl", DynSoft::DSListCtrl);
	downLinkedOrderButton                = XRCCTRL(*this, "downLinkedOrderButton", wxButton);
	upLinkedOrderButton                  = XRCCTRL(*this, "upLinkedOrderButton", wxButton);
	unlinkOrderButton                    = XRCCTRL(*this, "unlinkOrderButton", wxButton);
	linkOrderButton                      = XRCCTRL(*this, "linkOrderButton", wxButton);
	conditionComboBox                    = XRCCTRL(*this, "conditionComboBox", DynSoft::DSComboBox);
	paymentComboBox                      = XRCCTRL(*this, "paymentComboBox", DynSoft::DSComboBox);
	monitionCheckBox                     = XRCCTRL(*this, "monitionCheckBox", DynSoft::DSCheckBox);
	monitionStateStaticText              = XRCCTRL(*this, "monitionStateStaticText", wxStaticText);
	bankTextCtrl                         = XRCCTRL(*this, "bankTextCtrl", DynSoft::DSTextCtrl);
	bankCodeTextCtrl                     = XRCCTRL(*this, "bankCodeTextCtrl", DynSoft::DSTextCtrl);
	accountNumberTextCtrl                = XRCCTRL(*this, "accountNumberTextCtrl", DynSoft::DSTextCtrl);
	bicTextCtrl                          = XRCCTRL(*this, "bicTextCtrl", DynSoft::DSTextCtrl);
	ibanTextCtrl                         = XRCCTRL(*this, "ibanTextCtrl", DynSoft::DSTextCtrl);
	bankOwnerTextCtrl                    = XRCCTRL(*this, "bankOwnerTextCtrl", DynSoft::DSTextCtrl);
	liableToTaxCheckBox                  = XRCCTRL(*this, "liableToTaxCheckBox", DynSoft::DSCheckBox);
	orderNumberTextCtrl                  = XRCCTRL(*this, "orderNumberTextCtrl", DynSoft::DSTextCtrl);
	deliveryComboBox                     = XRCCTRL(*this, "deliveryComboBox", DynSoft::DSComboBox);
	prepandTextStaticText                = XRCCTRL(*this, "prepandTextStaticText", wxStaticText);
	prepandTextTextCtrl                  = XRCCTRL(*this, "prepandTextTextCtrl", DynSoft::DSTextCtrl);
	appendTextStaticText                 = XRCCTRL(*this, "appendTextStaticText", wxStaticText);
	appendTextTextCtrl                   = XRCCTRL(*this, "appendTextTextCtrl", DynSoft::DSTextCtrl);
	mainStatusBar                        = XRCCTRL(*this, "mainStatusBar", wxStatusBar);
	transferBankButton                   = XRCCTRL(*this, "transferBankButton", wxButton);
	findBankButton                       = XRCCTRL(*this, "findBankButton", wxButton);

	KeyValueDatabaseLibrary keyValueLibrary(rit, _N("advanced_preferences"));
	const wxString mediaDefault(keyValueLibrary.Get(_N("orders.media.default_description")).ToString());
	wxPanel *tmpMediaPanel               = XRCCTRL(*this, "mediaPanel", wxPanel);
	mediaBoxSizer                        = static_cast<wxBoxSizer *>(tmpMediaPanel->GetSizer());
	mediaPanel                           = new MediaPanel(tmpMediaPanel, -1, rit, _N("linked_files_orders"), orderEntity ? orderEntity->GetLinkedFileEntities() : NULL, &changed, mediaDefault);
	mediaBoxSizer->Add(mediaPanel, 1, wxEXPAND | wxALL, 5);

	wxPanel *tmpMoreDataPanel            = XRCCTRL(*this, "moreDataPanel", wxPanel);
	moreDataBoxSizer                     = static_cast<wxBoxSizer *>(tmpMoreDataPanel->GetSizer());
	moreDataPanel                        = new MoreDataPanel(tmpMoreDataPanel, -1, rit, orderEntity->GetKeysValuesType(), &changed);
	moreDataBoxSizer->Add(moreDataPanel, 1, wxEXPAND | wxALL, 5);

	ownClientIdTextCtrl->Disable();

	//mediaBoxSizer->RecalcSizes();
	//moreDataBoxSizer->RecalcSizes();
	mainBoxSizer->SetSizeHints(this);

	// Positions
	orderPositionsListCtrl->InsertColumn(0, _("Amount"), wxLIST_FORMAT_RIGHT, 50);
	orderPositionsListCtrl->InsertColumn(1, _("Product id"), wxLIST_FORMAT_RIGHT, 90);
	orderPositionsListCtrl->InsertColumn(2, _("Article number"), wxLIST_FORMAT_RIGHT, 90);
	orderPositionsListCtrl->InsertColumn(3, _("Short name"), wxLIST_FORMAT_LEFT, 120);
	orderPositionsListCtrl->InsertColumn(4, _("Name"), wxLIST_FORMAT_LEFT, 180);
	orderPositionsListCtrl->InsertColumn(5, _("Price"), wxLIST_FORMAT_RIGHT, 90);
	orderPositionsListCtrl->InsertColumn(6, _("Full price"), wxLIST_FORMAT_RIGHT, 90);
	orderPositionsListCtrl->InsertColumn(7, _("Reduction"), wxLIST_FORMAT_RIGHT, 90);
	orderPositionsListCtrl->InsertColumn(8, _("Sum"), wxLIST_FORMAT_RIGHT, 90);
	orderPositionsListCtrl->InsertColumn(9, _("Tax"), wxLIST_FORMAT_RIGHT, 90);
	orderPositionsListCtrl->InsertColumn(10, _("Absolute price"), wxLIST_FORMAT_RIGHT, 90);
	orderPositionsListCtrl->InsertColumn(11, _("Stock"), wxLIST_FORMAT_LEFT, 100);

	// Linked orders
	linkedOrdersListCtrl->InsertColumn(0, _("Order id"), wxLIST_FORMAT_RIGHT, 120);
	linkedOrdersListCtrl->InsertColumn(1, _("Price"), wxLIST_FORMAT_RIGHT, 120);
	linkedOrdersListCtrl->InsertColumn(2, _("Full tax"), wxLIST_FORMAT_RIGHT, 150);
	linkedOrdersListCtrl->InsertColumn(3, _("Reduced tax"), wxLIST_FORMAT_RIGHT, 150);
	linkedOrdersListCtrl->InsertColumn(4, _("Sum"), wxLIST_FORMAT_RIGHT, 120);

	// Fill
	wxArrayString kinds(ConstantsLibrary::OrderKindToArrayString());
	kinds.RemoveAt(kinds.GetCount() - 1);
	kindComboBox->Append(kinds);
	kindComboBox->SetSelection(0);
	
	taxRateComboBox->Append(ConstantsLibrary::TaxRateToArrayString());
	taxRateComboBox->SetSelection(0);

	paymentComboBox->Append(ConstantsLibrary::PaymentToArrayString());
	paymentComboBox->SetSelection(0);

	ListEntryController::FillComboBox(rit, conditionComboBox,                   _N("conditions"),   _N("condition"),    ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, deliveryComboBox,                    _N("delivery"),     _N("delivery"),     ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, shippingAddressTitleComboBox,        _N("titles"),       _N("title"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, shippingAddressSalutationComboBox,   _N("salutations"),  _N("salutation"),   ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, shippingAddressLandComboBox,         _N("lands"),        _N("land"),         ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, shippingAddressFederalStateComboBox, _N("federalstate"), _N("federalstate"), ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, billAddressTitleComboBox,            _N("titles"),       _N("title"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, billAddressSalutationComboBox,       _N("salutations"),  _N("salutation"),   ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, billAddressLandComboBox,             _N("lands"),        _N("land"),         ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, billAddressFederalStateComboBox,     _N("federalstate"), _N("federalstate"), ListEntryController::FILL_NEW, true);

	payedToolbarButton->SetEnabled(false);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, OrderEditFrame::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, OrderEditFrame::SaveEvent);
	CONNECT_TOOLBARBUTTON(findClientToolbarButton, OrderEditFrame::FindClientEvent);
	CONNECT_TOOLBARBUTTON(editedInformationsToolbarButton, OrderEditFrame::ShowEditedInformationsEvent);
	CONNECT_TOOLBARBUTTON(invoicingToolbarButton, OrderEditFrame::InvoicingEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, OrderEditFrame::PrintEvent);
	CONNECT_TOOLBARBUTTON(printPreviewToolbarButton, OrderEditFrame::PrintPreviewEvent);
	CONNECT_TOOLBARBUTTON(payedToolbarButton, OrderEditFrame::PayedEvent);
	CONNECT_TOOLBARBUTTON(remindersToolbarButton, OrderEditFrame::RemindersEvent);

	prepandTextStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(OrderEditFrame::TemplateSelectEvent), NULL, this);
	appendTextStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(OrderEditFrame::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[20];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,              WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,                (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,                (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,                (int) 'i',  EDITED_INFORMATIONS_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,                (int) 'f',  FIND_CLIENT_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL,                (int) 'n',  ADD_POSITION_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL,                (int) 'e',  EDIT_POSITION_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL,                (int) 'd',  REMOVE_POSITION_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL,                (int) 'o',  INVOICING_EVENT);
	acceleratorEntries[9].Set(wxACCEL_CTRL,                (int) 'p',  PRINT_EVENT);
	acceleratorEntries[10].Set(wxACCEL_CTRL,               (int) 'b',  PAYED_EVENT);
	acceleratorEntries[11].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'p',  PRINT_PREVIEW_EVENT);
	acceleratorEntries[12].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'd',  DOWN_POSITION_EVENT);
	acceleratorEntries[13].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'u',  UP_POSITION_EVENT);
	acceleratorEntries[14].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'x',  DOWN_LINKED_ORDER_EVENT);
	acceleratorEntries[15].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'y',  UP_LINKED_ORDER_EVENT);
	acceleratorEntries[16].Set(wxACCEL_CTRL,               (int) 'l',  LINK_ORDER_EVENT);
	acceleratorEntries[17].Set(wxACCEL_CTRL,               (int) 'k',  UNLINK_ORDER_EVENT);
	acceleratorEntries[18].Set(wxACCEL_CTRL,               (int) 'r',  NEGATE_POSITION_PRICES_EVENT);
	acceleratorEntries[19].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'w',  REMINDERS_EVENT);
	wxAcceleratorTable acceleratorTable(20, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Binding
	bind.Bind(_N("kind"), kindComboBox, DSBind::SELECTION);
	bind.Bind(_N("orderId"), orderIdTextCtrl);
	bind.Bind(_N("basedOn"), basedOnTextCtrl);
	bind.Bind(_N("clientId"), clientIdTextCtrl);
	bind.Bind(_N("ownClientId"), ownClientIdTextCtrl);
	bind.Bind(_N("taxNumber"), taxNumberTextCtrl);
	bind.Bind(_N("salesTaxIdentificationNumber"), salesTaxIdentificationNumberTextCtrl);
	bind.Bind(_N("taxRate"), taxRateComboBox, DSBind::SELECTION);
	bind.Bind(_N("when"), dateDatePickerCtrl);
	bind.Bind(_N("owner"), ownerTextCtrl);
	bind.Bind(_N("billAddressSalutation"), billAddressSalutationComboBox);
	bind.Bind(_N("billAddressTitle"), billAddressTitleComboBox);
	bind.Bind(_N("billAddressFirstname"), billAddressFirstnameTextCtrl);
	bind.Bind(_N("billAddressName1"), billAddressName1TextCtrl);
	bind.Bind(_N("billAddressName2"), billAddressName2TextCtrl);
	bind.Bind(_N("billAddressName3"), billAddressName3TextCtrl);
	bind.Bind(_N("billAddressName4"), billAddressName4TextCtrl);
	bind.Bind(_N("billAddressAddress"), billAddressAddressTextCtrl);
	bind.Bind(_N("billAddressNumber"), billAddressNumberTextCtrl);
	bind.Bind(_N("billAddressZipcode"), billAddressZipcodeTextCtrl);
	bind.Bind(_N("billAddressCity"), billAddressCityTextCtrl);
	bind.Bind(_N("billAddressFederalState"), billAddressFederalStateComboBox);
	bind.Bind(_N("billAddressLand"), billAddressLandComboBox);
	bind.Bind(_N("shippingAddressSalutation"), shippingAddressSalutationComboBox);
	bind.Bind(_N("shippingAddressTitle"), shippingAddressTitleComboBox);
	bind.Bind(_N("shippingAddressFirstname"), shippingAddressFirstnameTextCtrl);
	bind.Bind(_N("shippingAddressName1"), shippingAddressName1TextCtrl);
	bind.Bind(_N("shippingAddressName2"), shippingAddressName2TextCtrl);
	bind.Bind(_N("shippingAddressName3"), shippingAddressName3TextCtrl);
	bind.Bind(_N("shippingAddressName4"), shippingAddressName4TextCtrl);
	bind.Bind(_N("shippingAddressAddress"), shippingAddressAddressTextCtrl);
	bind.Bind(_N("shippingAddressNumber"), shippingAddressNumberTextCtrl);
	bind.Bind(_N("shippingAddressZipcode"), shippingAddressZipcodeTextCtrl);
	bind.Bind(_N("shippingAddressCity"), shippingAddressCityTextCtrl);
	bind.Bind(_N("shippingAddressFederalState"), shippingAddressFederalStateComboBox);
	bind.Bind(_N("shippingAddressLand"), shippingAddressLandComboBox);
	bind.Bind(_N("condition"), conditionComboBox);
	bind.Bind(_N("payment"), paymentComboBox, DSBind::SELECTION);
	bind.Bind(_N("bank"), bankTextCtrl);
	bind.Bind(_N("bankCode"), bankCodeTextCtrl);
	bind.Bind(_N("accountNumber"), accountNumberTextCtrl);
	bind.Bind(_N("bic"), bicTextCtrl);
	bind.Bind(_N("iban"), ibanTextCtrl);
	bind.Bind(_N("bankOwner"), bankOwnerTextCtrl);
	bind.Bind(_N("liableToTax"), liableToTaxCheckBox);
	bind.Bind(_N("orderNumber"), orderNumberTextCtrl);
	bind.Bind(_N("delivery"), deliveryComboBox);
	bind.Bind(_N("prepandText"), prepandTextTextCtrl);
	bind.Bind(_N("appendText"), appendTextTextCtrl);
	bind.Bind(_N("monition"), monitionCheckBox);

	if(orderEntity) {
		rit->Block(_N("orders"), orderId, this);
		
		bind.Push();
		if(orderId == 0)
			orderIdTextCtrl->SetValue(_("<automatic>"));

		OrderPositionEntities *orderPositionEntities = orderEntity->GetPositions();
		unsigned int count = orderPositionEntities->GetCount();
		for(unsigned int i = 0; i < count; i++)
			AddOrChangeToOrderPositionsListCtrl(orderPositionEntities->Get(i), true, true);


		OrderEntities *tmpLinkedOrderEntities = orderEntity->GetLinkedOrders();
		count = tmpLinkedOrderEntities->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			AddToLinkedOrdersListCtrl(tmpLinkedOrderEntities->Get(i));
			linkedOrderEntities.Add(tmpLinkedOrderEntities->Get(i));
		}

		clientId = orderEntity->Get(_N("clientId"))->ToUnsignedInt();

		shippingAddressTranslated = orderEntity->Get(_N("shippingAddressTranslated"))->ToString();
		billAddressTranslated     = orderEntity->Get(_N("billAddressTranslated"))->ToString();
		SetTranslatedAddressIcons();

		monitionStateStaticText->SetLabel(_("Monition state: ") + orderEntity->Get(_N("monitionState"))->ToString());
	}

	orderIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	basedOnTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	clientIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	bankCodeTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	accountNumberTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	kindComboBox->SetChange(&changed);
	clientIdTextCtrl->SetChange(&changed);
	taxNumberTextCtrl->SetChange(&changed);
	taxRateComboBox->SetChange(&changed);
	dateDatePickerCtrl->SetChange(&changed);
	ownerTextCtrl->SetChange(&changed);
	billAddressSalutationComboBox->SetChange(&changed);
	billAddressTitleComboBox->SetChange(&changed);
	billAddressFirstnameTextCtrl->SetChange(&changed);
	billAddressName1TextCtrl->SetChange(&changed);
	billAddressName2TextCtrl->SetChange(&changed);
	billAddressName3TextCtrl->SetChange(&changed);
	billAddressName4TextCtrl->SetChange(&changed);
	billAddressAddressTextCtrl->SetChange(&changed);
	billAddressNumberTextCtrl->SetChange(&changed);
	billAddressZipcodeTextCtrl->SetChange(&changed);
	billAddressCityTextCtrl->SetChange(&changed);
	billAddressFederalStateComboBox->SetChange(&changed);
	billAddressLandComboBox->SetChange(&changed);
	shippingAddressSalutationComboBox->SetChange(&changed);
	shippingAddressTitleComboBox->SetChange(&changed);
	shippingAddressFirstnameTextCtrl->SetChange(&changed);
	shippingAddressName1TextCtrl->SetChange(&changed);
	shippingAddressName2TextCtrl->SetChange(&changed);
	shippingAddressName3TextCtrl->SetChange(&changed);
	shippingAddressName4TextCtrl->SetChange(&changed);
	shippingAddressAddressTextCtrl->SetChange(&changed);
	shippingAddressNumberTextCtrl->SetChange(&changed);
	shippingAddressZipcodeTextCtrl->SetChange(&changed);
	shippingAddressCityTextCtrl->SetChange(&changed);
	shippingAddressFederalStateComboBox->SetChange(&changed);
	shippingAddressLandComboBox->SetChange(&changed);
	orderPositionsListCtrl->SetChange(&changed);
	conditionComboBox->SetChange(&changed);
	paymentComboBox->SetChange(&changed);
	bankTextCtrl->SetChange(&changed);
	bankCodeTextCtrl->SetChange(&changed);
	accountNumberTextCtrl->SetChange(&changed);
	bicTextCtrl->SetChange(&changed);
	ibanTextCtrl->SetChange(&changed);
	bankOwnerTextCtrl->SetChange(&changed);
	liableToTaxCheckBox->SetChange(&changed);
	orderNumberTextCtrl->SetChange(&changed);
	deliveryComboBox->SetChange(&changed);
	prepandTextTextCtrl->SetChange(&changed);
	appendTextTextCtrl->SetChange(&changed);
	monitionCheckBox->SetChange(&changed);

	this->editedInformationsDialog = new EditedInformationsDialog(this, -1, rit, orderEntity);
	this->editedInformationsDialog->Show(false);

	clientIdTextCtrl->SetFocus();

	basedOnTextCtrl->SetEditable(false);

	SetPayed();
	EnableClientIdField();
	Transfer();
	SetDisabled();

	changed = false;
	loaded  = true;

	ICON();
}

// Events
void OrderEditFrame::OnClose(wxCloseEvent &event) {
	bool ok = true;
	if(changed) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to save the changes?"),
			_("Close"),
			_("Save"),
			_("Cancel")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES)
			ok = Save();
		yesNoDialog->Destroy();
	}

	if(ok) {
		OrderEntity *orderEntity = static_cast<OrderEntity *>(bind.GetEntity());
		if(deleteOrderEntity)
			delete orderEntity;
		rit->Unblock(_N("orders"), orderId);

		Destroy();
		event.Skip();
	} else {
		event.Veto();
	}
}

void OrderEditFrame::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void OrderEditFrame::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void OrderEditFrame::RemindersEvent(wxMouseEvent &event) {
	Reminders();
	SKIP();
}

void OrderEditFrame::PrintEvent(wxMouseEvent &event) {
	Print();
	SKIP();
}

void OrderEditFrame::PrintPreviewEvent(wxMouseEvent &event) {
	Print(true);
	SKIP();
}

void OrderEditFrame::PrintOutEvent(wxCommandEvent &event) {
	const bool delivery = event.GetId() == PRINT_DELIVERY_EVENT;
	PrintOut(delivery);
}

void OrderEditFrame::FindClientEvent(wxMouseEvent &event) {
	FindClient();
	SKIP();
}

void OrderEditFrame::DownPositionEvent(wxCommandEvent &event) {
	DownPosition();
}

void OrderEditFrame::UpPositionEvent(wxCommandEvent &event) {
	UpPosition();
}

void OrderEditFrame::AddPositionEvent(wxCommandEvent &event) {
	AddPosition();
}

void OrderEditFrame::EditPositionEvent(wxCommandEvent &event) {
	EditPosition(static_cast<OrderPositionEntity *>(orderPositionsListCtrl->GetSingleSelectionPointer()));
}

void OrderEditFrame::DirectEditPositionEvent(wxListEvent &event) {
	EditPosition(static_cast<OrderPositionEntity *>(orderPositionsListCtrl->GetSingleSelectionPointer()));
}

void OrderEditFrame::RemovePositionEvent(wxCommandEvent &event) {
	RemovePosition(static_cast<OrderPositionEntity *>(orderPositionsListCtrl->GetSingleSelectionPointer()));
}

void OrderEditFrame::TransferAddressEvent(wxCommandEvent &event) {
	TransferAddress(event.GetId());
}

void OrderEditFrame::TransferBankEvent(wxCommandEvent &event) {
	TransferBank();
}

void OrderEditFrame::ShowEditedInformationsEvent(wxMouseEvent &event) {
	ShowEditedInformations();
	SKIP();
}

void OrderEditFrame::InvoicingEvent(wxMouseEvent &event) {
	Invoicing();
	SKIP();
}

void OrderEditFrame::PayedEvent(wxMouseEvent &event) {
	Payed();
	SKIP();
}

void OrderEditFrame::DownLinkedOrderEvent(wxCommandEvent &event) {
	DownLinkedOrder();
}

void OrderEditFrame::UpLinkedOrderEvent(wxCommandEvent &event) {
	UpLinkedOrder();
}

void OrderEditFrame::UnlinkOrderEvent(wxCommandEvent &event) {
	UnlinkOrder();
}

void OrderEditFrame::LinkOrderEvent(wxCommandEvent &event) {
	LinkOrder();
}

void OrderEditFrame::PayedKeyEvent(wxCommandEvent &event) {
	Payed();
}

void OrderEditFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void OrderEditFrame::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void OrderEditFrame::RemindersKeyEvent(wxCommandEvent &event) {
	Reminders();
}

void OrderEditFrame::PrintKeyEvent(wxCommandEvent &event) {
	Print();
}

void OrderEditFrame::PrintPreviewKeyEvent(wxCommandEvent &event) {
	Print(true);
}

void OrderEditFrame::ShowEditedInformationsKeyEvent(wxCommandEvent &event) {
	ShowEditedInformations();
}

void OrderEditFrame::FindClientKeyEvent(wxCommandEvent &event) {
	FindClient();
}

void OrderEditFrame::DownPositionKeyEvent(wxCommandEvent &event) {
	DownPosition();
}

void OrderEditFrame::UpPositionKeyEvent(wxCommandEvent &event) {
	UpPosition();
}

void OrderEditFrame::AddPositionKeyEvent(wxCommandEvent &event) {
	AddPosition();
}

void OrderEditFrame::EditPositionKeyEvent(wxCommandEvent &event) {
	EditPosition(static_cast<OrderPositionEntity *>(orderPositionsListCtrl->GetSingleSelectionPointer()));
}

void OrderEditFrame::RemovePositionKeyEvent(wxCommandEvent &event) {
	RemovePosition(static_cast<OrderPositionEntity *>(orderPositionsListCtrl->GetSingleSelectionPointer()));
}

void OrderEditFrame::InvoicingKeyEvent(wxCommandEvent &event) {
	Invoicing();
}

void OrderEditFrame::ClientIdEvent(wxCommandEvent &event) {
	ClientId(DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue()));
}

void OrderEditFrame::RecalcPositionsEvent(wxCommandEvent &event) {
	Transfer();
}

void OrderEditFrame::BankFindEvent(wxCommandEvent &event) {
	BankFind();
}

void OrderEditFrame::DownLinkedOrderKeyEvent(wxCommandEvent &event) {
	DownLinkedOrder();
}

void OrderEditFrame::UpLinkedOrderKeyEvent(wxCommandEvent &event) {
	UpLinkedOrder();
}

void OrderEditFrame::UnlinkOrderKeyEvent(wxCommandEvent &event) {
	UnlinkOrder();
}

void OrderEditFrame::LinkOrderKeyEvent(wxCommandEvent &event) {
	LinkOrder();
}

void OrderEditFrame::NegatePositionPricesEvent(wxCommandEvent &event) {
	NegatePositionPrices();
}

void OrderEditFrame::NegatePositionPricesKeyEvent(wxCommandEvent &event) {
	NegatePositionPrices();
}

void OrderEditFrame::AddressTranslateEvent(wxCommandEvent &event) {
	AddressTranslate(event.GetId());
}

void OrderEditFrame::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

// Methods
void OrderEditFrame::Close() {
	wxWindow::Close();
}

bool OrderEditFrame::Save() {
	clientIdTextCtrl->SetFocus();

	if(IfInvoicedShowThis())
		return false;
	
	if(IfLinkedShowThis())
		return false;

	if(!rit->Access(A_WRITE, _("You have no permission to save this order."), this))
		return false;

	ClientController clientController(rit);
	if(!clientController.Exists(_N("clientId"), clientIdTextCtrl->GetValue())) {
		rit->Log(true, _N("OrderEditFrame"), _("The order is not saved."), _("The client id you entered does not exist \"") + clientIdTextCtrl->GetValue() + _N("\"."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The client id you entered does not exist."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	if(!clientController.IsActive(DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue()))) {
		rit->Log(true, _N("OrderEditFrame"), _("The order is not saved."), _("The client id you entered is not active \"") + clientIdTextCtrl->GetValue() + _N("\"."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The client id you entered is not active."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	if(
		billAddressName1TextCtrl->GetValue().IsEmpty() ||
		billAddressAddressTextCtrl->GetValue().IsEmpty() ||
		billAddressZipcodeTextCtrl->GetValue().IsEmpty() ||
		billAddressCityTextCtrl->GetValue().IsEmpty()
	) {
		rit->Log(true, _N("OrderEditFrame"), _("The order is not saved."), _("Not all necessary fields are filled out."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The following fields are necessary: Name 1, address, zipcode, city (all bill address), and payment"),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	OrderEntity *orderEntity = static_cast<OrderEntity *>(bind.GetEntity());
	if(!orderEntity) {
		rit->Log(true, _N("OrderEditFrame"), _("The order is not saved."), _("Null Pointer."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The order is not saved. Null Pointer."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	Transfer(orderEntity);
	wxArrayString products, linkedOrders, allreadyLinkedOrders;
	bool isValidKind = false;
	const bool valid = orderEntity->IsValid(&products, &linkedOrders, &allreadyLinkedOrders, &isValidKind);

	if(!valid) {
		wxString message;
		if(products.GetCount() > 0)
			message += _("The following products do not exist any more: ") + Tools::ArrayStringToCommaSeparatedList(products) + _N(". ");
		if(linkedOrders.GetCount() > 0)
			message += _("The following orders do not exist any more: ") + Tools::ArrayStringToCommaSeparatedList(linkedOrders) + _N(". ");
		if(allreadyLinkedOrders.GetCount() > 0)
			message += _("The following orders are linked and cannot be linked again: ") + Tools::ArrayStringToCommaSeparatedList(allreadyLinkedOrders) + _N(". ");
		if(!isValidKind)
			message += _("The order must be of kind oder. ");

		rit->Log(true, _N("OrderEditFrame"), _("The order is not saved."), message);
		
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			message,
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	OrderController orderController(rit);
	const bool ok = orderController.Save(orderEntity, &orderPositionsRemoved);
	if(ok) {
		orderId = orderEntity->Get(_N("orderId"))->ToUnsignedInt();
		orderController.Get(DSCasts::ToString(orderId)); // Autofelder refreshen
		orderController.Run();
		orderController.Get(orderEntity, true);
		orderController.Reset();

		bind.Push();
		orderIdTextCtrl->SetValue(DSCasts::ToString(orderId));

		if(callbackInterface)
			callbackInterface->Callback(static_cast<void *>(orderEntity));

		rit->Block(_N("orders"), orderId, this);

		changed = false;

		rit->Log(false, _N("OrderEditFrame"), _("The order is saved."), _("The order with order id \"") + DSCasts::ToString(orderId) + _N("\" is saved."));

		editedInformationsDialog->Update();
	} else {
		rit->Log(true, _N("OrderEditFrame"), _("The order is not saved."), _("Database error."));

		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not save."),
			_("Save order")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}

	return ok;
}

void OrderEditFrame::FindClient() {
	clientIdTextCtrl->SetFocus();

	if(IfInvoicedShowThis())
		return;
	
	if(IfLinkedShowThis())
		return;

	ClientsFullFindDialog *clientsFullFindDialog = new ClientsFullFindDialog(this, -1, rit);
	clientsFullFindDialog->Center();
	if(clientsFullFindDialog->ShowModal() == wxID_OK) {
		ClientEntity *clientEntity = clientsFullFindDialog->GetClientEntity();
		if(clientEntity)
			clientIdTextCtrl->SetValue(clientEntity->Get(_N("clientId"))->ToString());
	}
}

void OrderEditFrame::AddPosition() {
	orderPositionsListCtrl->SetFocus();

	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to add a position."), this))
		return;

	DSCurrency discount;
	ClientEntity *clientEntity = GetClientEntity(DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue()));
	if(clientEntity) {
		discount = clientEntity->Get(_N("discount"))->ToCurrency();
		delete clientEntity;
	}
	OrderPositionEntity *orderPositionEntity = new OrderPositionEntity(rit);
	orderPositionEntity->Set(_N("reduction"), discount);
	OrderPositionEditDialog *orderPositionEditDialog = new OrderPositionEditDialog(
		this,
		-1,
		rit,
		orderPositionEntity,
		taxRateComboBox->GetSelection()
	);
	orderPositionEditDialog->Center();
	if(orderPositionEditDialog->ShowModal() == wxID_OK) {
		AddOrChangeToOrderPositionsListCtrl(orderPositionEntity, false);
		Transfer();
	} else {
		delete orderPositionEntity;
	}
}

void OrderEditFrame::EditPosition(OrderPositionEntity *orderPositionEntity) {
	orderPositionsListCtrl->SetFocus();

	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to edit a position."), this))
		return;

	if(!orderPositionEntity) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select an entry."),
			_("Edit position")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}
	DSCurrency discount;
	ClientEntity *clientEntity = GetClientEntity(DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue()));
	if(clientEntity) {
		discount = clientEntity->Get(_N("discount"))->ToCurrency();
		delete clientEntity;
	}
	OrderPositionEditDialog *orderPositionEditDialog = new OrderPositionEditDialog(
		this,
		-1,
		rit,
		orderPositionEntity,
		taxRateComboBox->GetSelection()
	);
	orderPositionEditDialog->Center();
	if(orderPositionEditDialog->ShowModal() == wxID_OK) {
		AddOrChangeToOrderPositionsListCtrl(orderPositionEntity, false);
	}
}

void OrderEditFrame::RemovePosition(OrderPositionEntity *orderPositionEntity) {
	orderPositionsListCtrl->SetFocus();

	if(IfInvoicedShowThis())
		return;
		
	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to remove a position."), this))
		return;

	if(!orderPositionEntity) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select an entry."),
			_("Edit position")
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
		_("Do you really want to remove this position?"),
		_("Remove position"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		const unsigned int orderPositionId = orderPositionEntity->Get(_N("orderPositionId"))->ToUnsignedInt();
		if(orderPositionId != 0)
			orderPositionsRemoved.Add(DSCasts::ToString(orderPositionId));
		orderPositionsListCtrl->DeleteItem(orderPositionsListCtrl->GetPosition(orderPositionEntity));

		OrderEntity *orderEntity = static_cast<OrderEntity *>(bind.GetEntity());
		if(orderEntity)
			orderEntity->GetPositions()->Remove(orderPositionEntity, true);
	}
	yesNoDialog->Destroy();

	Transfer();
}

void OrderEditFrame::AddOrChangeToOrderPositionsListCtrl(OrderPositionEntity *orderPositionEntity, const bool justAppend, const bool dontRecalcPositions) {
	if(!justAppend) { // TODO Was macht die Schleife sonst außer nix??
		const unsigned int count = orderPositionsListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			if(static_cast<OrderPositionEntity *>(orderPositionsListCtrl->GetPointer(i, 0)) == orderPositionEntity)
				break;
		}
	}

	OrderEntity *orderEntity = static_cast<OrderEntity *>(bind.GetEntity());
	orderEntity->SetValue(_N("taxRate"), taxRateComboBox->GetSelection());
	orderEntity->Recalc();

	DSCurrency amount(orderPositionEntity->Get(_N("amount"))->ToCurrency());
	DSCurrency price(orderPositionEntity->Get(_N("price"))->ToCurrency());
	DSCurrency fullPrice(price * amount);
	DSCurrency reduction(orderPositionEntity->Get(_N("reduction"))->ToCurrency());
	DSCurrency beforeTax(orderPositionEntity->Get(_N("beforeTax"))->ToCurrency());
	DSCurrency tax(orderPositionEntity->Get(_N("tax"))->ToCurrency());
	DSCurrency afterTax(orderPositionEntity->Get(_N("afterTax"))->ToCurrency());

	DSListCtrl::DSListCtrlRow *orderPositionsListCtrlRow = new DSListCtrl::DSListCtrlRow(orderPositionEntity);
	orderPositionsListCtrlRow->Add(amount.ToString());
	orderPositionsListCtrlRow->Add(orderPositionEntity->Get(_N("productId"))->ToString());
	orderPositionsListCtrlRow->Add(orderPositionEntity->Get(_N("articleNumber"))->ToString());
	orderPositionsListCtrlRow->Add(orderPositionEntity->Get(_N("shortName"))->ToString());
	orderPositionsListCtrlRow->Add(orderPositionEntity->Get(_N("name1"))->ToString());
	orderPositionsListCtrlRow->Add(price.ToString());
	orderPositionsListCtrlRow->Add(fullPrice.ToString());
	orderPositionsListCtrlRow->Add(reduction.ToString());
	orderPositionsListCtrlRow->Add(beforeTax.ToString());
	orderPositionsListCtrlRow->Add(tax.ToString());
	orderPositionsListCtrlRow->Add(afterTax.ToString());
	orderPositionsListCtrlRow->Add(orderPositionEntity->Get(_N("stock"))->ToString());

	orderPositionsListCtrl->Set(orderPositionsListCtrlRow, true);

	if(!dontRecalcPositions)
		Transfer();

	changed = true;
}

void OrderEditFrame::ChooseClientMessage() {
	wxMessageDialog *errorMessageDialog = new wxMessageDialog(
		this,
		_("Please enter a valid client id."),
		_("Error")
	);
	errorMessageDialog->Center();
	errorMessageDialog->ShowModal();
	errorMessageDialog->Destroy();
}

void OrderEditFrame::TransferAddress(const int objectId) {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	const unsigned int clientId = DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue());
	if(!clientId) {
		ChooseClientMessage();
		return;
	}
	
	ClientEntity *clientEntity = GetClientEntity(clientId);
	if(clientEntity) {
		const bool active = clientEntity->Get(_N("active"))->ToBool();
		delete clientEntity;

		if(!IsClientActive(active))
			return;

		wxComboBox *salutationComboBox   = NULL;
		wxComboBox *titleComboBox        = NULL;
		wxTextCtrl *firstnameTextCtrl    = NULL;
		wxTextCtrl *name1TextCtrl        = NULL;
		wxTextCtrl *name2TextCtrl        = NULL;
		wxTextCtrl *name3TextCtrl        = NULL;
		wxTextCtrl *name4TextCtrl        = NULL;
		wxTextCtrl *addressTextCtrl      = NULL;
		wxTextCtrl *numberTextCtrl       = NULL;
		wxTextCtrl *zipcodeTextCtrl      = NULL;
		wxTextCtrl *cityTextCtrl         = NULL;
		wxComboBox *federalStateComboBox = NULL;
		wxComboBox *landComboBox         = NULL;

		if(objectId == XRCID("billAddressTransferButton")) {
			salutationComboBox   = billAddressSalutationComboBox;
			titleComboBox        = billAddressTitleComboBox;
			firstnameTextCtrl    = billAddressFirstnameTextCtrl;
			name1TextCtrl        = billAddressName1TextCtrl;
			name2TextCtrl        = billAddressName2TextCtrl;
			name3TextCtrl        = billAddressName3TextCtrl;
			name4TextCtrl        = billAddressName4TextCtrl;
			addressTextCtrl      = billAddressAddressTextCtrl;
			numberTextCtrl       = billAddressNumberTextCtrl;
			zipcodeTextCtrl      = billAddressZipcodeTextCtrl;
			cityTextCtrl         = billAddressCityTextCtrl;
			federalStateComboBox = billAddressFederalStateComboBox;
			landComboBox         = billAddressLandComboBox;
		} else if(objectId == XRCID("shippingAddressTransferButton")) {
			salutationComboBox   = shippingAddressSalutationComboBox;
			titleComboBox        = shippingAddressTitleComboBox;
			firstnameTextCtrl    = shippingAddressFirstnameTextCtrl;
			name1TextCtrl        = shippingAddressName1TextCtrl;
			name2TextCtrl        = shippingAddressName2TextCtrl;
			name3TextCtrl        = shippingAddressName3TextCtrl;
			name4TextCtrl        = shippingAddressName4TextCtrl;
			addressTextCtrl      = shippingAddressAddressTextCtrl;
			numberTextCtrl       = shippingAddressNumberTextCtrl;
			zipcodeTextCtrl      = shippingAddressZipcodeTextCtrl;
			cityTextCtrl         = shippingAddressCityTextCtrl;
			federalStateComboBox = shippingAddressFederalStateComboBox;
			landComboBox         = shippingAddressLandComboBox;
		}

		AddressEntity *addressEntity = NULL;
		ClientController clientController(rit);
		AddressEntities *addressEntities = clientController.GetAddresses(clientId);
		if(addressEntities->GetCount() == 1) {
			addressEntity = addressEntities->Get(0);
		} else {
			AddressSelectDialog *addressSelectDialog = new AddressSelectDialog(
				this,
				-1,
				rit,
				*addressEntities
			);
			addressSelectDialog->Center();
			if(addressSelectDialog->ShowModal() == wxID_OK)
				addressEntity = addressSelectDialog->GetSelectedAddress();
			addressSelectDialog->Destroy();
		}

		if(addressEntity) {
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
		}

		addressEntities->Clear(true);
		delete addressEntities;
	}
}

void OrderEditFrame::TransferBank() {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	const unsigned int clientId = DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue());
	if(!clientId) {
		ChooseClientMessage();
		return;
	}

	ClientEntity *clientEntity = GetClientEntity(clientId);
	if(clientEntity) {
		if(!IsClientActive(clientEntity->Get(_N("active"))->ToBool()))
			return;
	
		if(clientEntity->GetBankConnectionEntities()->GetCount() == 0) {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("No bank connections available."),
				_("Choose bank")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
			return;
		}

		BankConnectionSelectDialog *bankConnectionSelectDialog = new BankConnectionSelectDialog(
			this,
			-1,
			rit,
			*clientEntity->GetBankConnectionEntities()
		);
		bankConnectionSelectDialog->Center();
		if(bankConnectionSelectDialog->ShowModal() == wxID_OK) {
			BankConnectionEntity *bankConnectionEntity = bankConnectionSelectDialog->GetSelectedBankConnection();

			if(bankConnectionEntity) {
				bankTextCtrl->SetValue(bankConnectionEntity->Get(_N("bank"))->ToString());
				bankCodeTextCtrl->SetValue(bankConnectionEntity->Get(_N("bankCode"))->ToString());
				accountNumberTextCtrl->SetValue(bankConnectionEntity->Get(_N("accountNumber"))->ToString());
				bicTextCtrl->SetValue(bankConnectionEntity->Get(_N("bic"))->ToString());
				ibanTextCtrl->SetValue(bankConnectionEntity->Get(_N("iban"))->ToString());
				bankOwnerTextCtrl->SetValue(bankConnectionEntity->Get(_N("bankOwner"))->ToString());
				deliveryComboBox->SetFocus();
			}
		}
		bankConnectionSelectDialog->Destroy();
	}
}

void OrderEditFrame::ShowEditedInformations() {
	editedInformationsDialog->Get(!editedInformationsDialog->IsShown());
}

void OrderEditFrame::Invoicing() {
	orderIdTextCtrl->SetFocus();

	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to invoice this order."), this))
		return;

	if(kindComboBox->GetSelection() <= 0) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("You just can invoice orders and credits."),
			_("Invoicing")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	}

	if(changed || orderId == 0) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please save first this order."),
			_("Invoicing")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	}

	InvoiceDialog *invoiceDialog = new InvoiceDialog(this, -1, rit);
	invoiceDialog->Center();
	if(invoiceDialog->ShowModal() == wxID_OK) {
		OrderEntity *orderEntity = static_cast<OrderEntity *>(bind.GetEntity());
		OrderController orderController(rit);
		if(orderController.Invoice(orderEntity, invoiceDialog->GetValue())) {
			wxMessageDialog *messageDialog = new wxMessageDialog(
				this,
				_("Invoice (") + orderEntity->Get(_N("invoiceId"))->ToString() + _(") created."),
				_("Invoicing")
			);
			messageDialog->Center();
			messageDialog->ShowModal();
			messageDialog->Destroy();
			SetDisabled();
			callbackInterface->Callback(static_cast<void *>(orderEntity));
		} else {
			wxMessageDialog *errorDialog = new wxMessageDialog(
				this,
				_("Could not create invoice. Unknown error."),
				_("Invoicing")
			);
			errorDialog->Center();
			errorDialog->ShowModal();
			errorDialog->Destroy();
		}
	}
	invoiceDialog->Destroy();
}

void OrderEditFrame::Print(const bool preview) {
	orderIdTextCtrl->SetFocus();

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

	this->preview = preview;
	if(JasperReportsLibrary::UseReports(rit)) {
		PrintOut(false);
	} else {
		wxMenu popupMenu;
		popupMenu.Append(PRINT_NORMAL_EVENT, _("Normal"));
		popupMenu.Append(PRINT_DELIVERY_EVENT, _("Delivery"));
		PopupMenu(&popupMenu);
	}
}

bool OrderEditFrame::IsClientActive(const bool active) {
	if(active)
		return true;

	wxMessageDialog *errorMessageDialog = new wxMessageDialog(
		this,
		_("The client you entered is inactive. You cannot use inactive clients."),
		_("Client not active")
	);
	errorMessageDialog->Center();
	errorMessageDialog->ShowModal();
	errorMessageDialog->Destroy();

	return false;
}

void OrderEditFrame::ClientId(const unsigned int clientId) {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!loaded)
		return;

	this->clientId = clientId;
	if(clientId != 0) {
		ClientEntity *clientEntity = GetClientEntity(clientId);
		if(clientEntity) {
			if(IsClientActive(clientEntity->Get(_N("active"))->ToBool())) {
				liableToTaxCheckBox->SetValue(clientEntity->Get(_N("liableToTax"))->ToBool());
				taxNumberTextCtrl->SetValue(clientEntity->Get(_N("taxNumber"))->ToString());
				salesTaxIdentificationNumberTextCtrl->SetValue(clientEntity->Get(_N("salesTaxIdentificationNumber"))->ToString());
				conditionComboBox->SetValue(clientEntity->Get(_N("condition"))->ToString());
				monitionCheckBox->SetValue(clientEntity->Get(_N("monitionable"))->ToBool());
			}
			delete clientEntity;
		}
	}
}

void OrderEditFrame::SetDisabled() {
	OrderEntity *orderEntity = static_cast<OrderEntity *>(bind.GetEntity());
	if(orderEntity->Get(_N("invoiceId"))->ToUnsignedInt() != 0 || orderEntity->Get(_N("linkedOrderId"))->ToUnsignedInt() != 0) {
		bind.EnableAll(false);
		billAddressTransferButton->Disable();
		shippingAddressTransferButton->Disable();
		saveToolbarButton->SetEnabled(false);
		findClientToolbarButton->SetEnabled(false);
		upPositionButton->Disable();
		downPositionButton->Disable();
		addPositionButton->Disable();
		editPositionButton->Disable();
		removePositionButton->Disable();
		invoicingToolbarButton->SetEnabled(false);
		payedToolbarButton->SetEnabled(true);
		transferBankButton->Disable();
		findBankButton->Disable();
		downLinkedOrderButton->Disable();
		upLinkedOrderButton->Disable();
		unlinkOrderButton->Disable();
		linkOrderButton->Disable();
		negatePositionPricesButton->Disable();
		mediaPanel->SetEditable(false);
		moreDataPanel->SetEditable(false);

		const wxString linkedOrderId(orderEntity->Get(_N("linkedOrderId"))->ToString());
		const wxString invoiceId(orderEntity->Get(_N("invoiceId"))->ToString());
		const wxString invoiceDate(orderEntity->Get(_N("invoiceDate"))->ToString());
		if(!linkedOrderId.IsEmpty())
			mainStatusBar->SetStatusText(_("Linked to: ") + linkedOrderId);
		else if(!invoiceId.IsEmpty())
			mainStatusBar->SetStatusText(_("Invoice: ") + invoiceId + _N(" ") + invoiceDate);

		orderPositionsListCtrl->SetFocus();
	}
}

bool OrderEditFrame::IfInvoicedShowThis() {
	if(!loaded) // Events will be fired on loading, so we must stop use this
		return false;
	
	if(!IsInvoiced())
		return false;
	
	wxMessageDialog *invoicedMessageDialog = new wxMessageDialog(
		this,
		_("This order is invoiced. You are not able to change any fields."),
		_("Invoiced")
	);
	invoicedMessageDialog->Center();
	invoicedMessageDialog->ShowModal();
	invoicedMessageDialog->Destroy();

	SetDisabled();

	return true;
}

bool OrderEditFrame::IfLinkedShowThis() {
	if(!loaded) // Events will be fired on loading, so we must stop use this
		return false;
	
	Transfer();
	OrderEntity *orderEntity = static_cast<OrderEntity *>(bind.GetEntity());

	OrderController orderController(rit);
	if(orderController.IsLinked(orderId, orderEntity->GetLinkedOrders())) {
		wxMessageDialog *invoicedMessageDialog = new wxMessageDialog(
			this,
			_("This order is linked (") + orderEntity->Get(_N("linkedOrderId"))->ToString() + _N("). You are not able to change any fields."),
			_("Linked")
		);
		invoicedMessageDialog->Center();
		invoicedMessageDialog->ShowModal();
		invoicedMessageDialog->Destroy();

		SetDisabled();

		return true;
	}

	return false;
}

ClientEntity *OrderEditFrame::GetClientEntity(const unsigned int clientId) {
	ClientEntity *clientEntity = NULL;
	ClientController clientController(rit);
	clientController.Get(DSCasts::ToString(clientId));
	if(clientController.Run()) {
		clientEntity = new ClientEntity(rit);
		clientController.Get(clientEntity);
		clientController.Reset();
	}

	return clientEntity;
}

void OrderEditFrame::PrintOut(const bool delivery) {
	unsigned int prints        = 0;
	OrderEntity *orderEntity   = static_cast<OrderEntity *>(bind.GetEntity());
	const unsigned int orderId = orderEntity->Get(_N("orderId"))->ToUnsignedInt();
	bool asCopy                = false;

	OrderController orderController(rit);
	
	const unsigned int kind = orderEntity->Get(_N("kind"))->ToUnsignedInt();
	if(!delivery) {
		switch(kind) {
			case ConstantsLibrary::ORDER_KIND_ORDER:
			case ConstantsLibrary::ORDER_KIND_CREDIT:
				prints = orderController.GetPrints(orderId);
				break;
		}
	}

	if(prints > 0) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_(
				"You already printed this order. Now you only should print a copy,"
				"because the should be only one original version.\n\n"
				"Do you want to print this order as copy?"
			),
			_("Print order"),
			_("Print"),
			_("Cancel")
		);
		yesNoDialog->Center();
		asCopy = yesNoDialog->ShowModal() == wxID_YES;
		yesNoDialog->Destroy();
	}

	bool printed = false;

	if(!JasperReportsLibrary::DoReport(
		this,
		rit,
		_N("orders"),
		orderId,
		preview ? JasperReportsController::SHOW : JasperReportsController::PRINT
	)) {
		HtmlPrintoutLibrary *htmlPrintoutLibrary = orderController.Print(orderEntity, delivery, asCopy);

		if(preview) {
			PrintPreviewDialog *printPreviewDialog = new PrintPreviewDialog(this, -1, rit, htmlPrintoutLibrary);
			printPreviewDialog->Center();
			printPreviewDialog->ShowModal();
			printed = printPreviewDialog->Printed();
			printPreviewDialog->Destroy();
		} else {
			wxPrinter printer;
			printed = printer.Print(this, htmlPrintoutLibrary, true);
		}

		delete htmlPrintoutLibrary;
	}

	if(printed)
		orderController.IncrementPrints(orderId);
}

void OrderEditFrame::BankFind() {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

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
			accountNumberTextCtrl->SetFocus();
		}
	}

	banksFullFindDialog->Destroy();
}

void OrderEditFrame::Payed() {
	if(!IsInvoiced())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to set this order as payed."), this))
		return;
	
	OrderController orderController(rit);
	wxString payed(orderController.GetPayed(orderId));

	bool ok = false;
	DSDateTime payedTime;

	if(payed.IsEmpty()) {
		PayedDialog *payedDialog = new PayedDialog(
			this,
			-1,
			rit,
			_("Set payed")
		);
		payedDialog->Center();
		ok = payedDialog->ShowModal() == wxID_OK;
		payedTime = payedDialog->GetValue();
		payedDialog->Destroy();
	} else {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you want to set this order unpayed?"),
			_("Order"),
			_("unpayed"),
			_("Cancel")
		);
		yesNoDialog->Center();
		ok = yesNoDialog->ShowModal() == wxID_YES;
		yesNoDialog->Destroy();
	}
	
	if(ok) {
		if(orderController.SetPayed(orderId, payed.IsEmpty(), payedTime)) {
			rit->Log(false, _N("OrderEditFrame"), _("The order is set as payed."), _("The order \"") + DSCasts::ToString(orderId) + _("\" is set as payed."));

			if(callbackInterface) {
				orderController.Get(DSCasts::ToString(orderId));
				if(orderController.Run()) {
					OrderEntity orderEntity(rit);
					orderController.Get(&orderEntity);
					orderController.Reset();
					callbackInterface->Callback(static_cast<void *>(&orderEntity));
				}
			}
		} else {
			rit->Log(true, _N("OrderEditFrame"), _("The order is not set as payed."), _("The order \"") + DSCasts::ToString(orderId) + _("\" is not set as payed."));
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("The order could not be set as payed. Database error."),
				_("Error")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		}
	}

	SetPayed();
}

void OrderEditFrame::SetPayed() {
	OrderController orderController(rit);
	wxString payed(orderController.GetPayed(orderId));
	payed = payed.Mid(0, payed.Find(_N(" ")));
	if(payed.IsEmpty()) {
		mainStatusBar->SetStatusText(_("Unpayed"), 2);
		payedToolbarButton->SetToggled(false);
	} else {
		mainStatusBar->SetStatusText(_("Payed: ") + payed, 2);
		payedToolbarButton->SetToggled(true);
	}
}

void OrderEditFrame::DownPosition() {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to change the index."), this))
		return;

	if(orderPositionsListCtrl->SelectedItemDown())
		changed = true;
}

void OrderEditFrame::UpPosition() {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to change the index."), this))
		return;

	if(orderPositionsListCtrl->SelectedItemUp())
		changed = true;
}

void OrderEditFrame::DownLinkedOrder() {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to change the index."), this))
		return;

	if(linkedOrdersListCtrl->SelectedItemDown())
		changed = true;
}

void OrderEditFrame::UpLinkedOrder() {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to change the index."), this))
		return;

	if(linkedOrdersListCtrl->SelectedItemUp())
		changed = true;
}

void OrderEditFrame::UnlinkOrder() {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to unlink orders."), this))
		return;
	
	OrderEntity *orderEntity = static_cast<OrderEntity *>(linkedOrdersListCtrl->GetSingleSelectionPointer());
	if(!orderEntity) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the order you want to unlink."),
			_("Unlink order")
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
		_("Do you really want to unlink the selected order?"),
		_("Unlink order"),
		_("Unlink"),
		_("Cancel")
	);
	yesNoDialog->Center();
	const bool ok = yesNoDialog->ShowModal() == wxID_YES;
	yesNoDialog->Destroy();

	if(ok) {
		AddToRemovedOrderArray(orderEntity->Get(_N("orderId"))->ToUnsignedInt());
		linkedOrdersListCtrl->DeleteItem(linkedOrdersListCtrl->GetSingleSelectionPosition());
		linkedOrderEntities.Remove(orderEntity, true);
		EnableClientIdField();
	}
}

void OrderEditFrame::LinkOrder() {
	if(IfInvoicedShowThis())
		return;

	if(IfLinkedShowThis())
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to open orders."), this))
		return;

	if(clientId == 0) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a client."),
			_("Orders")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	if(!clientOrdersDialog) {
		clientOrdersDialog = new ClientOrdersDialog(
			this, // parent
			-1, // id
			rit, // RuntimeInformationType
			clientId, // clientId
			true, // showUse
			this, // callbackInterface
			orderId, // exclude
			&linkedOrderEntities, // excludes
			&removedOrderArray,
			false, // showLinkedOrders
			true
		);
		clientOrdersDialog->Center();
	}

	clientOrdersDialog->SetClientId(clientId);
	clientOrdersDialog->SetExclude(orderId);
	clientOrdersDialog->Show(!clientOrdersDialog->IsShown());
}

void OrderEditFrame::Callback(void *data) {
	if(!data)
		return;

	const unsigned int orderId = *static_cast<const unsigned int *>(data);

	OrderController orderController(rit);
	orderController.Get(DSCasts::ToString(orderId));
	if(orderController.Run()) {
		OrderEntity *linkedOrderEntity = new OrderEntity(rit);
		orderController.Get(linkedOrderEntity);
		orderController.Reset();

		AddToLinkedOrdersListCtrl(linkedOrderEntity);
		linkedOrderEntities.Add(linkedOrderEntity);
		RemoveFromRemovedOrderArray(orderId);

		clientOrdersDialog->RemoveSelected();
	} else {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not find selected order in database. Maybe it was deleted by another person."),
			_("Error")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}

	Transfer();

	EnableClientIdField();
}

void OrderEditFrame::AddToLinkedOrdersListCtrl(OrderEntity *linkedOrderEntity) {
	DSListCtrl::DSListCtrlRow *linkedOrdersListCtrlRow = new DSListCtrl::DSListCtrlRow(linkedOrderEntity);

	linkedOrdersListCtrlRow->Add(linkedOrderEntity->Get(_N("orderId"))->ToString());
	linkedOrdersListCtrlRow->Add(linkedOrderEntity->Get(_N("beforeTax"))->ToCurrency().ToString());
	linkedOrdersListCtrlRow->Add(linkedOrderEntity->Get(_N("fullTax"))->ToCurrency().ToString());
	linkedOrdersListCtrlRow->Add(linkedOrderEntity->Get(_N("reducedTax"))->ToCurrency().ToString());
	linkedOrdersListCtrlRow->Add(linkedOrderEntity->Get(_N("afterTax"))->ToCurrency().ToString());

	linkedOrdersListCtrl->Set(linkedOrdersListCtrlRow, true);
}

void OrderEditFrame::EnableClientIdField() {
	OrderEntity *orderEntity = static_cast<OrderEntity *>(bind.GetEntity());
	if(orderEntity)
		clientIdTextCtrl->SetEditable(orderEntity->GetLinkedOrders()->GetCount() == 0);
}

void OrderEditFrame::AddToRemovedOrderArray(const unsigned int orderId) {
	bool found = false;

	const unsigned int count = removedOrderArray.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		if(orderId == removedOrderArray.Item(i)) {
			found = true;
			break;
		}
	}

	if(!found)
		removedOrderArray.Add(orderId);
}

void OrderEditFrame::RemoveFromRemovedOrderArray(const unsigned int orderId) {
	const unsigned int count = removedOrderArray.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		if(orderId == removedOrderArray.Item(i)) {
			removedOrderArray.RemoveAt(i);
			break;
		}
	}
}

void OrderEditFrame::Transfer(OrderEntity *orderEntity, const bool negate) {
	bool isChanged = changed;

	if(!orderEntity)
		orderEntity = static_cast<OrderEntity *>(bind.GetEntity());

	// Clean up positions
	OrderPositionEntities *orderPositionEntities = orderEntity->GetPositions();
	orderPositionEntities->Clear(false);

	DSCurrency tmp;
	OrderPositionEntity *orderPositionEntity = NULL;
	unsigned int count = orderPositionsListCtrl->GetItemCount();
	for(unsigned int i = 0; i < count; i++) {
		orderPositionEntity = static_cast<OrderPositionEntity *>(orderPositionsListCtrl->GetPointer(i, 0));
		if(orderPositionEntity) {
			orderPositionEntity->SetValue(_N("rank"), i + 1);

			if(negate) {
				tmp = orderPositionEntity->Get(_N("price"))->ToCurrency() * -1;
				orderPositionEntity->SetValue(_N("price"), tmp);
			}

			orderPositionEntities->Add(orderPositionEntity);
			AddOrChangeToOrderPositionsListCtrl(orderPositionEntity, false, true);
		}
	}

	// Clean up linked orders
	OrderEntities *orderLinkedOrderEntites = orderEntity->GetLinkedOrders();
	orderLinkedOrderEntites->Clear(false);

	OrderEntity *linkedOrderEntity = NULL;
	count = linkedOrderEntities.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		linkedOrderEntity = static_cast<OrderEntity *>(linkedOrdersListCtrl->GetPointer(i, 0));
		if(linkedOrderEntity) {
			linkedOrderEntity->SetValue(_N("linkedOrderIndex"), i + 1);
			orderLinkedOrderEntites->Add(linkedOrderEntity);
		}
	}

	bind.Pull();
	orderEntity->SetValue(_N("orderId"), orderId);
	orderEntity->SetValue(_N("shippingAddressTranslated"), shippingAddressTranslated);
	orderEntity->SetValue(_N("billAddressTranslated"), billAddressTranslated);
	orderEntity->Recalc();

	mainStatusBar->SetStatusText(
		_("Price: ") + orderEntity->Get(_N("beforeTax"))->ToCurrency().ToString() + _N(", ") +
		_("Full tax: ") + orderEntity->Get(_N("fullTax"))->ToCurrency().ToString() + _N(", ") +
		_("Reduced tax: ") + orderEntity->Get(_N("reducedTax"))->ToCurrency().ToString() +  _N(", ") +
		_("Sum: ") + orderEntity->Get(_N("afterTax"))->ToCurrency().ToString(),
		1
	);

	if(!isChanged)
		changed = false;
}

bool OrderEditFrame::IsInvoiced() {
	if(orderId == 0)
		return false;
	
	OrderController orderController(rit);
	return orderController.IsInvoiced(orderId);
}

void OrderEditFrame::NegatePositionPrices() {
	Transfer(NULL, true);
}

void OrderEditFrame::AddressTranslate(const int id) {
	wxString *s = NULL;
	
	if(id == XRCID("shippingAddressTranslateEditButton"))
		s = &shippingAddressTranslated;
	else if(id == XRCID("billAddressTranslateEditButton"))
		s = &billAddressTranslated;
	
	if(!s)
		return;

	wxString check(*s);
	
	TranslatedAddressEditDialog *translatedAddressEditDialog = new TranslatedAddressEditDialog(
		this,
		-1,
		rit,
		s,
		clientId,
		IsInvoiced()
	);
	translatedAddressEditDialog->Center();
	translatedAddressEditDialog->ShowModal();
	translatedAddressEditDialog->Destroy();

	if(*s != check)
		changed = true;

	SetTranslatedAddressIcons();
}

void OrderEditFrame::SetTranslatedAddressIcons() {
	billAddressTranslatedStaticImage->SetBitmap(wxBitmap(
		rit->GRP(_N("images"), billAddressTranslated.IsEmpty() ? _N("AddressNotTranslated.png") : _N("AddressTranslated.png")),
		wxBITMAP_TYPE_PNG
	));
	shippingAddressTranslatedStaticImage->SetBitmap(wxBitmap(
		rit->GRP(_N("images"), shippingAddressTranslated.IsEmpty() ? _N("AddressNotTranslated.png") : _N("AddressTranslated.png")),
		wxBITMAP_TYPE_PNG
	));
}

void OrderEditFrame::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == prepandTextStaticText)
		textCtrl = prepandTextTextCtrl;
	else if(object == appendTextStaticText)
		textCtrl = appendTextTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}

void OrderEditFrame::Reminders() {
	if(!rit->Access(A_READ, _("You have no permission to open reminders."), this))
		return;

	if(orderId == 0) {
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
		ReminderType::ORDERS,
		orderId
	);
	remindersDialog->Center();
	remindersDialog->ShowModal();
	remindersDialog->Destroy();
}
