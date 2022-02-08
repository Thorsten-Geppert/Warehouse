#ifndef PURCHASESMODULEPANEL_H_
#define PURCHASESMODULEPANEL_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/Toolbar.h"
#include "../lib/PurchaseEntity.h"
#include "GuiCallbackInterface.h"
#include "PurchasesFindDialog.h"

using namespace DynSoft;

class PurchasesModulePanel : public ModulePanel, public GuiCallbackInterface {

	public:
		PurchasesModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~PurchasesModulePanel();

		virtual void Load();
		virtual void Unload();
		virtual void Callback(void *data);
	
	protected:
		enum {
			NEW_EVENT,
			COPY_EVENT,
			EDIT_EVENT,
			REMOVE_EVENT,
			FIND_EVENT,
			PAYED_EVENT,
			MONITIONS_EVENT,
			EXPORT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void NewEvent(wxMouseEvent &event);
		void CopyEvent(wxMouseEvent &event);
		void EditEvent(wxMouseEvent &event);
		void DirectEditEvent(wxListEvent &event);
		void RemoveEvent(wxMouseEvent &event);
		void FindEvent(wxMouseEvent &event);
		void NewKeyEvent(wxCommandEvent &event);
		void CopyKeyEvent(wxCommandEvent &event);
		void EditKeyEvent(wxCommandEvent &event);
		void RemoveKeyEvent(wxCommandEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);

		void New();
		void Copy(const unsigned int *purchaseId);
		void Edit(const unsigned int *purchaseId);
		void Remove();
		void Find();
		void Export();

		void AddOrChangeToPurchasesListCtrl(PurchaseEntity *purchaseEntity, const bool justAppend);
		void ClearPurchasesListCtrl();

		// Stuff
		PurchasesFindDialog *purchasesFindDialog;

		ToolbarButton *newToolbarButton;
		ToolbarButton *copyToolbarButton;
		ToolbarButton *editToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *findToolbarButton;
		ToolbarButton *exportToolbarButton;
		DSListCtrl    *purchasesListCtrl;

};

#endif /* PURCHASESMODULEPANEL_H_ */
