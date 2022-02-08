#ifndef CONFIGURATIONDIALOG_H_
#define CONFIGURATIONDIALOG_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"


using namespace DynSoft;

class ConfigurationDialog : public wxDialog {

	public:
		ConfigurationDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		
	protected:
		enum {
			CANCEL_EVENT,
			RUN_EVENT,
			MANUAL_EVENT
		};

		DECLARE_EVENT_TABLE()

		// Events
		void RunEvent(wxMouseEvent &event);
		void RunKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void ManualEvent(wxMouseEvent &event);
		void ManualKeyEvent(wxCommandEvent &event);

		void AutoConfigurationEvent(wxCommandEvent &event);
		void EnterpriseDatabaseEvent(wxCommandEvent &event);

		// Methods
		void Run();
		void Cancel();
		void Manual();
		bool TestConnection(
			const wxString hostname,
			const wxString database,
			const wxString username,
			const wxString password,
			const wxString port
		);
		void NormalizeEnterpriseDatabase();
		bool SaveConfigurationFile();
		bool InsertEnterprise();
		bool CreateMainDatabase();
		bool CreateEnterprise();

		RuntimeInformationType *rit;
		wxString company;
		wxString enterpriseDatabase;
		wxString hostname;
		wxString database;
		wxString username;
		wxString password;
		wxString port;
		
		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *runToolbarButton;
		ToolbarButton *manualToolbarButton;
		ToolbarButton *cancelToolbarButton;

		wxTextCtrl *enterpriseEnterpriseTextCtrl;
		wxStaticText *enterpriseDatabaseStaticText;

		wxCheckBox *configurationCheckBox;
		wxPanel *databasePanel;
		wxFlexGridSizer *databaseFlexGridSizer;

		wxStaticText *hostnameStaticText;
		wxComboBox *hostnameComboBox;
		wxStaticText *databaseStaticText;
		wxComboBox *databaseComboBox;
		wxStaticText *usernameStaticText;
		wxComboBox *usernameComboBox;
		wxStaticText *passwordStaticText;
		DSTextCtrl *passwordTextCtrl;
		wxStaticText *portStaticText;
		wxComboBox *portComboBox;

};

#endif /* CONFIGURATIONDIALOG_H_ */
