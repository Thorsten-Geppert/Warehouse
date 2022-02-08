#ifndef YESNODIALOG_H_
#define YESNODIALOG_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class YesNoDialog : public wxDialog {

	public:
		YesNoDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &message,
			const wxString &headline,
			const wxString &labelYes = wxEmptyString,
			const wxString &labelNo  = wxEmptyString
		);
	
	protected:
		enum {
			YES_EVENT,
			NO_EVENT
		};

		DECLARE_EVENT_TABLE()

		void YesEvent(wxMouseEvent &event);
		void YesKeyEvent(wxCommandEvent &event);
		void NoEvent(wxMouseEvent &event);
		void NoKeyEvent(wxCommandEvent &event);

		void Yes();
		void No();

		RuntimeInformationType *rit;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *yesToolbarButton;
		ToolbarButton *noToolbarButton;
		wxTextCtrl    *messageTextCtrl;

};

#endif /* YESNODIALOG_H_ */
