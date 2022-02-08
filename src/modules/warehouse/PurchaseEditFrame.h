#ifndef PURCHASEEDITFRAME_H_
#define PURCHASEEDITFRAME_H_

#include <wx/wx.h>
#include <DSBind.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include <DSDatePickerCtrl.h>
#include <DSCheckBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/PurchaseEntity.h"
#include "GuiCallbackInterface.h"
#include "EditedInformationsDialog.h"
#include "MediaPanel.h"
#include "MoreDataPanel.h"

using namespace DynSoft;

class PurchaseEditFrame : public wxFrame {

	public:
		PurchaseEditFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			PurchaseEntity *purchaseEntity,
			GuiCallbackInterface *callbackInterface = NULL,
			const bool deletePurchaseEntity = true,
			const bool copy = false
		);

	protected:
		DECLARE_EVENT_TABLE()

		enum {
			CLOSE_EVENT,
			FIND_DISTRIBUTOR_BUTTON,
			PASTE_BILLING_ADDRESS_BUTTON,
			PASTE_DELIVERY_ADDRESS_BUTTON,
			ADD_POSITION_EVENT,
			EDIT_POSITION_EVENT,
			REMOVE_POSITION_EVENT,
			SAVE_EVENT,
			EDITED_INFORMATIONS_EVENT,
			REMINDERS_EVENT,
			DONE_EVENT,
			PRINT_EVENT,
			PRINT_PREVIEW_EVENT
		};
		
		void OnClose(wxCloseEvent &event);
		void CloseEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);
		void FindDistributorEvent(wxCommandEvent &event);
		void FindDistributorKeyEvent(wxCommandEvent &event);
		void PasteAddressEvent(wxCommandEvent &event);
		void PasteAddressKeyEvent(wxCommandEvent &event);
		void AddPositionEvent(wxCommandEvent &event);
		void EditPositionEvent(wxCommandEvent &event);
		void DirectEditPositionEvent(wxListEvent &event);
		void RemovePositionEvent(wxCommandEvent &event);
		void AddPositionKeyEvent(wxCommandEvent &event);
		void EditPositionKeyEvent(wxCommandEvent &event);
		void RemovePositionKeyEvent(wxCommandEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void ShowEditedInformationsEvent(wxMouseEvent &event);
		void ShowEditedInformationsKeyEvent(wxCommandEvent &event);
		void RemindersEvent(wxMouseEvent &event);
		void RemindersKeyEvent(wxCommandEvent &event);
		void DoneEvent(wxMouseEvent &event);
		void DoneKeyEvent(wxCommandEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void PrintPreviewEvent(wxMouseEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void PrintPreviewKeyEvent(wxCommandEvent &event);

		void Close();
		void AddPosition();
		void EditPosition(PurchasePositionEntity *PurchasePositionEntity);
		void RemovePosition(PurchasePositionEntity *PurchasePositionEntity);
		void TemplateSelect(wxObject *object);
		void FindDistributor();
		void PasteAddress(const int buttonId);
		void AddOrChangeToPurchasePositionsListCtrl(PurchasePositionEntity *purchasePositionEntity, const bool dontRecalcPositions = false);
		void Transfer(PurchaseEntity *purchaseEntity = NULL);
		bool Save();
		void ShowEditedInformations();
		void Reminders();
		void Done();
		void SetDone();
		void SetDisabled();
		void Print(const bool preview = false);

		DSBind bind;
		bool changed;
		bool isDone;
		RuntimeInformationType *rit;
		unsigned int purchaseId;
		GuiCallbackInterface *callbackInterface;
		wxArrayString purchasePositionsRemoved;
		EditedInformationsDialog *editedInformationsDialog;
		bool deletePurchaseEntity;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton    *saveToolbarButton;
		ToolbarButton    *printToolbarButton;
		ToolbarButton    *printPreviewToolbarButton;
		ToolbarButton    *doneToolbarButton;
		ToolbarButton    *editedInformationsToolbarButton;
		ToolbarButton    *remindersToolbarButton;
		ToolbarButton    *closeToolbarButton;
		Toolbar          *mainToolbar;
		DSTextCtrl       *distributorIdTextCtrl;
		DSTextCtrl       *distributorAddressTextCtrl;
		wxButton         *findDistributorButton;
		DSTextCtrl       *clientIdTextCtrl;
		DSTextCtrl       *orderIdTextCtrl;
		DSDatePickerCtrl *orderDateDatePickerCtrl;
		DSTextCtrl       *purchaserTextCtrl;
		DSTextCtrl       *purchaseIdTextCtrl;
		DSTextCtrl       *informationsTextCtrl;
		DSTextCtrl       *billingAddressTextCtrl;
		wxButton         *pasteBillingAddressButton;
		DSTextCtrl       *deliveryAddressTextCtrl;
		wxButton         *pasteDeliveryAddressButton;
		wxStaticText     *textHeadStaticText;
		DSTextCtrl       *textHeadTextCtrl;
		wxStaticText     *textFootStaticText;
		DSTextCtrl       *textFootTextCtrl;
		DSListCtrl       *purchasePositionsListCtrl;
		wxButton         *removePositionButton;
		wxButton         *editPositionButton;
		wxButton         *addPositionButton;
		wxStatusBar      *mainStatusBar;

		wxBoxSizer       *mediaBoxSizer;
		MediaPanel       *mediaPanel;
		wxBoxSizer       *moreDataBoxSizer;
		MoreDataPanel    *moreDataPanel;

};

#endif /* PURCHASEEDITFRAME_H_ */
