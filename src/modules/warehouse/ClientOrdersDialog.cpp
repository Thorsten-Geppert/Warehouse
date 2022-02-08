#include "ClientOrdersDialog.h"
#include <DSFile.h>
#include "../lib/OrderController.h"
#include "../lib/NameType.h"
#include "../lib/AddressType.h"
#include "../lib/ConstantsLibrary.h"
#include "../lib/EnterprisePreferencesController.h"
#include "../lib/CommonPreferencesController.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"
#include "OrderEditFrame.h"
#include "ProgressDialog.h"
#include "RemoveDialog.h"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"
#include "YesNoDialog.h"
#include "Configuration.h"
#include "OrdersFindDialog.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ClientOrdersDialog, wxDialog)
	EVT_LIST_ITEM_ACTIVATED(XRCID("ordersListCtrl"), ClientOrdersDialog::DirectEditEvent)
	EVT_MENU(CLOSE_EVENT, ClientOrdersDialog::CancelKeyEvent)
	EVT_MENU(UPDATE_EVENT, ClientOrdersDialog::UpdateKeyEvent)
	EVT_MENU(USE_EVENT, ClientOrdersDialog::UseKeyEvent)
	EVT_MENU(NEW_EVENT, ClientOrdersDialog::NewKeyEvent)
	EVT_MENU(EDIT_EVENT, ClientOrdersDialog::EditKeyEvent)
	EVT_MENU(REMOVE_EVENT, ClientOrdersDialog::RemoveKeyEvent)
	EVT_MENU(FIND_EVENT, ClientOrdersDialog::FindKeyEvent)
	EVT_MENU(EXPORT_EVENT, ClientOrdersDialog::ExportKeyEvent)
END_EVENT_TABLE()

ClientOrdersDialog::ClientOrdersDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const unsigned int clientId,
	const bool showUse,
	GuiCallbackInterface *callbackInterface,
	const unsigned int exclude,
	OrderEntities *excludes,
	OrderIdArray *includes,
	const bool showLinkedOrders,
	const bool linkMode
) {
	SetId(id);
	this->rit               = rit;
	this->clientId          = clientId;
	this->callbackInterface = callbackInterface;
	this->excludes          = excludes;
	this->includes          = includes;
	this->showLinkedOrders  = showLinkedOrders;
	this->linkMode          = linkMode;

	SetExclude(exclude);

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ClientOrdersDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ClientOrdersDialog"));

	updateToolbarButton = XRCCTRL(*this, "updateToolbarButton", ToolbarButton);
	newToolbarButton     = XRCCTRL(*this, "newToolbarButton", ToolbarButton);
	editToolbarButton    = XRCCTRL(*this, "editToolbarButton", ToolbarButton);
	removeToolbarButton  = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	findToolbarButton    = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	exportToolbarButton  = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);
	useToolbarButton     = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	cancelToolbarButton  = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	ordersListCtrl       = XRCCTRL(*this, "ordersListCtrl", DynSoft::DSListCtrl);

	ordersListCtrl->InsertColumn(0, _("Kind"), wxLIST_FORMAT_LEFT, 80);
	ordersListCtrl->InsertColumn(1, _("Date"), wxLIST_FORMAT_LEFT, 120);
	ordersListCtrl->InsertColumn(2, _("Order id"), wxLIST_FORMAT_RIGHT, 90);
	ordersListCtrl->InsertColumn(3, _("Invoice id"), wxLIST_FORMAT_RIGHT, 90);
	ordersListCtrl->InsertColumn(4, _("Client id"), wxLIST_FORMAT_RIGHT, 90);
	ordersListCtrl->InsertColumn(5, _("Billing address"), wxLIST_FORMAT_LEFT, 300);
	ordersListCtrl->InsertColumn(6, _("Shipping address"), wxLIST_FORMAT_LEFT, 300);
	ordersListCtrl->InsertColumn(7, _("Price"), wxLIST_FORMAT_RIGHT, 90);
	ordersListCtrl->InsertColumn(8, _("Full tax"), wxLIST_FORMAT_RIGHT, 90);
	ordersListCtrl->InsertColumn(9, _("Reduced tax"), wxLIST_FORMAT_RIGHT, 90);
	ordersListCtrl->InsertColumn(10, _("Sum"), wxLIST_FORMAT_RIGHT, 90);

	if(showUse)
		CONNECT_TOOLBARBUTTON(useToolbarButton, ClientOrdersDialog::UseEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ClientOrdersDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(updateToolbarButton, ClientOrdersDialog::UpdateEvent);
	CONNECT_TOOLBARBUTTON(newToolbarButton, ClientOrdersDialog::NewEvent);
	CONNECT_TOOLBARBUTTON(editToolbarButton, ClientOrdersDialog::EditEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, ClientOrdersDialog::RemoveEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, ClientOrdersDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, ClientOrdersDialog::ExportEvent);

	wxAcceleratorEntry acceleratorEntries[9];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, (int) 'w',    CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL, (int) 'r',    UPDATE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL, (int) 'n',    NEW_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL, (int) 'e',    EDIT_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL, (int) 'd',    REMOVE_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL, (int) 'f',    FIND_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL, (int) 'k',    EXPORT_EVENT);
	if(showUse)
		acceleratorEntries[8].Set(wxACCEL_CTRL, (int) 'u',  USE_EVENT);
	else
		acceleratorEntries[8].Set(wxACCEL_CTRL, (int) 'w',  CLOSE_EVENT); // FIXME: Empty Event
	wxAcceleratorTable acceleratorTable(9, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	if(!showUse)
		useToolbarButton->SetEnabled(false);

	SetSize(wxSize(800, 600));

	Update();

	ICON();
}

