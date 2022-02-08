#include "RemoveDialog.h"
#include <DSCasts.h>
#include "../lib/ReferenceType.h"
#include "../lib/ConstantsLibrary.h"
#include "../lib/BlockType.h"
#include "../lib/OrderController.h"
#include "../lib/OrderEntity.h"
#include "../lib/ConstantsLibrary.h"
#include "OrderEditFrame.h"
#include "OrdersModulePanel.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(RemoveDialog, wxDialog)
	EVT_MENU(CLOSE_EVENT, RemoveDialog::CancelKeyEvent)
	EVT_MENU(OPEN_EVENT, RemoveDialog::OpenKeyEvent)
	EVT_LISTBOX_DCLICK(XRCID("itemsListBox"), RemoveDialog::OpenDirectEvent)
END_EVENT_TABLE()

RemoveDialog::RemoveDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	ReferencesType *referencesType
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("RemoveDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("RemoveDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	openToolbarButton   = XRCCTRL(*this, "openToolbarButton", ToolbarButton);
	itemsListBox        = XRCCTRL(*this, "itemsListBox", wxListBox);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(openToolbarButton, RemoveDialog::OpenEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, RemoveDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'o',  OPEN_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Fill
	if(!referencesType)
		return;

	ReferenceType *referenceType = NULL;
	const unsigned int count = referencesType->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		referenceType = static_cast<ReferenceType *>(referencesType->Get(i));
		itemsListBox->Append(
			ConstantsLibrary::KindToString(referenceType->GetKind()) +
			_N(": ") +
			DSCasts::ToString(referenceType->GetId()) +
			_N(" ") +
			referenceType->GetMessage(),
			referenceType
		);
	}

	ICON();
}

void RemoveDialog::OpenEvent(wxMouseEvent &event) {
	Open();
	SKIP();
}

void RemoveDialog::OpenDirectEvent(wxCommandEvent &event) {
	Open();
}

void RemoveDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void RemoveDialog::OpenKeyEvent(wxCommandEvent &event) {
	Open();
}

void RemoveDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void RemoveDialog::Open() {
	ReferenceType *referenceType = static_cast<ReferenceType *>(itemsListBox->GetClientData(itemsListBox->GetSelection()));
	if(!referenceType) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the entry you want to open."),
			_("Open")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	if(referenceType->GetKind() == ConstantsLibrary::ORDER_KIND) {
		const unsigned int orderId = referenceType->GetId();
		BlockType *blockType = rit->IsBlocked(_N("orders") , orderId);
		if(blockType) {
			OrderEditFrame *orderEditFrame = static_cast<OrderEditFrame *>(blockType->GetData());
			if(orderEditFrame)
				orderEditFrame->Raise();
		} else {
			OrderController orderController(rit);
			orderController.Get(DSCasts::ToString(orderId));
			if(orderController.Run()) {
				OrderEntity *orderEntity = new OrderEntity(rit);
				orderController.Get(orderEntity);

				OrdersModulePanel *ordersModulePanel = static_cast<OrdersModulePanel *>(rit->GetObject(_N("ordersModulePanel")));
				OrderEditFrame *orderEditFrame = new OrderEditFrame(ordersModulePanel, -1, rit, orderEntity, ordersModulePanel);
				orderEditFrame->Center();
				orderEditFrame->Show();

				rit->Block(_N("orders"), orderEntity->Get(_N("orderId"))->ToUnsignedInt(), orderEditFrame);
			} else {
				wxMessageDialog *errorMessageDialog = new wxMessageDialog(
					this,
					_("Could not find the order in the database."),
					_("Edit order")
				);
				errorMessageDialog->Center();
				errorMessageDialog->ShowModal();
				errorMessageDialog->Destroy();
			}
		}
		Cancel();
	}
}

void RemoveDialog::Cancel() {
	EndModal(wxID_CANCEL);
}
