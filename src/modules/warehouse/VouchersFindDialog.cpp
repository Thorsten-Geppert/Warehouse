#include "VouchersFindDialog.h"
#include "../lib/KeyValueDatabaseLibrary.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(VouchersFindDialog, wxDialog)
	EVT_MENU(FIND_EVENT, VouchersFindDialog::FindKeyEvent)
	EVT_MENU(CANCEL_EVENT, VouchersFindDialog::CancelKeyEvent)
END_EVENT_TABLE()

VouchersFindDialog::VouchersFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("VouchersFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("VouchersFindDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton   = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	voucherIdTextCtrl   = XRCCTRL(*this, "voucherIdTextCtrl", DynSoft::DSTextCtrl);
	descriptionComboBox = XRCCTRL(*this, "descriptionComboBox", wxComboBox);
	invoiceIdTextCtrl   = XRCCTRL(*this, "invoiceIdTextCtrl", wxTextCtrl);
	companyTextCtrl     = XRCCTRL(*this, "companyTextCtrl", wxTextCtrl);
	notesTextCtrl       = XRCCTRL(*this, "notesTextCtrl", wxTextCtrl);
	yearTextCtrl        = XRCCTRL(*this, "yearTextCtrl", DynSoft::DSTextCtrl);

	yearTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	voucherIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(findToolbarButton, VouchersFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, VouchersFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();
}

int VouchersFindDialog::ShowModal() {
	KeyValueDatabaseLibrary keyValueDatabaseLibrary(rit, _N("advanced_preferences"));
	if(keyValueDatabaseLibrary.Get(_N("global.year")).ToInt() > 0)
		yearTextCtrl->SetValue(keyValueDatabaseLibrary.Get(_N("global.year")).ToString());
	else
		yearTextCtrl->Clear();

	voucherIdTextCtrl->SetFocus();
	return wxDialog::ShowModal();
}

void VouchersFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void VouchersFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void VouchersFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void VouchersFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void VouchersFindDialog::Find() {
	EndModal(wxID_OK);
}

void VouchersFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

VoucherEntity *VouchersFindDialog::GetVoucherEntity() const {
	VoucherEntity *voucherEntity = new VoucherEntity(rit);

	voucherEntity->SetValue(_N("voucherId"), voucherIdTextCtrl->GetValue());
	voucherEntity->SetValue(_N("description"), descriptionComboBox->GetValue());
	voucherEntity->SetValue(_N("invoiceId"), invoiceIdTextCtrl->GetValue());
	voucherEntity->SetValue(_N("company"), companyTextCtrl->GetValue());
	voucherEntity->SetValue(_N("notes"), notesTextCtrl->GetValue());
	voucherEntity->SetSearchYear(DSCasts::ToUnsignedInt(yearTextCtrl->GetValue()));

	return voucherEntity;
}