ClientOrdersDialog::~ClientOrdersDialog() {
	ClearOrdersListCtrl();
}

void ClientOrdersDialog::UpdateEvent(wxMouseEvent &event) {
	Update();
	SKIP();
}

void ClientOrdersDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void ClientOrdersDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ClientOrdersDialog::UpdateKeyEvent(wxCommandEvent &event) {
	Update();
}

void ClientOrdersDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void ClientOrdersDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ClientOrdersDialog::NewEvent(wxMouseEvent &event) {
	New(static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer()));
	SKIP();
}

void ClientOrdersDialog::EditEvent(wxMouseEvent &event) {
	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());
	Edit(orderId);
	SKIP();
}

void ClientOrdersDialog::DirectEditEvent(wxListEvent &event) {
	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());
	Edit(orderId);
}

void ClientOrdersDialog::RemoveEvent(wxMouseEvent &event) {
	Remove();
	SKIP();
}

void ClientOrdersDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void ClientOrdersDialog::NewKeyEvent(wxCommandEvent &event) {
	New(static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer()));
}

void ClientOrdersDialog::EditKeyEvent(wxCommandEvent &event) {
	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());
	Edit(orderId);
}

void ClientOrdersDialog::RemoveKeyEvent(wxCommandEvent &event) {
	Remove();
}

void ClientOrdersDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void ClientOrdersDialog::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void ClientOrdersDialog::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void ClientOrdersDialog::Export() {
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

void ClientOrdersDialog::New(const unsigned int *orderId) {
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
			_("Create"),
			_("Cancel")
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

void ClientOrdersDialog::Edit(const unsigned int *orderId) {
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

	BlockType *blockType = rit->IsBlocked(_N("orders") , *orderId);
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

void ClientOrdersDialog::Remove() {
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
		_("Remove"),
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

void ClientOrdersDialog::Find() {
	ordersListCtrl->SetFocus();

	OrdersFindDialog *ordersFindDialog = new OrdersFindDialog(this, -1, rit, clientId);
	ordersFindDialog->CenterOnScreen();
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
	ordersFindDialog->Destroy();

	ordersListCtrl->SetFocus();
}

void ClientOrdersDialog::Callback(void *data) {
	OrderEntity *orderEntity = static_cast<OrderEntity *>(data);
	if(orderEntity)
		AddOrChangeToOrdersListCtrl(orderEntity, false);
}

void ClientOrdersDialog::Use() {
	if(!callbackInterface)
		return;

	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());
	if(!orderId) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a order."),
			_("Use")
		);
		errorMessage->ShowModal();
	} else {
		callbackInterface->Callback(static_cast<void *>(orderId));
	}
}

void ClientOrdersDialog::Cancel() {
	Show(false);
}

void ClientOrdersDialog::ClearOrdersListCtrl() {
	ordersListCtrl->CleanUp();
}

void ClientOrdersDialog::AddOrChangeToOrdersListCtrl(OrderEntity *orderEntity, const bool justAppend) {
	if(!orderEntity)
		return;

	unsigned int *foundOrderId = NULL;
	const unsigned int orderId = orderEntity->Get(_N("orderId"))->ToUnsignedInt();

	bool found = false;
	if(includes) {
		const unsigned int count = includes->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			if(orderId == includes->Item(i)) {
				found = true;
				break;
			}
		}
	}

	if(!found && exclude > 0) {
		if(exclude == orderId)
			return;

		if(orderEntity->Get(_N("linkedOrderId"))->ToUnsignedInt() > 0)
			return;

		if(excludes) {
			OrderEntity *tmpOrderId = NULL;
			const unsigned int count = excludes->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				tmpOrderId = excludes->Get(i);
				if(tmpOrderId && tmpOrderId->Get(_N("orderId"))->ToUnsignedInt() == orderId)
					return;
			}
		}
	}

	if(linkMode) {
		if(orderEntity->Get(_N("invoiceId"))->ToUnsignedInt() != 0)
			return;

		if(orderEntity->Get(_N("kind"))->ToUnsignedInt() != ConstantsLibrary::ORDER_KIND_ORDER)
			return;
	}
	
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

	ordersListCtrlRow->Add(ConstantsLibrary::OrderKindToString(orderEntity->Get(_N("kind"))->ToUnsignedInt()));
	ordersListCtrlRow->Add(orderEntity->Get(_N("when"))->ToString());
	ordersListCtrlRow->Add(orderEntity->Get(_N("orderId"))->ToString());
	ordersListCtrlRow->Add(invoiceId);
	ordersListCtrlRow->Add(orderEntity->Get(_N("clientId"))->ToString());
	ordersListCtrlRow->Add(billNameAddress);
	ordersListCtrlRow->Add(shippingNameAddress);
	ordersListCtrlRow->Add(orderEntity->Get(_N("beforeTax"))->ToCurrency().ToString());
	ordersListCtrlRow->Add(orderEntity->Get(_N("fullTax"))->ToCurrency().ToString());
	ordersListCtrlRow->Add(orderEntity->Get(_N("reducedTax"))->ToCurrency().ToString());
	ordersListCtrlRow->Add(orderEntity->Get(_N("afterTax"))->ToCurrency().ToString());

	ordersListCtrl->Set(ordersListCtrlRow, true);
}

void ClientOrdersDialog::Update() {
	ClearOrdersListCtrl();

	OrderEntity orderEntity(rit);
	OrderController orderController(rit);
	orderController.Get(_N("clientId"), DSCasts::ToString(clientId));
	while(orderController.Run()) {
		orderController.Get(&orderEntity);
		AddOrChangeToOrdersListCtrl(&orderEntity, true);
	}
}

// TODO Same as in OrdersModulePanel
bool ClientOrdersDialog::CheckProfile() {
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

bool ClientOrdersDialog::RemoveSelected() {
	const long position = ordersListCtrl->GetSingleSelectionPosition();
	if(position == wxNOT_FOUND)
		return false;
	
	unsigned int *orderId = static_cast<unsigned int *>(ordersListCtrl->GetSingleSelectionPointer());
	if(orderId)
		delete orderId;
	
	ordersListCtrl->DeleteItem(position);

	return true;
}

void ClientOrdersDialog::SetExclude(const unsigned int exclude) {
	this->exclude = exclude;
}

void ClientOrdersDialog::SetClientId(const unsigned int clientId) {
	this->clientId = clientId;
	Update();
}
