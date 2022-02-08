#include "PayedDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(PayedDialog, wxDialog)
	EVT_MENU(CLOSE_EVENT, PayedDialog::CancelKeyEvent)
	EVT_MENU(SAVE_EVENT, PayedDialog::SaveKeyEvent)
END_EVENT_TABLE()

PayedDialog::PayedDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &label
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("PayedDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("PayedDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar         = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	saveToolbarButton   = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	payedDatePickerCtrl = XRCCTRL(*this, "payedDatePickerCtrl", wxDatePickerCtrl);

	saveToolbarButton->SetLabel(label);

	payedDatePickerCtrl->SetValue(wxDateTime::Now());

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(300, -1));

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, PayedDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, PayedDialog::SaveEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();
}

void PayedDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void PayedDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void PayedDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void PayedDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void PayedDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void PayedDialog::Save() {
	EndModal(wxID_OK);
}

DSDateTime PayedDialog::GetValue() {
	DSDateTime dt;
	dt.SetWxDateTime(payedDatePickerCtrl->GetValue());
	return dt;
}
