#include "PurchasesFindDialog.h"
#include "Configuration.h"
#include "../lib/KeyValueDatabaseLibrary.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(PurchasesFindDialog, wxDialog)
	EVT_MENU(FIND_EVENT, PurchasesFindDialog::FindKeyEvent)
	EVT_MENU(CANCEL_EVENT, PurchasesFindDialog::CancelKeyEvent)
END_EVENT_TABLE()

PurchasesFindDialog::PurchasesFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const unsigned int clientId
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("PurchasesFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("PurchasesFindDialog"));

	mainPanel                          = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                       = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton                  = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	cancelToolbarButton                = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	purchaseIdTextCtrl                 = XRCCTRL(*this, "purchaseIdTextCtrl", DynSoft::DSTextCtrl);
	distributorIdTextCtrl              = XRCCTRL(*this, "distributorIdTextCtrl", DynSoft::DSTextCtrl);
	distributorAddressContainsTextCtrl = XRCCTRL(*this, "distributorAddressContainsTextCtrl", wxTextCtrl);
	billingAddressContainsTextCtrl     = XRCCTRL(*this, "billingAddressContainsTextCtrl", wxTextCtrl);
	deliveryAddressContainsTextCtrl    = XRCCTRL(*this, "deliveryAddressContainsTextCtrl", wxTextCtrl);
	orderIdTextCtrl                    = XRCCTRL(*this, "orderIdTextCtrl", DynSoft::DSTextCtrl);
	purchaserTextCtrl                  = XRCCTRL(*this, "purchaserTextCtrl", wxTextCtrl);
	informationsTextCtrl               = XRCCTRL(*this, "informationsTextCtrl", wxTextCtrl);
	yearTextCtrl                       = XRCCTRL(*this, "yearTextCtrl", DynSoft::DSTextCtrl);

	yearTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	purchaseIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	distributorIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	orderIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	CONNECT_TOOLBARBUTTON(findToolbarButton, PurchasesFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, PurchasesFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();
}

int PurchasesFindDialog::ShowModal() {
	KeyValueDatabaseLibrary keyValueDatabaseLibrary(rit, _N("advanced_preferences"));
	if(keyValueDatabaseLibrary.Get(_N("global.year")).ToInt() > 0)
		yearTextCtrl->SetValue(keyValueDatabaseLibrary.Get(_N("global.year")).ToString());
	else
		yearTextCtrl->Clear();

	purchaseIdTextCtrl->SetFocus();
	return wxDialog::ShowModal();
}

void PurchasesFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void PurchasesFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void PurchasesFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void PurchasesFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void PurchasesFindDialog::Find() {
	EndModal(wxID_OK);
}

void PurchasesFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

PurchaseEntity *PurchasesFindDialog::GetPurchaseEntity() const {
	PurchaseEntity *purchaseEntity = new PurchaseEntity(rit);

	purchaseEntity->SetValue(_N("purchaseId"), purchaseIdTextCtrl->GetValue());
	purchaseEntity->SetValue(_N("distributorId"), distributorIdTextCtrl->GetValue());
	purchaseEntity->SetValue(_N("distributorAddress"), distributorAddressContainsTextCtrl->GetValue());
	purchaseEntity->SetValue(_N("deliveryAddress"), deliveryAddressContainsTextCtrl->GetValue());
	purchaseEntity->SetValue(_N("orderId"), orderIdTextCtrl->GetValue());
	purchaseEntity->SetValue(_N("purchaser"), purchaserTextCtrl->GetValue());
	purchaseEntity->SetValue(_N("informations"), informationsTextCtrl->GetValue());
	purchaseEntity->SetSearchYear(DSCasts::ToUnsignedInt(yearTextCtrl->GetValue()));

	return purchaseEntity;
}
