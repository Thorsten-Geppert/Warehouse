#ifndef ORDERSMODULEPANEL_H_
#define ORDERSMODULEPANEL_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/Toolbar.h"
#include "GuiCallbackInterface.h"
#include "OrdersFindDialog.h"
#include "MonitionsFrame.h"

using namespace DynSoft;

class OrdersModulePanel : public ModulePanel, public GuiCallbackInterface {

	public:
		OrdersModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~OrdersModulePanel();

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
		void PayedEvent(wxMouseEvent &event);
		void PayedKeyEvent(wxCommandEvent &event);
		void MonitionsEvent(wxMouseEvent &event);
		void MonitionsKeyEvent(wxCommandEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);

		void New(const unsigned int *orderId);
		void Copy(const unsigned int *orderId);
		void Edit(const unsigned int *orderId);
		void Remove();
		void Find();
		void Payed(const unsigned int *orderId);
		void Monitions();
		void Export();

		void AddOrChangeToOrdersListCtrl(OrderEntity *orderEntity, const bool justAppend);
		void ClearOrdersListCtrl();
		bool CheckProfile();
		bool IsInvoiced(const unsigned int orderId);
		bool IfLinkedShowThis(const unsigned int orderId);

		// Stuff
		OrdersFindDialog *ordersFindDialog;
		MonitionsFrame   *monitionsFrame;

		ToolbarButton *newToolbarButton;
		ToolbarButton *copyToolbarButton;
		ToolbarButton *editToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *findToolbarButton;
		ToolbarButton *payedToolbarButton;
		ToolbarButton *monitionsToolbarButton;
		ToolbarButton *exportToolbarButton;
		DSListCtrl    *ordersListCtrl;

};

#endif /* ORDERSMODULEPANEL_H_ */
