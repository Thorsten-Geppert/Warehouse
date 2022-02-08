#ifndef TEXTINPUTDIALOG_H_
#define TEXTINPUTDIALOG_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

class TextInputDialog : public wxDialog {

	public:
		TextInputDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &title,
			const wxString &headline,
			const bool emptyInputTextAllowed = false,
			const wxString &inputText = wxEmptyString
		);

		wxString GetValue() const;
	
	protected:
		DECLARE_EVENT_TABLE()
		
		enum {
			SAVE_EVENT,
			CLOSE_EVENT
		};

		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);

		void Cancel();
		void Save();

		RuntimeInformationType *rit;
		bool emptyInputTextAllowed;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *saveToolbarButton;

		wxStaticText *headlineStaticText;
		wxTextCtrl *inputTextCtrl;

};

#endif /* TEXTINPUTDIALOG_H_ */
