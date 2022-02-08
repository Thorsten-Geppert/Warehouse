#include "HtmlEditPanel.h"
#include <wx/xrc/xmlres.h>
#include "HtmlTableCreationDialog.h"

BEGIN_EVENT_TABLE(HtmlEditPanel, wxPanel)
	EVT_TOOL(XRCID("copyTool"), HtmlEditPanel::CopyEvent)
	EVT_TOOL(XRCID("cutTool"), HtmlEditPanel::CutEvent)
	EVT_TOOL(XRCID("pasteTool"), HtmlEditPanel::PasteEvent)
	EVT_TOOL(XRCID("boldTool"), HtmlEditPanel::BoldEvent)
	EVT_TOOL(XRCID("italicTool"), HtmlEditPanel::ItalicEvent)
	EVT_TOOL(XRCID("underlineTool"), HtmlEditPanel::UnderlineEvent)
	EVT_TOOL(XRCID("tableTool"), HtmlEditPanel::TableEvent)
END_EVENT_TABLE()

HtmlEditPanel::HtmlEditPanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &content
) {
	this->rit = rit;

	Hide();

	SetId(id);

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("HtmlEditPanel.xml")));
	wxXmlResource::Get()->LoadPanel(this, parent, _N("HtmlEditPanel"));

	contentTextCtrl = XRCCTRL(*this, "contentTextCtrl", wxTextCtrl);
	
	contentTextCtrl->SetValue(content);
}

void HtmlEditPanel::CopyEvent(wxCommandEvent &event) {
	Copy();
}

void HtmlEditPanel::CutEvent(wxCommandEvent &event) {
	Cut();
}

void HtmlEditPanel::PasteEvent(wxCommandEvent &event) {
	Paste();
}

void HtmlEditPanel::BoldEvent(wxCommandEvent &event) {
	Bold();
}

void HtmlEditPanel::ItalicEvent(wxCommandEvent &event) {
	Italic();
}

void HtmlEditPanel::UnderlineEvent(wxCommandEvent &event) {
	Underline();
}

void HtmlEditPanel::TableEvent(wxCommandEvent &event) {
	Table();
}

void HtmlEditPanel::Copy() {
	contentTextCtrl->Copy();
}

void HtmlEditPanel::Cut() {
	contentTextCtrl->Cut();
}

void HtmlEditPanel::Paste() {
	contentTextCtrl->Paste();
}

void HtmlEditPanel::Bold() {
	SurroundText(_N("<b>"), _N("</b>"));
}

void HtmlEditPanel::Italic() {
	SurroundText(_N("<i>"), _N("</i>"));
}

void HtmlEditPanel::Underline() {
	SurroundText(_N("<u>"), _N("</u>"));
}

void HtmlEditPanel::Table() {
	long column = 0;
	long row    = 0;

	contentTextCtrl->PositionToXY(contentTextCtrl->GetInsertionPoint(), &column, &row);
	const wxString lineText(contentTextCtrl->GetLineText(row));
	const unsigned int length = lineText.Length();
	unsigned int tabs = 0;
	for(unsigned int i = 0; i < length; i++)
		if(lineText[i] == '\t')
			tabs++;
		else
			break;

	HtmlTableCreationDialog *htmlTableCreationDialog = new HtmlTableCreationDialog(this, -1, rit, tabs);
	htmlTableCreationDialog->Center();
	if(htmlTableCreationDialog->ShowModal() == wxID_OK) {
		WriteText(htmlTableCreationDialog->GetTable());
	}
	htmlTableCreationDialog->Destroy();
}

void HtmlEditPanel::SetContent(const wxString &content) {
	contentTextCtrl->SetValue(content);
}

wxString HtmlEditPanel::GetContent() const {
	return contentTextCtrl->GetValue();
}

void HtmlEditPanel::WriteText(const wxString &text) {
	contentTextCtrl->WriteText(text);
}

void HtmlEditPanel::SurroundText(const wxString &prepend, const wxString &append) {
	const long insertionPoint = contentTextCtrl->GetInsertionPoint();
	const wxString content(contentTextCtrl->GetStringSelection());
	WriteText(prepend + content + append);
	contentTextCtrl->SetInsertionPoint(prepend.Length() + content.Length() + insertionPoint);
}
