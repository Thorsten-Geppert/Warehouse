#ifndef VOUCHERSMODULEPANEL_H_
#define VOUCHERSMODULEPANEL_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/Toolbar.h"
#include "../lib/VoucherEntity.h"
#include "GuiCallbackInterface.h"
#include "VouchersFindDialog.h"
#include "CashJournalFrame.h"

using namespace DynSoft;

class VouchersModulePanel : public ModulePanel, public GuiCallbackInterface {

	public:
		VouchersModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~VouchersModulePanel();

		virtual void Load();
		virtual void Unload();
		virtual void Callback(void *data);
	
	protected:
		enum {
			NEW_EVENT,
			EDIT_EVENT,
			REMOVE_EVENT,
			FIND_EVENT,
			CASH_JOURNAL_EVENT,
			EXPORT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void NewEvent(wxMouseEvent &event);
		void EditEvent(wxMouseEvent &event);
		void DirectEditEvent(wxListEvent &event);
		void RemoveEvent(wxMouseEvent &event);
		void FindEvent(wxMouseEvent &event);
		void NewKeyEvent(wxCommandEvent &event);
		void EditKeyEvent(wxCommandEvent &event);
		void RemoveKeyEvent(wxCommandEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);
		void CashJournalEvent(wxMouseEvent &event);
		void CashJournalKeyEvent(wxCommandEvent &event);

		void New(unsigned int *voucherId);
		void Edit(unsigned int *voucherId);
		void Remove();
		void Find();
		void Export();
		void CashJournal();

		void AddOrChangeToVouchersListCtrl(VoucherEntity *voucherEntity, const bool justAppend);
		void ClearVouchersListCtrl();

		// Stuff
		VouchersFindDialog *vouchersFindDialog;
		CashJournalFrame *cashJournalFrame;

		ToolbarButton *newToolbarButton;
		ToolbarButton *editToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *findToolbarButton;
		ToolbarButton *cashJournalToolbarButton;
		ToolbarButton *exportToolbarButton;
		DSListCtrl    *vouchersListCtrl;

};

#endif /* VOUCHERSMODULEPANEL_H_ */
