#include "InvoiceDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(InvoiceDialog, wxDialog)
	EVT_MENU(CLOSE_EVENT, InvoiceDialog::CancelKeyEvent)
	EVT_MENU(SAVE_EVENT, InvoiceDialog::SaveKeyEvent)
END_EVENT_TABLE()

InvoiceDialog::InvoiceDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("InvoiceDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("InvoiceDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar           = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton   = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	saveToolbarButton     = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	invoiceDatePickerCtrl = XRCCTRL(*this, "invoiceDatePickerCtrl", wxDatePickerCtrl);

	invoiceDatePickerCtrl->SetValue(wxDateTime::Now());

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(300, -1));

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, InvoiceDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, InvoiceDialog::SaveEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();
}

void InvoiceDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void InvoiceDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void InvoiceDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void InvoiceDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void InvoiceDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void InvoiceDialog::Save() {
	EndModal(wxID_OK);
}

DSDateTime InvoiceDialog::GetValue() {
	DSDateTime dt;
	dt.SetWxDateTime(invoiceDatePickerCtrl->GetValue());
	return dt;
}
