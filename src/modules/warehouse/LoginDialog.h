#ifndef LOGINDIALOG_H_
#define LOGINDIALOG_H_

#include <wx/wx.h>
#include <wx/hyperlink.h>
#include "../lib/RuntimeInformationType.h"

class LoginDialog : public wxDialog {

	public:
		LoginDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~LoginDialog();
		bool IsDemoVersion();
	
	private:
		DECLARE_EVENT_TABLE()

		void LicenceEvent(wxCommandEvent &event);
		void CancelEvent(wxCommandEvent &event);
		void LoginEvent(wxCommandEvent &event);
		void ConfigureEnterprisesEvent(wxCommandEvent &event);
		void LoginInformationEvent(wxCommandEvent &event);
		void LoginHelpEvent(wxCommandEvent &event);
		void DynSoftEvent(wxMouseEvent &event);

		void Licence();
		void Cancel();
		void Login();
		void ConfigureEnterprises();
		void DynSoft();
		void LoginInformation();
		void LoginHelp();

		unsigned int LoadEnterprises();
		unsigned int UnloadEnterprises();
		unsigned int ReloadEnterprises();

		RuntimeInformationType *rit;
		wxString lastSelectedEnterprise;
		bool demoVersion;

		wxPanel *loginPanel;
		wxBoxSizer *loginBoxSizer;

		wxComboBox *enterpriseComboBox;
		wxBitmapButton *enterpriseConfigurationBitmapButton;
		wxTextCtrl *usernameTextCtrl;
		wxTextCtrl *passwordTextCtrl;

		wxButton *cancelButton;
		wxButton *loginButton;

		wxStaticBitmap *loginLogoStaticBitmap;
		wxButton *informationButton;
		wxButton *helpButton;

};

#endif /* LOGINDIALOG_H_ */
