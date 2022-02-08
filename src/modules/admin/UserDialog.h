#ifndef USER_DIALOG_H_
#define USER_DIALOG_H_

#include <wx/wx.h>
#include <wx/statline.h>

class UserDialog : public wxDialog {

	public:
		UserDialog(wxWindow *parent, wxWindowID id);
		void GetUsernameAndPassword(wxString &username, wxString &password);
	
	protected:
		enum {
			ABORT_EVENT,
			ADD_EVENT
		};

		DECLARE_EVENT_TABLE()

		void AbortEvent(wxCommandEvent &event);
		void AddEvent(wxCommandEvent &event);

		void Abort();
		void Add();
		
		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		wxTextCtrl *informationTextCtrl;
		wxStaticLine *informationStaticLine;
		wxFlexGridSizer *dataFlexGridSizer;
		wxStaticText *usernameStaticText;
		wxTextCtrl *usernameTextCtrl;
		wxStaticText *password1StaticText;
		wxTextCtrl *password1TextCtrl;
		wxStaticText *password2StaticText;
		wxTextCtrl *password2TextCtrl;
		wxStaticLine *optionsStaticLine;
		wxBoxSizer *optionsBoxSizer;
		wxButton *abortButton;
		wxButton *addButton;

};

#endif /* USER_DIALOG_H_ */
