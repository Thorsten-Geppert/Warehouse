#include "YesNoDialog.h"
#include <DSCasts.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(YesNoDialog, wxDialog)
	EVT_MENU(YES_EVENT, YesNoDialog::YesKeyEvent)
	EVT_MENU(NO_EVENT, YesNoDialog::NoKeyEvent)
END_EVENT_TABLE()

YesNoDialog::YesNoDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &message,
	const wxString &headline,
	const wxString &labelYes,
	const wxString &labelNo
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("YesNoDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("YesNoDialog"));

	mainPanel          = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer       = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	yesToolbarButton   = XRCCTRL(*this, "yesToolbarButton", ToolbarButton);
	noToolbarButton    = XRCCTRL(*this, "noToolbarButton", ToolbarButton);
	messageTextCtrl    = XRCCTRL(*this, "messageTextCtrl", wxTextCtrl);

	if(!labelYes.IsEmpty())
		yesToolbarButton->SetLabel(labelYes);
	if(!labelNo.IsEmpty())
		noToolbarButton->SetLabel(labelNo);

	mainBoxSizer->SetSizeHints(this);
	SetSize(500, -1);

	CONNECT_TOOLBARBUTTON(yesToolbarButton, YesNoDialog::YesEvent);
	CONNECT_TOOLBARBUTTON(noToolbarButton, YesNoDialog::NoEvent);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, NO_EVENT);
	acceleratorEntries[1].Set(wxACCEL_NORMAL, WXK_RETURN, YES_EVENT);
	acceleratorEntries[2].Set(wxACCEL_NORMAL, WXK_SPACE,  YES_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,   (int) 'w',  NO_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Fill
	messageTextCtrl->SetValue(message);
	SetTitle(headline);
	messageTextCtrl->SetFocus();

	ICON();
}

void YesNoDialog::YesEvent(wxMouseEvent &event) {
	Yes();
	SKIP();
}

void YesNoDialog::YesKeyEvent(wxCommandEvent &event) {
	Yes();
}

void YesNoDialog::NoEvent(wxMouseEvent &event) {
	No();
	SKIP();
}

void YesNoDialog::NoKeyEvent(wxCommandEvent &event) {
	No();
}

void YesNoDialog::Yes() {
	EndModal(wxID_YES);
}

void YesNoDialog::No() {
	EndModal(wxID_NO);
}
