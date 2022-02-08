#ifndef ENTERPRISE_CREATE_DATABASE_DIALOG_H_
#define ENTERPRISE_CREATE_DATABASE_DIALOG_H_

#include <wx/wx.h>
#include <wx/gauge.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/DatabaseType.h"

using namespace DynSoft;

class EnterpriseCreateDatabaseDialog : public wxDialog {

	public:
		EnterpriseCreateDatabaseDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &company,
			DatabaseType *databaseType,
			const wxString &sql,
			const bool showErrorMessage = true
		);
	
	protected:
		enum {
			CANCEL_EVENT,
			RUN_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void RunEvent(wxMouseEvent &event);
		void RunKeyEvent(wxCommandEvent &event);

		void Cancel();
		void Run();

		RuntimeInformationType *rit;
		DatabaseType *databaseType;
		unsigned int enterprisePreferencesId;
		unsigned int commonPreferencesId;
		wxString sql;
		bool showErrorMessage;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *runToolbarButton;
		ToolbarButton *cancelToolbarButton;
		wxTextCtrl *companyTextCtrl;
		wxTextCtrl *executiveTextCtrl;
		wxTextCtrl *address1TextCtrl;
		wxTextCtrl *address2TextCtrl;
		wxTextCtrl *zipcodeTextCtrl;
		wxTextCtrl *cityTextCtrl;
		wxTextCtrl *salesTaxIdentificationNumberTextCtrl;
		wxTextCtrl *taxNumberTextCtrl;
		wxTextCtrl *salesTaxIndicatorTextCtrl;
		wxTextCtrl *telephoneTextCtrl;
		wxTextCtrl *faxTextCtrl;
		wxTextCtrl *emailTextCtrl;
		wxTextCtrl *webTextCtrl;
		wxTextCtrl *bankTextCtrl;
		DSTextCtrl *bankCodeTextCtrl;
		wxTextCtrl *holderTextCtrl;
		DSTextCtrl *accountNumberTextCtrl;
		DSTextCtrl *bicTextCtrl;
		DSTextCtrl *ibanTextCtrl;
		DSTextCtrl *minorValueAddedTaxTextCtrl;
		DSTextCtrl *majorValueAddedTaxTextCtrl;
		DSTextCtrl *clientIdBeginTextCtrl;
		DSTextCtrl *productIdBeginTextCtrl;
		DSTextCtrl *orderIdBeginTextCtrl;
		DSTextCtrl *invoiceIdBeginTextCtrl;
		DSTextCtrl *creditIdBeginTextCtrl;
		DSTextCtrl *purchaseIdBeginTextCtrl;

};

#endif /* ENTERPRISE_CREATE_DATABASE_DIALOG_H_ */
