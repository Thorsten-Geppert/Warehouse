#include "ClientsModulePanel.h"

#include <wx/xrc/xmlres.h>

#include "../lib/PermissionLibrary.h"
#include "../lib/ClientController.h"
#include "../lib/PhoneType.h"
#include "../lib/AddressType.h"
#include "../lib/ReferencesType.h"
#include "../lib/OrderController.h"
#include "../lib/ConstantsLibrary.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"
#include "../lib/ProductController.h"

#include "ClientEditFrame.h"
#include "ProgressDialog.h"
#include "RemoveDialog.h"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"
#include "YesNoDialog.h"

BEGIN_EVENT_TABLE(ClientsModulePanel, ModulePanel)
	EVT_LIST_ITEM_ACTIVATED(XRCID("clientsListCtrl"), ClientsModulePanel::DirectEditEvent)
	EVT_MENU(NEW_EVENT, ClientsModulePanel::NewKeyEvent)
	EVT_MENU(EDIT_EVENT, ClientsModulePanel::EditKeyEvent)
	EVT_MENU(REMOVE_EVENT, ClientsModulePanel::RemoveKeyEvent)
	EVT_MENU(FIND_EVENT, ClientsModulePanel::FindKeyEvent)
	EVT_MENU(EXPORT_EVENT, ClientsModulePanel::ExportKeyEvent)
END_EVENT_TABLE()

ClientsModulePanel::ClientsModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("ClientsModulePanel")
) {
	// Toolbar
	newToolbarButton         = XRCCTRL(*this, "newToolbarButton", ToolbarButton);
	editToolbarButton        = XRCCTRL(*this, "editToolbarButton", ToolbarButton);
	removeToolbarButton      = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	findToolbarButton        = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	exportToolbarButton      = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);

	// Clients
	clientsListCtrl = XRCCTRL(*this, "clientsListCtrl", DynSoft::DSListCtrl);

	clientsListCtrl->InsertColumn(0, _("Customer id"), wxLIST_FORMAT_RIGHT, 90);
	clientsListCtrl->InsertColumn(1, _("Own customer id"), wxLIST_FORMAT_RIGHT, 160);
	clientsListCtrl->InsertColumn(2, _("Name"), wxLIST_FORMAT_LEFT, 200);
	clientsListCtrl->InsertColumn(3, _("Firstname"), wxLIST_FORMAT_LEFT, 200);
	clientsListCtrl->InsertColumn(4, _("Address"), wxLIST_FORMAT_LEFT, 200);
	clientsListCtrl->InsertColumn(5, _("Zipcode"), wxLIST_FORMAT_LEFT, 70);
	clientsListCtrl->InsertColumn(6, _("City"), wxLIST_FORMAT_LEFT, 150);
	clientsListCtrl->InsertColumn(7, _("Land"), wxLIST_FORMAT_LEFT, 150);
	clientsListCtrl->InsertColumn(8, _("Telephone"), wxLIST_FORMAT_LEFT, 150);
	clientsListCtrl->InsertColumn(9, _("Email"), wxLIST_FORMAT_LEFT, 160);
	clientsListCtrl->InsertColumn(10, _("Active"), wxLIST_FORMAT_LEFT, 100);

	// Events
	CONNECT_TOOLBARBUTTON(newToolbarButton, ClientsModulePanel::NewEvent);
	CONNECT_TOOLBARBUTTON(editToolbarButton, ClientsModulePanel::EditEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, ClientsModulePanel::RemoveEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, ClientsModulePanel::FindEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, ClientsModulePanel::ExportEvent);

	wxAcceleratorEntry acceleratorEntries[5];
	acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 'n', NEW_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, (int) 'e', EDIT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL, (int) 'd', REMOVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL, (int) 'f', FIND_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL, (int) 'k', EXPORT_EVENT);
	wxAcceleratorTable acceleratorTable(5, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Stuff
	clientsFindDialog = NULL;
}

ClientsModulePanel::~ClientsModulePanel() {
	ClearClientsListCtrl();
}

void ClientsModulePanel::Load() {
	clientsListCtrl->SetFocus();
}

void ClientsModulePanel::Unload() {
}

void ClientsModulePanel::NewEvent(wxMouseEvent &event) {
	New();
	SKIP();
}

