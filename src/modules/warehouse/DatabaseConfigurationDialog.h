#ifndef DATABASECONFIGURATIONDIALOG_H_
#define DATABASECONFIGURATIONDIALOG_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <DSPostgreSQLDatabase.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class DatabaseConfigurationDialog : public wxDialog {

	public:
		DatabaseConfigurationDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
	
	protected:
		enum {
			CANCEL_EVENT,
			TEST_CONNECTION_EVENT,
			NEW_CONNECTION_EVENT,
			SAVE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void TestConnectionEvent(wxMouseEvent &event);
		void TestConnectionKeyEvent(wxCommandEvent &event);
		void NewConnectionEvent(wxMouseEvent &event);
		void NewConnectionKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);

		void TestConnection();
		void NewConnection();
		void Cancel();
		void Save();

		RuntimeInformationType *rit;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *testConnectionToolbarButton;
		ToolbarButton *newConnectionToolbarButton;
		ToolbarButton *saveToolbarButton;

		wxComboBox *hostnameComboBox;
		wxComboBox *databaseComboBox;
		wxComboBox *usernameComboBox;
		wxTextCtrl *passwordTextCtrl;
		wxComboBox *portComboBox;

};

#endif /* DATABASECONFIGURATIONDIALOG_H_ */
