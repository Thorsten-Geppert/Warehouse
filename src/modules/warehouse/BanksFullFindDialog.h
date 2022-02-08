#ifndef BANKSFULLFINDDIALOG_H_
#define BANKSFULLFINDDIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/BankEntity.h"

using namespace DynSoft;

class BanksFullFindDialog : public wxDialog {

	public:
		BanksFullFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~BanksFullFindDialog();
		BankEntity *GetBankEntity() const;
		int ShowModal();
	
	protected:
		enum {
			CLOSE_EVENT,
			FIND_EVENT,
			USE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void FindEvent(wxMouseEvent &event);
		void UseEvent(wxMouseEvent &event);
		void UseDirectEvent(wxListEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Find();
		void Use();
		void Cancel();
		void ClearBanksListCtrl();
		void AddToBanksListCtrl(BankEntity *bankEntity);

		RuntimeInformationType *rit;
		BankEntity *bankEntity;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *findToolbarButton;
		ToolbarButton *useToolbarButton;
		ToolbarButton *cancelToolbarButton;
		wxTextCtrl *bankTextCtrl;
		DSTextCtrl  *bankCodeTextCtrl;
		DSTextCtrl  *bicTextCtrl;
		DSListCtrl *banksListCtrl;

};

#endif /* BANKSFULLDIALOG_H_ */
