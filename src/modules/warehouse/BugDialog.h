#ifndef BUG_DIALOG_H_
#define BUG_DIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class BugDialog : public wxDialog {

	public:
		BugDialog(
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
		DSTextCtrl *bugTextCtrl;
		DSComboBox *moduleComboBox;
		DSComboBox *priorityComboBox;
		DSTextCtrl *descriptionTextCtrl;

};

#endif /* BUG_DIALOG_H_ */
