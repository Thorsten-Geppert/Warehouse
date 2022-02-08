#ifndef DYNSOFT_WAREHOUSE_FRAME_H_
#define DYNSOFT_WAREHOUSE_FRAME_H_

#include <wx/wx.h>
#include <wx/statline.h>
#include <DSPostgreSQLDatabase.h>
#include <DSListCtrl.h>

using namespace DynSoft;

class AdminFrame : public wxFrame {

	private:
		static const int ACTION_UPDATE_ENTERPRISE_DATABASE =  0;
		static const int ACTION_UPDATE_SPECIAL             =  1;
		static const int ACTION_USER_ADD_EDIT              =  2;
		static const int ACTION_ENTERPRISE_BACKUP          =  3;
		static const int ACTION_ENTERPRISE_RECOVER         =  4;
		static const int ACTION_ENTERPRISE_DELETE          =  5;
		static const int ACTION_SHOW_PASSWORDS             =  6;
		static const int ACTION_NETWORK_ACCESSS            =  7;
		static const int ACTION_DATABASE_PASSWORD          =  8;
		static const int ACTION_DATABASE_VACUUM            =  9;
		static const int ACTION_MAIN_DATABASE_VACUUM       = 10;
		static const int ACTION_ABOUT                      = 11;

	public:
		AdminFrame(DSPostgreSQLDatabase *db);
		~AdminFrame();
	
	protected:
		enum {
			REFRESH_EVENT,
			EXECUTE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void RefreshEvent(wxCommandEvent &event);
		void ExecuteEvent(wxCommandEvent &event);

		void UpdateEnterpriseDatabase();
		void UpdateSpecial();
		void About();
		void RefreshList();
		void Recover();
		void Backup();
		void AddUser();
		void EnterpriseDelete();
		void Execute(const int action);
		void NetworkAccess();
		void DatabasePassword();
		void DatabaseVacuum();
		void MainDatabaseVacuum();
		unsigned int LoadEnterprises();

		DSPostgreSQLDatabase *db;
		bool showPasswords;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		wxStaticBitmap *logoStaticBitmap;
		wxStaticText *versionStaticText;
		DSListCtrl *enterprisesListCtrl;
		wxBoxSizer *optionsBoxSizer;
		wxButton *refreshButton;
		wxStaticText *actionStaticText;
		wxComboBox *actionsComboBox;
		wxButton *executeButton;

};

#endif /* DYNSOFT_WAREHOUSE_FRAME_H_ */
