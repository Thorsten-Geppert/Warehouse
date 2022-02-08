#ifndef PURCHASESFINDDIALOG_H_
#define PURCHASESFINDDIALOG_H_

#include <wx/wx.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/PurchaseEntity.h"

class PurchasesFindDialog : public wxDialog {

	public:
		PurchasesFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int clientId = 0
		);
		PurchaseEntity *GetPurchaseEntity() const;
		int ShowModal();
	
	protected:
		enum {
			FIND_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void FindEvent(wxMouseEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Find();
		void Cancel();

		RuntimeInformationType *rit;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *findToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSTextCtrl *purchaseIdTextCtrl;
		DSTextCtrl *distributorIdTextCtrl;
		wxTextCtrl *distributorAddressContainsTextCtrl;
		wxTextCtrl *billingAddressContainsTextCtrl;
		wxTextCtrl *deliveryAddressContainsTextCtrl;
		DSTextCtrl *orderIdTextCtrl;
		wxTextCtrl *purchaserTextCtrl;
		wxTextCtrl *informationsTextCtrl;
		DSTextCtrl *yearTextCtrl;

};

#endif /* PURCHASESDIALOG_H_ */
