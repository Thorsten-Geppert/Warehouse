#include "OrdersModulePanel.h"
#include <wx/xrc/xmlres.h>
#include "OrderEditFrame.h"
#include "ProgressDialog.h"
#include "RemoveDialog.h"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"
#include "YesNoDialog.h"
#include "PayedDialog.h"

#include "../lib/PermissionLibrary.h"
#include "../lib/OrderController.h"
#include "../lib/NameType.h"
#include "../lib/AddressType.h"
#include "../lib/ConstantsLibrary.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"
#include "../lib/EnterprisePreferencesController.h"
#include "../lib/CommonPreferencesController.h"

BEGIN_EVENT_TABLE(OrdersModulePanel, ModulePanel)
	EVT_LIST_ITEM_ACTIVATED(XRCID("ordersListCtrl"), OrdersModulePanel::DirectEditEvent)
	EVT_MENU(NEW_EVENT, OrdersModulePanel::NewKeyEvent)
	EVT_MENU(COPY_EVENT, OrdersModulePanel::CopyKeyEvent)
	EVT_MENU(EDIT_EVENT, OrdersModulePanel::EditKeyEvent)
	EVT_MENU(REMOVE_EVENT, OrdersModulePanel::RemoveKeyEvent)
	EVT_MENU(FIND_EVENT, OrdersModulePanel::FindKeyEvent)
	EVT_MENU(PAYED_EVENT, OrdersModulePanel::PayedKeyEvent)
	EVT_MENU(MONITIONS_EVENT, OrdersModulePanel::MonitionsKeyEvent)
	EVT_MENU(EXPORT_EVENT, OrdersModulePanel::ExportKeyEvent)
END_EVENT_TABLE()

OrdersModulePanel::OrdersModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("OrdersModulePanel")
) {
	// Toolbar
	newToolbarButton       = XRCCTRL(*this, "newToolbarButton", ToolbarButton);
	copyToolbarButton      = XRCCTRL(*this, "copyToolbarButton", ToolbarButton);
	editToolbarButton      = XRCCTRL(*this, "editToolbarButton", ToolbarButton);
	removeToolbarButton    = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	findToolbarButton      = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	payedToolbarButton     = XRCCTRL(*this, "payedToolbarButton", ToolbarButton);
	monitionsToolbarButton = XRCCTRL(*this, "monitionsToolbarButton", ToolbarButton);
	exportToolbarButton    = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);
	ordersListCtrl         = XRCCTRL(*this, "ordersListCtrl", DynSoft::DSListCtrl);

	ordersListCtrl->InsertColumn(0, _("Payed"), wxLIST_FORMAT_LEFT, 100);
	ordersListCtrl->InsertColumn(1, _("Kind"), wxLIST_FORMAT_LEFT, 80);
	ordersListCtrl->InsertColumn(2, _("Date"), wxLIST_FORMAT_LEFT, 120);
	ordersListCtrl->InsertColumn(3, _("Order id"), wxLIST_FORMAT_RIGHT, 120);
	ordersListCtrl->InsertColumn(4, _("Invoice id"), wxLIST_FORMAT_RIGHT, 140);
	ordersListCtrl->InsertColumn(5, _("Customer id"), wxLIST_FORMAT_RIGHT, 120);
	ordersListCtrl->InsertColumn(6, _("Own customer id"), wxLIST_FORMAT_RIGHT, 170);
	ordersListCtrl->InsertColumn(7, _("Billing address"), wxLIST_FORMAT_LEFT, 300);
	ordersListCtrl->InsertColumn(8, _("Shipping address"), wxLIST_FORMAT_LEFT, 300);
	ordersListCtrl->InsertColumn(9, _("Price"), wxLIST_FORMAT_RIGHT, 90);
	ordersListCtrl->InsertColumn(10, _("Full tax"), wxLIST_FORMAT_RIGHT, 150);
	ordersListCtrl->InsertColumn(11, _("Reduced tax"), wxLIST_FORMAT_RIGHT, 150);
	ordersListCtrl->InsertColumn(12, _("Sum"), wxLIST_FORMAT_RIGHT, 90);

	// Events
	CONNECT_TOOLBARBUTTON(newToolbarButton, OrdersModulePanel::NewEvent);
	CONNECT_TOOLBARBUTTON(copyToolbarButton, OrdersModulePanel::CopyEvent);
	CONNECT_TOOLBARBUTTON(editToolbarButton, OrdersModulePanel::EditEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, OrdersModulePanel::RemoveEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, OrdersModulePanel::FindEvent);
	CONNECT_TOOLBARBUTTON(payedToolbarButton, OrdersModulePanel::PayedEvent);
	CONNECT_TOOLBARBUTTON(monitionsToolbarButton, OrdersModulePanel::MonitionsEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, OrdersModulePanel::ExportEvent);

	wxAcceleratorEntry acceleratorEntries[8];
	acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 'n', NEW_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, (int) 'e', EDIT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL, (int) 'd', REMOVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL, (int) 'f', FIND_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL, (int) 'm', MONITIONS_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL, (int) 'k', EXPORT_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL, (int) 'c', COPY_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL, (int) 'b', PAYED_EVENT);
	wxAcceleratorTable acceleratorTable(8, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Stuff
	ordersFindDialog = NULL;

	monitionsFrame = new MonitionsFrame(this, -1, rit);
	monitionsFrame->CenterOnScreen();
}

