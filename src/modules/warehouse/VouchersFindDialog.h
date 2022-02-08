#ifndef VOUCHERSFINDDIALOG_H_
#define VOUCHERSFINDDIALOG_H_

#include <wx/wx.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/VoucherEntity.h"

using namespace DynSoft;

class VouchersFindDialog : public wxDialog {

	public:
		VouchersFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		VoucherEntity *GetVoucherEntity() const;
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

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *findToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSTextCtrl    *voucherIdTextCtrl;
		wxComboBox    *descriptionComboBox;
		wxTextCtrl    *invoiceIdTextCtrl;
		wxTextCtrl    *companyTextCtrl;
		wxTextCtrl    *notesTextCtrl;
		DSTextCtrl    *yearTextCtrl;

};

#endif /* VOUCHERSDIALOG_H_ */
