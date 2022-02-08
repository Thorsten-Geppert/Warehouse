#ifndef ORDEREDITFRAME_H_
#define ORDEREDITFRAME_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSBind.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include <DSDatePickerCtrl.h>
#include <DSCheckBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/OrderEntity.h"
#include "../lib/OrderPositionEntity.h"
#include "../lib/ClientEntity.h"
#include "../lib/OrderEntities.h"
#include "GuiCallbackInterface.h"
#include "EditedInformationsDialog.h"
#include "ClientOrdersDialog.h"
#include "MediaPanel.h"
#include "MoreDataPanel.h"

using namespace DynSoft;

class OrderEditFrame : public wxFrame, public GuiCallbackInterface {

	public:
		OrderEditFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			OrderEntity *orderEntity,
			GuiCallbackInterface *callbackInterface = NULL,
			const bool deleteOrderEntity = true
		);

		virtual void Callback(void *data);

	protected:
		enum {
			CLOSE_EVENT,
			SAVE_EVENT,
			PRINT_EVENT,
			PRINT_PREVIEW_EVENT,
			EDITED_INFORMATIONS_EVENT,
			FIND_CLIENT_EVENT,
			DOWN_POSITION_EVENT,
			UP_POSITION_EVENT,
			ADD_POSITION_EVENT,
			EDIT_POSITION_EVENT,
			REMOVE_POSITION_EVENT,
			INVOICING_EVENT,
			PRINT_NORMAL_EVENT,
			PRINT_DELIVERY_EVENT,
			PAYED_EVENT,
			DOWN_LINKED_ORDER_EVENT,
			UP_LINKED_ORDER_EVENT,
			UNLINK_ORDER_EVENT,
			LINK_ORDER_EVENT,
			NEGATE_POSITION_PRICES_EVENT,
			REMINDERS_EVENT
		};

		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void RemindersEvent(wxMouseEvent &event);
		void PrintPreviewEvent(wxMouseEvent &event);
		void FindClientEvent(wxMouseEvent &event);
		void DownPositionEvent(wxCommandEvent &event);
		void UpPositionEvent(wxCommandEvent &event);
		void AddPositionEvent(wxCommandEvent &event);
		void EditPositionEvent(wxCommandEvent &event);
		void RemovePositionEvent(wxCommandEvent &event);
		void DirectEditPositionEvent(wxListEvent &event);
		void DownLinkedOrderEvent(wxCommandEvent &event);
		void UpLinkedOrderEvent(wxCommandEvent &event);
		void UnlinkOrderEvent(wxCommandEvent &event);
		void LinkOrderEvent(wxCommandEvent &event);
		void NegatePositionPricesEvent(wxCommandEvent &event);
		void PayedEvent(wxMouseEvent &event);
		void InvoicingEvent(wxMouseEvent &event);
		void TransferAddressEvent(wxCommandEvent &event);
		void TransferBankEvent(wxCommandEvent &event);
		void ShowEditedInformationsEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void ShowEditedInformationsKeyEvent(wxCommandEvent &event);
		void FindClientKeyEvent(wxCommandEvent &event);
		void DownPositionKeyEvent(wxCommandEvent &event);
		void UpPositionKeyEvent(wxCommandEvent &event);
		void AddPositionKeyEvent(wxCommandEvent &event);
		void EditPositionKeyEvent(wxCommandEvent &event);
		void RemovePositionKeyEvent(wxCommandEvent &event);
		void InvoicingKeyEvent(wxCommandEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void RemindersKeyEvent(wxCommandEvent &event);
		void PrintPreviewKeyEvent(wxCommandEvent &event);
		void ClientIdEvent(wxCommandEvent &event);
		void RecalcPositionsEvent(wxCommandEvent &event);
		void PrintOutEvent(wxCommandEvent &event);
		void BankFindEvent(wxCommandEvent &event);
		void PayedKeyEvent(wxCommandEvent &event);
		void NegatePositionPricesKeyEvent(wxCommandEvent &event);
		void AddressTranslateEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);

		void DownLinkedOrderKeyEvent(wxCommandEvent &event);
		void UpLinkedOrderKeyEvent(wxCommandEvent &event);
		void UnlinkOrderKeyEvent(wxCommandEvent &event);
		void LinkOrderKeyEvent(wxCommandEvent &event);

		void Close();
		bool Save();
		void FindClient();
		void AddPosition();
		void EditPosition(OrderPositionEntity *orderPositionEntity);
		void RemovePosition(OrderPositionEntity *orderPositionEntity);
		void AddOrChangeToOrderPositionsListCtrl(OrderPositionEntity *orderPositionEntity, const bool justAppend, const bool dontRecalcPositions = false);
		void AddToLinkedOrdersListCtrl(OrderEntity *linkedOrderEntity);
		void EnableClientIdField();
		void TransferAddress(const int objectId);
		void TransferBank();
		void ShowEditedInformations();
		void Invoicing();
		void Print(const bool preview = false);
		void PrintOut(const bool delivery);
		void ClientId(const unsigned int clientId);
		void SetDisabled();
		bool IfInvoicedShowThis();
		bool IfLinkedShowThis();
		ClientEntity *GetClientEntity(const unsigned int clientId);
		void BankFind();
		void Payed();
		void SetPayed();
		void DownPosition();
		void UpPosition();
		void DownLinkedOrder();
		void UpLinkedOrder();
		void UnlinkOrder();
		void LinkOrder();
		void AddToRemovedOrderArray(const unsigned int orderId);
		void RemoveFromRemovedOrderArray(const unsigned int orderId);
		void Transfer(OrderEntity *orderEntity = NULL, const bool negate = false);
		bool IsInvoiced();
		void NegatePositionPrices();
		void AddressTranslate(const int id);
		void SetTranslatedAddressIcons();
		void TemplateSelect(wxObject *object);
		void Reminders();
		bool IsClientActive(const bool active);
		void ChooseClientMessage();

		RuntimeInformationType *rit;
		DSBind bind;
		unsigned int orderId;
		unsigned int clientId;
		GuiCallbackInterface *callbackInterface;
		wxArrayString orderPositionsRemoved;
		bool changed;
		EditedInformationsDialog *editedInformationsDialog;
		bool preview;
		bool loaded;
		ClientOrdersDialog *clientOrdersDialog;
		bool deleteOrderEntity;
		OrderEntities linkedOrderEntities;
		OrderIdArray removedOrderArray;
		wxString shippingAddressTranslated, billAddressTranslated;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		ToolbarButton    *saveToolbarButton;
		ToolbarButton    *findClientToolbarButton;
		ToolbarButton    *closeToolbarButton;
		ToolbarButton    *editedInformationsToolbarButton;
		ToolbarButton    *invoicingToolbarButton;
		ToolbarButton    *printToolbarButton;
		ToolbarButton    *printPreviewToolbarButton;
		ToolbarButton    *payedToolbarButton;
		ToolbarButton    *remindersToolbarButton;
		Toolbar          *mainToolbar;
		DSComboBox       *kindComboBox;
		DSTextCtrl       *clientIdTextCtrl;
		DSTextCtrl       *ownClientIdTextCtrl;
		DSTextCtrl       *taxNumberTextCtrl;
		DSTextCtrl       *salesTaxIdentificationNumberTextCtrl;
		DSTextCtrl       *orderIdTextCtrl;
		DSTextCtrl       *basedOnTextCtrl;
		DSComboBox       *taxRateComboBox;
		DSDatePickerCtrl *dateDatePickerCtrl;
		DSTextCtrl       *ownerTextCtrl;
		wxButton         *negatePositionPricesButton;
		DSComboBox       *billAddressSalutationComboBox;
		DSComboBox       *billAddressTitleComboBox;
		DSTextCtrl       *billAddressFirstnameTextCtrl;
		DSTextCtrl       *billAddressName1TextCtrl;
		DSTextCtrl       *billAddressName2TextCtrl;
		DSTextCtrl       *billAddressName3TextCtrl;
		DSTextCtrl       *billAddressName4TextCtrl;
		DSTextCtrl       *billAddressAddressTextCtrl;
		DSTextCtrl       *billAddressNumberTextCtrl;
		DSTextCtrl       *billAddressZipcodeTextCtrl;
		DSTextCtrl       *billAddressCityTextCtrl;
		DSComboBox       *billAddressFederalStateComboBox;
		DSComboBox       *billAddressLandComboBox;
		wxButton         *billAddressTransferButton;
		wxButton         *billAddressTranslateEditButton;
		wxStaticBitmap   *billAddressTranslatedStaticImage;
		DSComboBox       *shippingAddressSalutationComboBox;
		DSComboBox       *shippingAddressTitleComboBox;
		DSTextCtrl       *shippingAddressFirstnameTextCtrl;
		DSTextCtrl       *shippingAddressName1TextCtrl;
		DSTextCtrl       *shippingAddressName2TextCtrl;
		DSTextCtrl       *shippingAddressName3TextCtrl;
		DSTextCtrl       *shippingAddressName4TextCtrl;
		DSTextCtrl       *shippingAddressAddressTextCtrl;
		DSTextCtrl       *shippingAddressNumberTextCtrl;
		DSTextCtrl       *shippingAddressZipcodeTextCtrl;
		DSTextCtrl       *shippingAddressCityTextCtrl;
		DSComboBox       *shippingAddressFederalStateComboBox;
		DSComboBox       *shippingAddressLandComboBox;
		wxButton         *shippingAddressTransferButton;
		wxButton         *shippingAddressTranslateEditButton;
		wxStaticBitmap   *shippingAddressTranslatedStaticImage;
		wxPanel          *positionsPanel;
		wxBoxSizer       *positionsBoxSizer;
		DSListCtrl       *orderPositionsListCtrl;
		wxButton         *downPositionButton;
		wxButton         *upPositionButton;
		wxButton         *removePositionButton;
		wxButton         *editPositionButton;
		wxButton         *addPositionButton;
		wxPanel          *linkedOrdersPanel;
		wxBoxSizer       *linkedOrdersBoxSizer;
		DSListCtrl       *linkedOrdersListCtrl;
		wxButton         *downLinkedOrderButton;
		wxButton         *upLinkedOrderButton;
		wxButton         *unlinkOrderButton;
		wxButton         *linkOrderButton;
		DSComboBox       *conditionComboBox;
		DSComboBox       *paymentComboBox;
		DSCheckBox       *monitionCheckBox;
		wxStaticText     *monitionStateStaticText;
		DSTextCtrl       *bankTextCtrl;
		DSTextCtrl       *bankCodeTextCtrl;
		DSTextCtrl       *accountNumberTextCtrl;
		DSTextCtrl       *bicTextCtrl;
		DSTextCtrl       *ibanTextCtrl;
		DSTextCtrl       *bankOwnerTextCtrl;
		DSCheckBox       *liableToTaxCheckBox;
		wxStatusBar      *mainStatusBar;
		DSTextCtrl       *orderNumberTextCtrl;
		DSComboBox       *deliveryComboBox;
		wxStaticText     *appendTextStaticText;
		DSTextCtrl       *prepandTextTextCtrl;
		wxStaticText     *prepandTextStaticText;
		DSTextCtrl       *appendTextTextCtrl;
		wxButton         *transferBankButton;
		wxButton         *findBankButton;
		wxBoxSizer       *mediaBoxSizer;
		MediaPanel       *mediaPanel;
		wxBoxSizer       *moreDataBoxSizer;
		MoreDataPanel    *moreDataPanel;

};

#endif /* ORDEREDITFRAME_H_ */
