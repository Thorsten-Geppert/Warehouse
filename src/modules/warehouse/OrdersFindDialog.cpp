#include "OrdersFindDialog.h"
#include <DSCasts.h>
#include "../lib/ConstantsLibrary.h"
#include "../lib/ListEntryController.h"
#include "../lib/KeyValueDatabaseLibrary.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(OrdersFindDialog, wxDialog)
	EVT_MENU(FIND_EVENT, OrdersFindDialog::FindKeyEvent)
	EVT_MENU(CANCEL_EVENT, OrdersFindDialog::CancelKeyEvent)
END_EVENT_TABLE()

OrdersFindDialog::OrdersFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const unsigned int clientId
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("OrdersFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("OrdersFindDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton   = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	orderIdTextCtrl     = XRCCTRL(*this, "orderIdTextCtrl", wxTextCtrl);
	invoiceIdTextCtrl   = XRCCTRL(*this, "invoiceIdTextCtrl", wxTextCtrl);
	clientIdTextCtrl    = XRCCTRL(*this, "clientIdTextCtrl", wxTextCtrl);
	kindComboBox        = XRCCTRL(*this, "kindComboBox", wxComboBox);
	salutationComboBox  = XRCCTRL(*this, "salutationComboBox", wxComboBox);
	titleComboBox       = XRCCTRL(*this, "titleComboBox", wxComboBox);
	firstnameTextCtrl   = XRCCTRL(*this, "firstnameTextCtrl", wxTextCtrl);
	nameTextCtrl        = XRCCTRL(*this, "nameTextCtrl", wxTextCtrl);
	addressTextCtrl     = XRCCTRL(*this, "addressTextCtrl", wxTextCtrl);
	zipcodeTextCtrl     = XRCCTRL(*this, "zipcodeTextCtrl", wxTextCtrl);
	cityTextCtrl        = XRCCTRL(*this, "cityTextCtrl", wxTextCtrl);
	landComboBox        = XRCCTRL(*this, "landComboBox", wxComboBox);
	onlyUnpayedCheckBox = XRCCTRL(*this, "onlyUnpayedCheckBox", wxCheckBox);
	yearTextCtrl        = XRCCTRL(*this, "yearTextCtrl", DynSoft::DSTextCtrl);

	yearTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(findToolbarButton, OrdersFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, OrdersFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Fill
	kindComboBox->Append(ConstantsLibrary::OrderKindToArrayString());
	kindComboBox->SetString(kindComboBox->GetCount() - 1, _N(""));
	kindComboBox->SetSelection(kindComboBox->GetCount() - 1);

	ListEntryController::FillComboBox(rit, titleComboBox,        _N("titles"),       _N("title"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, salutationComboBox,   _N("salutations"),  _N("salutation"),   ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, landComboBox,         _N("lands"),        _N("land"),         ListEntryController::FILL_NEW, true);

	if(clientId != 0) {
		clientIdTextCtrl->SetEditable(false);
		clientIdTextCtrl->SetValue(DSCasts::ToString(clientId));
	}

	ICON();
}

int OrdersFindDialog::ShowModal() {
	KeyValueDatabaseLibrary keyValueDatabaseLibrary(rit, _N("advanced_preferences"));
	if(keyValueDatabaseLibrary.Get(_N("global.year")).ToInt() > 0)
		yearTextCtrl->SetValue(keyValueDatabaseLibrary.Get(_N("global.year")).ToString());
	else
		yearTextCtrl->Clear();

	orderIdTextCtrl->SetFocus();
	return wxDialog::ShowModal();
}

void OrdersFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void OrdersFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void OrdersFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void OrdersFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void OrdersFindDialog::Find() {
	EndModal(wxID_OK);
}

void OrdersFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

OrderEntity *OrdersFindDialog::GetOrderEntity() const {
	OrderEntity *orderEntity = new OrderEntity(rit);

	orderEntity->SetValue(_N("orderId"), orderIdTextCtrl->GetValue());
	orderEntity->SetValue(_N("clientId"), clientIdTextCtrl->GetValue());
	orderEntity->SetValue(_N("invoiceId"), invoiceIdTextCtrl->GetValue());
	orderEntity->SetValue(_N("billAddressSalutation"), salutationComboBox->GetValue());
	orderEntity->SetValue(_N("billAddressTitle"), titleComboBox->GetValue());
	orderEntity->SetValue(_N("billAddressFirstname"), firstnameTextCtrl->GetValue());
	orderEntity->SetValue(_N("billAddressName1"), nameTextCtrl->GetValue());
	orderEntity->SetValue(_N("billAddressName2"), nameTextCtrl->GetValue());
	orderEntity->SetValue(_N("billAddressName3"), nameTextCtrl->GetValue());
	orderEntity->SetValue(_N("billAddressName4"), nameTextCtrl->GetValue());
	orderEntity->SetValue(_N("billAddressAddress"), addressTextCtrl->GetValue());
	orderEntity->SetValue(_N("billAddressCity"), cityTextCtrl->GetValue());
	orderEntity->SetValue(_N("billAddressLand"), landComboBox->GetValue());
	orderEntity->SetValue(_N("shippingAddressSalutation"), salutationComboBox->GetValue());
	orderEntity->SetValue(_N("shippingAddressTitle"), titleComboBox->GetValue());
	orderEntity->SetValue(_N("shippingAddressFirstname"), firstnameTextCtrl->GetValue());
	orderEntity->SetValue(_N("shippingAddressName1"), nameTextCtrl->GetValue());
	orderEntity->SetValue(_N("shippingAddressName2"), nameTextCtrl->GetValue());
	orderEntity->SetValue(_N("shippingAddressName3"), nameTextCtrl->GetValue());
	orderEntity->SetValue(_N("shippingAddressName4"), nameTextCtrl->GetValue());
	orderEntity->SetValue(_N("shippingAddressAddress"), addressTextCtrl->GetValue());
	orderEntity->SetValue(_N("shippingAddressCity"), cityTextCtrl->GetValue());
	orderEntity->SetValue(_N("shippingAddressLand"), landComboBox->GetValue());
	orderEntity->SetValue(_N("kind"), kindComboBox->GetSelection());
	orderEntity->SetOnlyUnpayed(onlyUnpayedCheckBox->GetValue());
	orderEntity->SetSearchYear(DSCasts::ToUnsignedInt(yearTextCtrl->GetValue()));

	return orderEntity;
}
