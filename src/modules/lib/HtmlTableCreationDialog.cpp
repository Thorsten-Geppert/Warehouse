#include "HtmlTableCreationDialog.h"

BEGIN_EVENT_TABLE(HtmlTableCreationDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, HtmlTableCreationDialog::CancelKeyEvent)
	EVT_MENU(INSERT_EVENT, HtmlTableCreationDialog::InsertKeyEvent)
END_EVENT_TABLE()

HtmlTableCreationDialog::HtmlTableCreationDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const unsigned int tabs
) {
	SetId(id);
	this->rit  = rit;
	this->tabs = tabs;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("HtmlTableCreationDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("HtmlTableCreationDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar         = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	insertToolbarButton = XRCCTRL(*this, "insertToolbarButton", ToolbarButton);
	columnsSpinCtrl     = XRCCTRL(*this, "columnsSpinCtrl", wxSpinCtrl);
	rowsSpinCtrl        = XRCCTRL(*this, "rowsSpinCtrl", wxSpinCtrl);
	borderCheckBox      = XRCCTRL(*this, "borderCheckBox", wxCheckBox);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(300, -1));

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, HtmlTableCreationDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(insertToolbarButton, HtmlTableCreationDialog::InsertEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  INSERT_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);
}

void HtmlTableCreationDialog::InsertEvent(wxMouseEvent &event) {
	Insert();
	SKIP();
}

void HtmlTableCreationDialog::InsertKeyEvent(wxCommandEvent &event) {
	Insert();
}

void HtmlTableCreationDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void HtmlTableCreationDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void HtmlTableCreationDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void HtmlTableCreationDialog::Insert() {
	EndModal(wxID_OK);
}

wxString HtmlTableCreationDialog::CreateTabs(const unsigned int tabs) {
	wxString tmp;
	for(unsigned int i = 0; i < tabs; i++)
		tmp += _N("	");
	return tmp;
}

wxString HtmlTableCreationDialog::GetTable() {
	wxString content;
	const int rows = rowsSpinCtrl->GetValue();
	const int cols = columnsSpinCtrl->GetValue();

	for(int i = 0; i < rows; i++) {
		content += CreateTabs(tabs) + _N("	<tr>\n");
		for(int j = 0; j < cols; j++)
			content += CreateTabs(tabs) + _N("		<td></td>\n");
		content += CreateTabs(tabs) + _N("	</tr>\n");
	}

	return 
		_N("<table border=\"") + (borderCheckBox->GetValue() ? _N("1") : _N("0")) + _N("\">\n") +
		content +
		CreateTabs(tabs) + _N("</table>\n")
	;
}