void ClientsModulePanel::EditEvent(wxMouseEvent &event) {
	unsigned int *clientId = static_cast<unsigned int *>(clientsListCtrl->GetSingleSelectionPointer());
	Edit(clientId);
	SKIP();
}

void ClientsModulePanel::DirectEditEvent(wxListEvent &event) {
	unsigned int *clientId = static_cast<unsigned int *>(clientsListCtrl->GetSingleSelectionPointer());
	Edit(clientId);
}

void ClientsModulePanel::RemoveEvent(wxMouseEvent &event) {
	Remove();
	SKIP();
}

void ClientsModulePanel::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void ClientsModulePanel::NewKeyEvent(wxCommandEvent &event) {
	New();
}

void ClientsModulePanel::EditKeyEvent(wxCommandEvent &event) {
	unsigned int *clientId = static_cast<unsigned int *>(clientsListCtrl->GetSingleSelectionPointer());
	Edit(clientId);
}

void ClientsModulePanel::RemoveKeyEvent(wxCommandEvent &event) {
	Remove();
}

void ClientsModulePanel::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void ClientsModulePanel::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void ClientsModulePanel::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void ClientsModulePanel::New() {
	clientsListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to create a new client."), this))
		return;

	ClientEditFrame *clientEditFrame = new ClientEditFrame(this, -1, rit, new ClientEntity(rit), this);
	clientEditFrame->Center();
	clientEditFrame->Show();
}