OrdersModulePanel::~OrdersModulePanel() {
	ClearOrdersListCtrl();
}

void OrdersModulePanel::Load() {
	ordersListCtrl->SetFocus();
}

void OrdersModulePanel::Unload() {
}

void OrdersModulePanel::NewEvent(wxMouseEvent &event) {
	New(static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer()));
	SKIP();
}

void OrdersModulePanel::EditEvent(wxMouseEvent &event) {
	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());
	Edit(orderId);
	SKIP();
}

void OrdersModulePanel::DirectEditEvent(wxListEvent &event) {
	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());
	Edit(orderId);
}

void OrdersModulePanel::RemoveEvent(wxMouseEvent &event) {
	Remove();
	SKIP();
}

void OrdersModulePanel::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void OrdersModulePanel::NewKeyEvent(wxCommandEvent &event) {
	New(static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer()));
}

void OrdersModulePanel::EditKeyEvent(wxCommandEvent &event) {
	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());
	Edit(orderId);
}

void OrdersModulePanel::RemoveKeyEvent(wxCommandEvent &event) {
	Remove();
}

void OrdersModulePanel::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void OrdersModulePanel::MonitionsEvent(wxMouseEvent &event) {
	Monitions();
	SKIP();
}

void OrdersModulePanel::MonitionsKeyEvent(wxCommandEvent &event) {
	Monitions();
}

void OrdersModulePanel::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void OrdersModulePanel::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void OrdersModulePanel::CopyEvent(wxMouseEvent &event) {
	Copy(static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer()));
	SKIP();
}

void OrdersModulePanel::CopyKeyEvent(wxCommandEvent &event) {
	Copy(static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer()));
}

void OrdersModulePanel::PayedEvent(wxMouseEvent &event) {
	Payed(static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer()));
	SKIP();
}

void OrdersModulePanel::PayedKeyEvent(wxCommandEvent &event) {
	Payed(static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer()));
}

void OrdersModulePanel::New(const unsigned int *orderId) {
	ordersListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to create a new order."), this))
		return;

	CheckProfile();

	OrderEntity *orderEntity = new OrderEntity(rit);
	if(orderId && orderEntity->Get(_N("kind"))->ToUnsignedInt() < ConstantsLibrary::ORDER_KIND_CREDIT) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you want to create an order/a credit based on the selected one?"),
			_("New order"),
			_("Create based on"),
			_("Do not create based on")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES) {
			OrderController orderController(rit);
			orderController.Get(DSCasts::ToString(*orderId));
			if(orderController.Run()) {
				orderController.Get(orderEntity);
				orderController.Reset();
				orderController.Change(orderEntity);
			} else {
				YesNoDialog *yesNoDialog = new YesNoDialog(
					this,
					-1,
					rit,
					_("Could not create an order based on the selected one. Will create a new order. Maybe the selected order is not in database anymore."),
					_("New order"),
					_("Create"),
					_("Cancel")
				);
				yesNoDialog->Center();
				yesNoDialog->ShowModal();
				yesNoDialog->Destroy();
			}
		}
		yesNoDialog->Destroy();
	}

	OrderEditFrame *orderEditFrame = new OrderEditFrame(this, -1, rit, orderEntity, this);
	orderEditFrame->Center();
	orderEditFrame->Show();
}

