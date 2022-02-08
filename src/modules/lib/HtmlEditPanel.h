#ifndef HTML_EDIT_PANEL_H_
#define HTML_EDIT_PANEL_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"

using namespace DynSoft;

class HtmlEditPanel : public wxPanel {

	public:
		HtmlEditPanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &content
		);

		void SetContent(const wxString &content);
		wxString GetContent() const;

		void WriteText(const wxString &text);
		void SurroundText(const wxString &prepend, const wxString &append);

	protected:
		DECLARE_EVENT_TABLE()

		void CopyEvent(wxCommandEvent &event);
		void CutEvent(wxCommandEvent &event);
		void PasteEvent(wxCommandEvent &event);
		void BoldEvent(wxCommandEvent &event);
		void ItalicEvent(wxCommandEvent &event);
		void UnderlineEvent(wxCommandEvent &event);
		void TableEvent(wxCommandEvent &event);

		void Copy();
		void Cut();
		void Paste();
		void Bold();
		void Italic();
		void Underline();
		void Table();

		RuntimeInformationType *rit;

		wxTextCtrl *contentTextCtrl;

};

#endif /* HTML_EDIT_PANEL_H_ */
