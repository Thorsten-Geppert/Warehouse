#ifndef ENTERPRISECONFIGURATIONDIALOG_H_
#define ENTERPRISECONFIGURATIONDIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/EnterpriseEntity.h"
#include "../lib/EnterpriseController.h"

using namespace DynSoft;

class EnterpriseConfigurationDialog : public wxDialog {

	public:
		EnterpriseConfigurationDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~EnterpriseConfigurationDialog();
	
	protected:
		enum {
			CANCEL_EVENT,
			NEW_EVENT,
			REMOVE_EVENT,
			SAVE_EVENT,
			NEW_DATABASE_EVENT,
			TEST_CONNECTION_EVENT
		};

		DECLARE_EVENT_TABLE()

		void NewEvent(wxMouseEvent &event);
		void NewKeyEvent(wxCommandEvent &event);
		void EditEvent(wxCommandEvent &event);
		void RemoveEvent(wxMouseEvent &event);
		void RemoveKeyEvent(wxCommandEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void NewDatabaseEvent(wxMouseEvent &event);
		void NewDatabaseKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void CloseEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void TestConnectionEvent(wxMouseEvent &event);
		void TestConnectionKeyEvent(wxCommandEvent &event);

		void New();
		void Edit();
		void Remove();
		bool Save();
		void NewDatabase();
		void Cancel();
		void Close();
		void TestConnection();

		void SetModus(const bool edit);
		unsigned int LoadEnterprises();
		unsigned int UnloadEnterprises();
		unsigned int ReloadEnterprises();

		void TransferData();
		void SetData();

		RuntimeInformationType *rit;
		EnterpriseEntity *editingEnterprise;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *newToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *saveToolbarButton;
		ToolbarButton *newDatabaseToolbarButton;
		ToolbarButton *testConnectionToolbarButton;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *closeToolbarButton;

		wxListBox *enterpriseListBox;
		wxTextCtrl *companyTextCtrl;
		wxComboBox *hostnameComboBox;
		wxTextCtrl *databaseNameTextCtrl;
		wxComboBox *usernameComboBox;
		wxTextCtrl *passwordTextCtrl;
		wxComboBox *portComboBox;

};

#endif /* ENTERPRISECONFIGURATIONDIALOG_H_ */
