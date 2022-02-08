#ifndef ORDERSFINDDIALOG_H_
#define ORDERSFINDDIALOG_H_

#include <wx/wx.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/OrderEntity.h"

using namespace DynSoft;

class OrdersFindDialog : public wxDialog {

	public:
		OrdersFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int clientId = 0
		);
		OrderEntity *GetOrderEntity() const;
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
		wxTextCtrl *orderIdTextCtrl;
		wxTextCtrl *invoiceIdTextCtrl;
		wxTextCtrl *clientIdTextCtrl;
		wxComboBox *kindComboBox;
		wxComboBox *salutationComboBox;
		wxComboBox *titleComboBox;
		wxTextCtrl *firstnameTextCtrl;
		wxTextCtrl *nameTextCtrl;
		wxTextCtrl *addressTextCtrl;
		wxTextCtrl *zipcodeTextCtrl;
		wxTextCtrl *cityTextCtrl;
		wxComboBox *landComboBox;
		wxCheckBox *onlyUnpayedCheckBox;
		DSTextCtrl *yearTextCtrl;

};

#endif /* ORDERSDIALOG_H_ */
