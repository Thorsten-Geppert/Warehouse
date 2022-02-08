#ifndef CLIENTORDERSDIALOG_H_
#define CLIENTORDERSDIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/OrderEntity.h"
#include "../lib/OrderEntities.h"
#include "GuiCallbackInterface.h"

WX_DEFINE_ARRAY_INT(unsigned int, OrderIdArray);

using namespace DynSoft;

class ClientOrdersDialog : public wxDialog, public GuiCallbackInterface {

	public:
		ClientOrdersDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int clientId,
			const bool showUse                      = false,
			GuiCallbackInterface *callbackInterface = NULL,
			const unsigned int exclude              = 0,      // Order id
			OrderEntities *excludes                 = NULL,   // Order ids
			OrderIdArray *includes                  = NULL,   // Order ids
			const bool showLinkedOrders             = true,
			const bool linkMode                     = false   // No Credits, Bids, Bills
		);
		~ClientOrdersDialog();
		virtual void Callback(void *data);
		bool RemoveSelected();
		void SetExclude(const unsigned int exclude);
		void SetClientId(const unsigned int clientId);
	
	protected:
		enum {
			CLOSE_EVENT,
			USE_EVENT,
			UPDATE_EVENT,
			NEW_EVENT,
			EDIT_EVENT,
			REMOVE_EVENT,
			FIND_EVENT,
			EXPORT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void UpdateEvent(wxMouseEvent &event);
		void UseEvent(wxMouseEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void UpdateKeyEvent(wxCommandEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
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

		void Use();
		void Cancel();
		void New(const unsigned int *orderId);
		void Edit(const unsigned int *orderId);
		void Remove();
		void Find();
		void Export();
		void ClearOrdersListCtrl();
		void AddOrChangeToOrdersListCtrl(OrderEntity *orderEntity, const bool justAppend = false);
		void Update();
		bool CheckProfile();

		RuntimeInformationType *rit;
		unsigned int clientId;
		GuiCallbackInterface *callbackInterface;
		unsigned int exclude;
		OrderEntities *excludes;
		OrderIdArray *includes;
		bool showLinkedOrders;
		bool linkMode;

		ToolbarButton *updateToolbarButton;
		ToolbarButton *newToolbarButton;
		ToolbarButton *editToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *findToolbarButton;
		ToolbarButton *exportToolbarButton;
		ToolbarButton *useToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSListCtrl    *ordersListCtrl;

};

#endif /* CLIENTSFULLDIALOG_H_ */