void ClientsModulePanel::Edit(unsigned int *clientId) {
	clientsListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to open a client."), this))
		return;

	if(!clientId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the client you want to edit."),
			_("Edit client")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("clients"), *clientId);
	if(blockType) {
		ClientEditFrame *clientEditFrame = static_cast<ClientEditFrame *>(blockType->GetData());
		if(clientEditFrame) {
			clientEditFrame->Raise();
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The client you want to edit is allready opened."),
				_("Edit client")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
		return;
	}

	ClientController clientController(rit);
	clientController.Get(DSCasts::ToString(*clientId));
	if(clientController.Run()) {
		ClientEntity *clientEntity = new ClientEntity(rit);
		clientController.Get(clientEntity);
		clientController.Reset();

		ClientEditFrame *clientEditFrame = new ClientEditFrame(this, -1, rit, clientEntity, this);
		clientEditFrame->Center();
		clientEditFrame->Show();

		rit->Block(_N("clients"), clientEntity->Get(_N("clientId"))->ToUnsignedInt(), clientEditFrame);
	} else {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not find the client in the database."),
			_("Edit client")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}
}

void ClientsModulePanel::Remove() {
	clientsListCtrl->SetFocus();

	if(!rit->Access(A_DELETE, _("You have no permission to remove a client."), this))
		return;

	unsigned int *clientId = static_cast<unsigned int *>(clientsListCtrl->GetSingleSelectionPointer());

	if(!clientId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the client you want to remove."),
			_("Remove client")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("clients") , *clientId);
	if(blockType) {
		rit->Log(true, _N("ClientsModulePanel"), _("Could not remove client."), _("The client is open."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The client you want to remove is open."),
			_("Remove client")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		ClientEditFrame *clientEditFrame = static_cast<ClientEditFrame *>(blockType->GetData());
		if(clientEditFrame)
			clientEditFrame->Raise();

		return;
	}

	ReferencesType referencesType;
	OrderController orderController(rit);
	orderController.GetReferences(ConstantsLibrary::CLIENT_KIND, *clientId, &referencesType);
	ProductController productController(rit);
	productController.GetClientReferences(*clientId, &referencesType);
	if(referencesType.GetCount() != 0) {
		rit->Log(true, _N("ClientsModulePanel"), _("Could not remove client."), _("The client is referenced."));

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
		_("Do you want to delete the selected client?"),
		_("Remove client"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		ClientEntity clientEntity(rit);
		clientEntity.SetValue(_N("clientId"), *clientId);
		ClientController clientController(rit);
		if(clientController.Delete(&clientEntity)) {
			clientsListCtrl->DeleteItem(clientsListCtrl->GetPosition(clientId));
			rit->Log(false, _N("ClientsModulePanel"), _("The client is removed."), _("Client id: ") + DSCasts::ToString(*clientId));
			delete clientId;
			clientId = NULL;
		} else {
			rit->Log(true, _N("ClientsModulePanel"), _("The client could not been removed."), _("Client id: ") + DSCasts::ToString(*clientId));

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not remove client."),
				_("Remove client")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	yesNoDialog->Destroy();
}

void ClientsModulePanel::Find() {
	clientsListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to find clients."), this))
		return;
	
	if(!clientsFindDialog) {
		clientsFindDialog = new ClientsFindDialog(this, -1, rit);
		clientsFindDialog->CenterOnScreen();
	}
	
	if(clientsFindDialog->ShowModal() == wxID_OK) {
		ClearClientsListCtrl();

		ClientEntity *clientEntity = clientsFindDialog->GetClientEntity();

		ClientController clientController(rit);
		clientController.Find(clientEntity);

		ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, clientController.GetCount());
		progressDialog->Center();
		progressDialog->Start();
		unsigned int i = 0;
		while(clientController.Run()) {
			clientController.Get(clientEntity);
			AddOrChangeToClientsListCtrl(clientEntity, true);
			progressDialog->SetValue(++i);
			if(progressDialog->IsCanceled()) {
				clientController.Reset();
				break;
			}
		}
		progressDialog->Stop();

		delete clientEntity;
		clientEntity = NULL;
	}

	clientsListCtrl->SetFocus();
}

void ClientsModulePanel::Callback(void *data) {
	ClientEntity *clientEntity = static_cast<ClientEntity *>(data);
	if(clientEntity) {
		AddOrChangeToClientsListCtrl(clientEntity, false);
	}
}

void ClientsModulePanel::AddOrChangeToClientsListCtrl(ClientEntity *clientEntity, const bool justAppend) {
	unsigned int *foundCustomerId = NULL;
	const unsigned int clientId = clientEntity->Get(_N("clientId"))->ToUnsignedInt();

	if(!justAppend) {
		const unsigned int count = clientsListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			foundCustomerId = static_cast<unsigned int *>(clientsListCtrl->GetPointer(i, 0));
			if(foundCustomerId && *foundCustomerId == clientId)
				break;
			foundCustomerId = NULL;
		}
	}

	if(!foundCustomerId && !justAppend)
		return;

	if(!foundCustomerId)
		foundCustomerId = new unsigned int(clientId);

	DSListCtrl::DSListCtrlRow *clientsListCtrlRow = new DSListCtrl::DSListCtrlRow(foundCustomerId);
	clientsListCtrlRow->SetBackgroundColour(wxColour(clientEntity->Get(_N("formOfOrganization"))->ToUnsignedInt() == 0 ? wxColour(165, 255, 150) : wxColour(255, 177, 150)));

	clientsListCtrlRow->Add(clientEntity->Get(_N("clientId"))->ToString());
	clientsListCtrlRow->Add(clientEntity->Get(_N("ownClientId"))->ToString());
	clientsListCtrlRow->Add(clientEntity->Get(_N("name1"))->ToString());
	clientsListCtrlRow->Add(clientEntity->Get(_N("firstname"))->ToString());
	clientsListCtrlRow->Add(AddressType::GetAddressAndNumber(
		clientEntity->Get(_N("address"))->ToString(),
		clientEntity->Get(_N("number"))->ToString()
	));
	clientsListCtrlRow->Add(clientEntity->Get(_N("zipcode"))->ToString());
	clientsListCtrlRow->Add(clientEntity->Get(_N("city"))->ToString());
	clientsListCtrlRow->Add(clientEntity->Get(_N("land"))->ToString());
	clientsListCtrlRow->Add(PhoneType(
		clientEntity->Get(_N("telephone1Country"))->ToString(),
		clientEntity->Get(_N("telephone1Prefix"))->ToString(),
		clientEntity->Get(_N("telephone1Number"))->ToString()
	).ToString());
	clientsListCtrlRow->Add(clientEntity->Get(_N("email1"))->ToString());
	clientsListCtrlRow->Add(clientEntity->Get(_N("active"))->ToBool() ? _("yes") : _("no"));

	clientsListCtrl->Set(clientsListCtrlRow, true);
}

void ClientsModulePanel::ClearClientsListCtrl() {
	clientsListCtrl->CleanUp();
}

void ClientsModulePanel::Export() {
	clientsListCtrl->SetFocus();

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

			const unsigned int count = clientsListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, true);

			for(unsigned int i = 0; i < count; i++) {
				exportLibrary.ExportAdd(ClientController::GetDirect(
					rit,
					*static_cast<unsigned int *>(clientsListCtrl->GetPointer(i, 0))
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
