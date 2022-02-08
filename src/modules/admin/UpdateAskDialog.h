#ifndef UPDATE_ASK_DIALOG_H_
#define UPDATE_ASK_DIALOG_H_

#include <wx/wx.h>
#include <wx/statline.h>

class UpdateAskDialog : public wxDialog {

	public:
		UpdateAskDialog(
			wxWindow *parent,
			wxWindowID id,
			const wxString &from,
			const wxString &to
		);
	
	protected:
		enum {
			CANCEL_BUTTON_EVENT,
			UPDATE_BUTTON_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelEvent(wxCommandEvent &event);
		void UpdateEvent(wxCommandEvent &event);

		void Cancel();
		void Update();
		
		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		wxBoxSizer *dataBoxSizer;

		wxStaticText *headlineStaticText;
		wxTextCtrl *informationsTextCtrl;

		wxBoxSizer *optionsBoxSizer;
		wxButton *cancelButton;
		wxButton *updateButton;

};

#endif /* UPDATE_ASK_DIALOG_H_ */
