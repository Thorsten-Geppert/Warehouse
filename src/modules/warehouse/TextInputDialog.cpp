#include "TextInputDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(TextInputDialog, wxDialog)
	EVT_TEXT_ENTER(XRCID("inputTextCtrl"), TextInputDialog::SaveKeyEvent)
	EVT_MENU(CLOSE_EVENT, TextInputDialog::CancelKeyEvent)
	EVT_MENU(SAVE_EVENT, TextInputDialog::SaveKeyEvent)
END_EVENT_TABLE()

TextInputDialog::TextInputDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &title,
	const wxString &headline,
	const bool emptyInputTextAllowed,
	const wxString &inputText
) {
	SetId(id);
	this->rit                   = rit;
	this->emptyInputTextAllowed = emptyInputTextAllowed;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("TextInputDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("TextInputDialog"));

	SetTitle(title);

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar         = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	saveToolbarButton   = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	inputTextCtrl       = XRCCTRL(*this, "inputTextCtrl", wxTextCtrl);
	headlineStaticText  = XRCCTRL(*this, "headlineStaticText", wxStaticText);

	headlineStaticText->SetLabel(headline);
	inputTextCtrl->SetValue(inputText);
	inputTextCtrl->SetSelection(-1, -1);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(300, -1));

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, TextInputDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, TextInputDialog::SaveEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();

	inputTextCtrl->SetFocus();
}

void TextInputDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void TextInputDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void TextInputDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void TextInputDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void TextInputDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void TextInputDialog::Save() {
	if(!emptyInputTextAllowed && inputTextCtrl->GetValue().IsEmpty()) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please fill out input field"),
			_("An error has occured")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();

		inputTextCtrl->SetFocus();
	} else {
		EndModal(wxID_OK);
	}
}

wxString TextInputDialog::GetValue() const {
	return inputTextCtrl->GetValue();
}
