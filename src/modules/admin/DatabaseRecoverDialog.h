#ifndef DATABASE_RECOVER_DIALOG_H_
#define DATABASE_RECOVER_DIALOG_H_

#include <wx/wx.h>
#include <wx/statline.h>
#include <DSPostgreSQLDatabase.h>

using namespace DynSoft;

class DatabaseRecoverDialog : public wxDialog {

	public:
		DatabaseRecoverDialog(wxWindow *parent, wxWindowID id, DSPostgreSQLDatabase *db);
	
	protected:
		wxString path, filename;
		DSPostgreSQLDatabase *db;
		bool databaseTextCtrlIsEdited;

		enum {
			ABORT_EVENT,
			RECOVER_EVENT,
			BROWSE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void AbortEvent(wxCommandEvent &event);
		void RecoverEvent(wxCommandEvent &event);
		void BrowseEvent(wxCommandEvent &event);
		void CompanyEvent(wxCommandEvent &event);
		void DatabaseEvent(wxCommandEvent &event);

		void Abort();
		void Recover();
		void Browse();
		void Company();
		void Database();
		
		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		wxTextCtrl *informationTextCtrl;
		wxStaticLine *informationStaticLine;
		wxFlexGridSizer *dataFlexGridSizer;

		wxStaticText *filenameStaticText;
		wxBoxSizer *filenameBoxSizer;
		wxTextCtrl *filenameTextCtrl;
		wxButton *filenameButton;

		wxStaticText *companyStaticText;
		wxTextCtrl *companyTextCtrl;
		wxStaticText *hostnameStaticText;
		wxTextCtrl *hostnameTextCtrl;
		wxStaticText *databaseStaticText;
		wxTextCtrl *databaseTextCtrl;
		wxStaticText *usernameStaticText;
		wxTextCtrl *usernameTextCtrl;
		wxStaticText *passwordStaticText;
		wxTextCtrl *passwordTextCtrl;
		wxStaticText *portStaticText;
		wxTextCtrl *portTextCtrl;

		wxStaticLine *optionsStaticLine;
		wxBoxSizer *optionsBoxSizer;
		wxButton *abortButton;
		wxButton *recoverButton;

};

#endif /* DATABASE_RECOVER_DIALOG_H_ */
