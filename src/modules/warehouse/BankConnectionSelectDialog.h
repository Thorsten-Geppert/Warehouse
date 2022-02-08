#ifndef BANKSELECTIONSELECTDIALOG_H_
#define BANKSELECTIONSELECTDIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/BankConnectionEntities.h"
#include "../lib/BankConnectionEntity.h"

using namespace DynSoft;

class BankConnectionSelectDialog : public wxDialog {

	public:
		BankConnectionSelectDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			BankConnectionEntities &bankConnectionEntities
		);

		BankConnectionEntity *GetSelectedBankConnection() const;
	
	protected:
		enum {
			USE_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void UseEvent(wxMouseEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void UseSelectEvent(wxListEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Use();
		void Cancel();

		RuntimeInformationType *rit;
		BankConnectionEntity *bankConnectionEntity;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *useToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSListCtrl    *bankConnectionsListCtrl;

};

#endif /* BANKSELECTIONSELECTDIALOG_H_ */