void OrdersModulePanel::Edit(const unsigned int *orderId) {
	ordersListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to open an order."), this))
		return;

	if(!orderId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the order you want to edit."),
			_("Edit order")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	CheckProfile();

	BlockType *blockType = rit->IsBlocked(_N("orders"), *orderId);
	if(blockType) {
		OrderEditFrame *orderEditFrame = static_cast<OrderEditFrame *>(blockType->GetData());
		if(orderEditFrame) {
			orderEditFrame->Raise();
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The order you want to edit is allready opened."),
				_("Edit order")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
		return;
	}

	OrderController orderController(rit);
	orderController.Get(DSCasts::ToString(*orderId));
	if(orderController.Run()) {
		OrderEntity *orderEntity = new OrderEntity(rit);
		orderController.Get(orderEntity);
		orderController.Reset();

		OrderEditFrame *orderEditFrame = new OrderEditFrame(this, -1, rit, orderEntity, this);
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

void OrdersModulePanel::Remove() {
	ordersListCtrl->SetFocus();

	if(!rit->Access(A_DELETE, _("You have no permission to remove an order."), this))
		return;

	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());

	if(!orderId) {
		rit->Log(true, _N("OrderModulePanel"), _("Could not remove order."), _("No order selected."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the order you want to remove."),
			_("Remove order")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	OrderController orderController(rit);
	if(orderController.IsInvoiced(*orderId)) {
		rit->Log(true, _N("OrderModulePanel"), _("Could not remove order."), _("Order is invoiced."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The order is invoiced. You cannot remove invoiced orders."),
			_("Remove order")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("orders") , *orderId);
	if(blockType) {
		rit->Log(true, _N("OrderModulePanel"), _("Could not remove order."), _("Order is open."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The order you want to remove is open."),
			_("Remove order")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		OrderEditFrame *orderEditFrame = static_cast<OrderEditFrame *>(blockType->GetData());
		if(orderEditFrame)
			orderEditFrame->Raise();

		return;
	}

	ReferencesType referencesType;
	orderController.GetReferences(ConstantsLibrary::ORDER_KIND, *orderId, &referencesType);
	if(referencesType.GetCount() != 0) {
		rit->Log(true, _N("OrderModulePanel"), _("Could not remove order."), _("Order is referenced with other data."));

		RemoveDialog *removeDialog = new RemoveDialog(this, -1, rit, &referencesType);
		removeDialog->Center();
		removeDialog->ShowModal();
		removeDialog->Destroy();
		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you want to delete the selected order?"),
		_("Remove order"),
		_("Delete"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		OrderEntity orderEntity(rit);
		orderEntity.SetValue(_N("orderId"), *orderId);
		if(orderController.Delete(&orderEntity)) {
			ordersListCtrl->DeleteItem(ordersListCtrl->GetPosition(orderId));
			rit->Log(false, _N("OrderModulePanel"), _("Order is removed."), _("Order id: ") + DSCasts::ToString(*orderId));
			delete orderId;
			orderId = NULL;
		} else {
			rit->Log(true, _N("OrderModulePanel"), _("Could not remove order."), _("Order id: ") + DSCasts::ToString(*orderId));

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not remove order."),
				_("Remove order")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	yesNoDialog->Destroy();
}

void OrdersModulePanel::Find() {
	ordersListCtrl->SetFocus();

	if(!ordersFindDialog) {
		ordersFindDialog = new OrdersFindDialog(this, -1, rit);
		ordersFindDialog->CenterOnScreen();
	}

	if(ordersFindDialog->ShowModal() == wxID_OK) {
		ClearOrdersListCtrl();

		OrderEntity *orderEntity = ordersFindDialog->GetOrderEntity();

		OrderController orderController(rit);
		orderController.Find(orderEntity);

		ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, orderController.GetCount());
		progressDialog->Center();
		progressDialog->Start();
		unsigned int i = 0;
		while(orderController.Run()) {
			orderController.Get(orderEntity);
			AddOrChangeToOrdersListCtrl(orderEntity, true);
			progressDialog->SetValue(++i);
			if(progressDialog->IsCanceled()) {
				orderController.Reset();
				break;
			}
		}
		progressDialog->Stop();

		delete orderEntity;
		orderEntity = NULL;
	}

	ordersListCtrl->SetFocus();
}

void OrdersModulePanel::Callback(void *data) {
	OrderEntity *orderEntity = static_cast<OrderEntity *>(data);
	if(orderEntity)
		AddOrChangeToOrdersListCtrl(orderEntity, false);
}

void OrdersModulePanel::AddOrChangeToOrdersListCtrl(OrderEntity *orderEntity, const bool justAppend) {
	unsigned int *foundOrderId = NULL;
	const unsigned int orderId = orderEntity->Get(_N("orderId"))->ToUnsignedInt();

	if(!justAppend) {
		const unsigned int count = ordersListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			foundOrderId = static_cast<unsigned int *>(ordersListCtrl->GetPointer(i, 0));
			if(foundOrderId && *foundOrderId == orderId)
				break;
			foundOrderId = NULL;
		}
	}
	if(!foundOrderId && !justAppend)
		return;

	if(!foundOrderId)
		foundOrderId = new unsigned int(orderId);

	const wxString billName(NameType(
		orderEntity->Get(_N("billAddressSalutation"))->ToString(),
		orderEntity->Get(_N("billAddressTitle"))->ToString(),
		orderEntity->Get(_N("billAddressFirstname"))->ToString(),
		orderEntity->Get(_N("billAddressName1"))->ToString(),
		orderEntity->Get(_N("billAddressName2"))->ToString(),
		orderEntity->Get(_N("billAddressName3"))->ToString(),
		orderEntity->Get(_N("billAddressName4"))->ToString()
	).GetConcatedFullName(_N(", ")));
	const wxString billAddress(AddressType(
		orderEntity->Get(_N("billAddressAddress"))->ToString(),
		orderEntity->Get(_N("billAddressNumber"))->ToString(),
		orderEntity->Get(_N("billAddressZipcode"))->ToString(),
		orderEntity->Get(_N("billAddressCity"))->ToString(),
		orderEntity->Get(_N("billAddressFederalState"))->ToString(),
		orderEntity->Get(_N("billAddressLand"))->ToString()
	).GetConcatedAddress(_N(", ")));
	const wxString billNameAddress(
		billName +
		(billName.IsEmpty() || billAddress.IsEmpty() ? _N("") : _N(": ")) +
		billAddress
	);
 
	const wxString shippingName(NameType(
		orderEntity->Get(_N("shippingAddressSalutation"))->ToString(),
		orderEntity->Get(_N("shippingAddressTitle"))->ToString(),
		orderEntity->Get(_N("shippingAddressFirstname"))->ToString(),
		orderEntity->Get(_N("shippingAddressName1"))->ToString(),
		orderEntity->Get(_N("shippingAddressName2"))->ToString(),
		orderEntity->Get(_N("shippingAddressName3"))->ToString(),
		orderEntity->Get(_N("shippingAddressName4"))->ToString()
	).GetConcatedFullName(_(", ")));
	const wxString shippingAddress(AddressType(
		orderEntity->Get(_N("shippingAddressAddress"))->ToString(),
		orderEntity->Get(_N("shippingAddressNumber"))->ToString(),
		orderEntity->Get(_N("shippingAddressZipcode"))->ToString(),
		orderEntity->Get(_N("shippingAddressCity"))->ToString(),
		orderEntity->Get(_N("shippingAddressFederalState"))->ToString(),
		orderEntity->Get(_N("shippingAddressLand"))->ToString()
	).GetConcatedAddress(_N(", ")));
	const wxString shippingNameAddress(
		shippingName +
		(shippingName.IsEmpty() || shippingAddress.IsEmpty() ? _N("") : _N(": ")) +
		shippingAddress
	);

	wxColour backgroundColour;
	switch(orderEntity->Get(_N("kind"))->ToUnsignedInt()) {
		case 2:
			backgroundColour.Set(165, 255, 150);
			break;
		case 1:
			backgroundColour.Set(255, 177, 150);
			break;
		case 0:
		default:
			backgroundColour.Set(150, 190, 255);
			break;
	}

	DSListCtrl::DSListCtrlRow *ordersListCtrlRow = new DSListCtrl::DSListCtrlRow(foundOrderId);
	ordersListCtrlRow->SetBackgroundColour(backgroundColour);

	wxString invoiceId(orderEntity->Get(_N("invoiceId"))->ToString());
	if(invoiceId.IsEmpty() || invoiceId == _N("0"))
		invoiceId = _N("-");

	wxString when(orderEntity->Get(_N("when"))->ToString());
	when = when.Mid(0, when.Find(_N(" ")));

	DSDateTime payed(DSQuery::GetDate(orderEntity->Get(_N("payed"))->ToString()));
	
	ordersListCtrlRow->Add(orderEntity->Get(_N("payed"))->ToString().IsEmpty() ? wxString(_("Not payed")) : payed.ToDatabaseDate());
	ordersListCtrlRow->Add(ConstantsLibrary::OrderKindToString(orderEntity->Get(_N("kind"))->ToUnsignedInt()));
	ordersListCtrlRow->Add(when);
	ordersListCtrlRow->Add(orderEntity->Get(_N("orderId"))->ToString());
	ordersListCtrlRow->Add(invoiceId);
	ordersListCtrlRow->Add(orderEntity->Get(_N("clientId"))->ToString());
	ordersListCtrlRow->Add(orderEntity->Get(_N("ownClientId"))->ToString());
	ordersListCtrlRow->Add(billNameAddress);
	ordersListCtrlRow->Add(shippingNameAddress);
	ordersListCtrlRow->Add(orderEntity->Get(_N("beforeTax"))->ToCurrency().ToString());
	ordersListCtrlRow->Add(orderEntity->Get(_N("fullTax"))->ToCurrency().ToString());
	ordersListCtrlRow->Add(orderEntity->Get(_N("reducedTax"))->ToCurrency().ToString());
	ordersListCtrlRow->Add(orderEntity->Get(_N("afterTax"))->ToCurrency().ToString());

	ordersListCtrl->Set(ordersListCtrlRow, true);
}

void OrdersModulePanel::ClearOrdersListCtrl() {
	ordersListCtrl->CleanUp();
}

void OrdersModulePanel::Export() {
	ordersListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to export."), this))
		return;

	ExportType exportType;
	ExportDialog *exportDialog = new ExportDialog(this, -1, rit, &exportType);
	exportDialog->Center();

	DSFile file;
	while(exportDialog->ShowModal() == wxID_OK) {
		exportDialog->TransferToExportType();

		if(file.Open(exportType.GetFilename(), _N("w"))) {
			file.Close();

			const unsigned int count = ordersListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, true);

			for(unsigned int i = 0; i < count; i++) {
				exportLibrary.ExportAdd(OrderController::GetDirect(
					rit,
					*static_cast<unsigned int *>(ordersListCtrl->GetPointer(i, 0))
				));
			}

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Error writing file. Aborting."),
				_("Export")
			);
			errorMessageDialog->Center();

			if(exportLibrary.ExportStart()) {
				if(exportLibrary.ExportRun()) {
					if(!exportLibrary.ExportStop())
						errorMessageDialog->ShowModal();
				} else {
					errorMessageDialog->ShowModal();
				}
			} else {
				errorMessageDialog->ShowModal();
			}

			errorMessageDialog->Destroy();

			break;
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("File is not writeable. Please check file permissions."),
				_("Export")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	exportDialog->Destroy();
}

bool OrdersModulePanel::CheckProfile() {
	bool ok = true;
	if(!EnterprisePreferencesController::CheckEnterpriseProfile(rit)) {
		wxMessageDialog *enterpriseMessageDialog = new wxMessageDialog(
			this,
			_("Your enterprise is not fully completed. Please check the preferences. The printing of bills will be uncompleted."),
			_("Error")
		);
		enterpriseMessageDialog->Center();
		enterpriseMessageDialog->ShowModal();
		enterpriseMessageDialog->Destroy();
		ok = false;
	}

	if(!CommonPreferencesController::CheckTaxProfile(rit)) {
		wxMessageDialog *taxMessageDialog = new wxMessageDialog(
			this,
			_("You have not entered valid tax informations in the common preferences. There will be calculation errors and the printing of bills will be uncompleted or incorrect."),
			_("Error")
		);
		taxMessageDialog->Center();
		taxMessageDialog->ShowModal();
		taxMessageDialog->Destroy();
		ok = false;
	}

	return ok;
}

void OrdersModulePanel::Monitions() {
	if(!rit->Access(A_READ, _("You have no permission to view monitions."), this))
		return;

	monitionsFrame->Show(!monitionsFrame->IsShown());
}

void OrdersModulePanel::Copy(const unsigned int *orderId) {
	ordersListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to copy an order."), this))
		return;

	if(!orderId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the order you want to copy."),
			_("Copy order")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	OrderEntity *orderEntity = new OrderEntity(rit);
	OrderController orderController(rit);
	orderController.Get(DSCasts::ToString(*orderId));
	if(orderController.Run()) {
		orderController.Get(orderEntity);
		orderController.Reset();
		orderController.Change(orderEntity, true);
	} else {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Could not copy the order. Maybe the selected order is not in database anymore."),
			_("New order"),
			_("Create"),
			_("Cancel")
		);
		yesNoDialog->Center();
		yesNoDialog->ShowModal();
		yesNoDialog->Destroy();

		delete orderEntity;
		return;
	}

	OrderEditFrame *orderEditFrame = new OrderEditFrame(this, -1, rit, orderEntity, this);
	orderEditFrame->Center();
	orderEditFrame->Show();
}

void OrdersModulePanel::Payed(const unsigned int *orderId) {
	if(!orderId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the order you want to edit."),
			_("Edit order")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("orders") , *orderId);
	if(blockType) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The order you want to set payed is open. Please use the edit dialog."),
			_("Set payed")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		OrderEditFrame *orderEditFrame = static_cast<OrderEditFrame *>(blockType->GetData());
		if(orderEditFrame)
			orderEditFrame->Raise();

		return;
	}

	if(!IsInvoiced(*orderId)) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The order you want to set payed is not invoiced. You can only set invoiced orders payed."),
			_("Set payed")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	if(IfLinkedShowThis(*orderId))
		return;

	if(!rit->Access(A_WRITE, _("You have no permission to set this order as payed."), this))
		return;
	
	OrderController orderController(rit);
	wxString payed(orderController.GetPayed(*orderId));

	bool ok = false;
	DSDateTime payedTime;

	if(payed.IsEmpty()) {
		PayedDialog *payedDialog = new PayedDialog(
			this,
			-1,
			rit,
			_("Set payed")
		);
		payedDialog->Center();
		ok = payedDialog->ShowModal() == wxID_OK;
		payedTime = payedDialog->GetValue();
		payedDialog->Destroy();
	} else {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you want to set this order unpayed?"),
			_("Order"),
			_("unpayed"),
			_("Cancel")
		);
		yesNoDialog->Center();
		ok = yesNoDialog->ShowModal() == wxID_YES;
		yesNoDialog->Destroy();
	}
	
	if(ok) {
		if(orderController.SetPayed(*orderId, payed.IsEmpty(), payedTime)) {
			rit->Log(false, _N("OrderEditFrame"), _("The order is set as payed."), _("The order \"") + DSCasts::ToString(orderId) + _("\" is set as payed."));

			orderController.Get(DSCasts::ToString(*orderId));
			if(orderController.Run()) {
				OrderEntity orderEntity(rit);
				orderController.Get(&orderEntity);
				orderController.Reset();

				AddOrChangeToOrdersListCtrl(&orderEntity, false);
			}
		} else {
			rit->Log(true, _N("OrderEditFrame"), _("The order is not set as payed."), _("The order \"") + DSCasts::ToString(orderId) + _("\" is not set as payed."));
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("The order could not be set as payed. Database error."),
				_("Error")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		}
	}
}

bool OrdersModulePanel::IsInvoiced(const unsigned int orderId) {
	OrderController orderController(rit);
	return orderController.IsInvoiced(orderId);
}

bool OrdersModulePanel::IfLinkedShowThis(const unsigned int orderId) {
	OrderEntity orderEntity(rit);
	OrderController orderController(rit);
	orderController.Get(DSCasts::ToString(orderId));
	if(!orderController.Run())
		return false;
	orderController.Get(&orderEntity);
	orderController.Reset();

	if(orderController.IsLinked(orderId, orderEntity.GetLinkedOrders())) {
		wxMessageDialog *invoicedMessageDialog = new wxMessageDialog(
			this,
			_("This order is linked (") + orderEntity.Get(_N("linkedOrderId"))->ToString() + _N("). You are not able to change any fields."),
			_("Linked")
		);
		invoicedMessageDialog->Center();
		invoicedMessageDialog->ShowModal();
		invoicedMessageDialog->Destroy();

		return true;
	}

	return false;
}
