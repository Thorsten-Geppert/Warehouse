#ifndef CONTACT_DIALOG_H_
#define CONTACT_DIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class ContactDialog : public wxDialog {

	public:
		ContactDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
	
	protected:
		enum {
			SEND_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void SendEvent(wxMouseEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void SendKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Send();
		void Cancel();

		RuntimeInformationType *rit;
		bool changed;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		ToolbarButton *sendToolbarButton;
		ToolbarButton *cancelToolbarButton;

		DSTextCtrl *yourNameTextCtrl;
		DSTextCtrl *emailTextCtrl;
		DSTextCtrl *subjectTextCtrl;
		DSTextCtrl *messageTextCtrl;

};

#endif /* CONTACT_DIALOG_H_ */
