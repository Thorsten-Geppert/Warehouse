#include "PurchasesModulePanel.h"
#include <wx/xrc/xmlres.h>
#include "PurchaseEditFrame.h"
#include "ProgressDialog.h"
#include "RemoveDialog.h"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"
#include "YesNoDialog.h"

#include "../lib/PermissionLibrary.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"
#include "../lib/PurchaseController.h"
#include "../lib/PurchaseEntity.h"
#include "../lib/Tools.h"

BEGIN_EVENT_TABLE(PurchasesModulePanel, ModulePanel)
	EVT_LIST_ITEM_ACTIVATED(XRCID("purchasesListCtrl"), PurchasesModulePanel::DirectEditEvent)
	EVT_MENU(NEW_EVENT, PurchasesModulePanel::NewKeyEvent)
	EVT_MENU(COPY_EVENT, PurchasesModulePanel::CopyKeyEvent)
	EVT_MENU(EDIT_EVENT, PurchasesModulePanel::EditKeyEvent)
	EVT_MENU(REMOVE_EVENT, PurchasesModulePanel::RemoveKeyEvent)
	EVT_MENU(FIND_EVENT, PurchasesModulePanel::FindKeyEvent)
	EVT_MENU(EXPORT_EVENT, PurchasesModulePanel::ExportKeyEvent)
END_EVENT_TABLE()

PurchasesModulePanel::PurchasesModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("PurchasesModulePanel")
) {
	// Toolbar
	newToolbarButton       = XRCCTRL(*this, "newToolbarButton", ToolbarButton);
	copyToolbarButton      = XRCCTRL(*this, "copyToolbarButton", ToolbarButton);
	editToolbarButton      = XRCCTRL(*this, "editToolbarButton", ToolbarButton);
	removeToolbarButton    = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	findToolbarButton      = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	exportToolbarButton    = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);
	purchasesListCtrl      = XRCCTRL(*this, "purchasesListCtrl", DynSoft::DSListCtrl);

	purchasesListCtrl->InsertColumn(0, _("Purchase id"), wxLIST_FORMAT_RIGHT, 130);
	purchasesListCtrl->InsertColumn(1, _("Done"), wxLIST_FORMAT_LEFT, 110);
	purchasesListCtrl->InsertColumn(2, _("Order date"), wxLIST_FORMAT_LEFT, 130);
	purchasesListCtrl->InsertColumn(3, _("Order id"), wxLIST_FORMAT_RIGHT, 130);
	purchasesListCtrl->InsertColumn(4, _("Distributor id"), wxLIST_FORMAT_RIGHT, 140);
	purchasesListCtrl->InsertColumn(5, _("Client id"), wxLIST_FORMAT_LEFT, 140);
	purchasesListCtrl->InsertColumn(6, _("Distributor address"), wxLIST_FORMAT_LEFT, 300);
	purchasesListCtrl->InsertColumn(7, _("Billing address"), wxLIST_FORMAT_LEFT, 300);
	purchasesListCtrl->InsertColumn(8, _("Delivery address"), wxLIST_FORMAT_LEFT, 300);
	purchasesListCtrl->InsertColumn(9, _("Sum"), wxLIST_FORMAT_RIGHT, 90);
	purchasesListCtrl->InsertColumn(10, _("Purchaser"), wxLIST_FORMAT_LEFT, 110);

	// Events
	CONNECT_TOOLBARBUTTON(newToolbarButton, PurchasesModulePanel::NewEvent);
	CONNECT_TOOLBARBUTTON(copyToolbarButton, PurchasesModulePanel::CopyEvent);
	CONNECT_TOOLBARBUTTON(editToolbarButton, PurchasesModulePanel::EditEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, PurchasesModulePanel::RemoveEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, PurchasesModulePanel::FindEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, PurchasesModulePanel::ExportEvent);

	wxAcceleratorEntry acceleratorEntries[6];
	acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 'n', NEW_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, (int) 'e', EDIT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL, (int) 'd', REMOVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL, (int) 'f', FIND_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL, (int) 'k', EXPORT_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL, (int) 'c', COPY_EVENT);
	wxAcceleratorTable acceleratorTable(6, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Stuff
	purchasesFindDialog = NULL;
}

PurchasesModulePanel::~PurchasesModulePanel() {
	ClearPurchasesListCtrl();
}

void PurchasesModulePanel::Load() {
	purchasesListCtrl->SetFocus();
}

void PurchasesModulePanel::Unload() {
}

void PurchasesModulePanel::NewEvent(wxMouseEvent &event) {
	New();
	SKIP();
}

void PurchasesModulePanel::EditEvent(wxMouseEvent &event) {
	unsigned int *purchaseId = static_cast<unsigned int *>(purchasesListCtrl->GetSingleSelectionPointer());
	Edit(purchaseId);
	SKIP();
}

void PurchasesModulePanel::DirectEditEvent(wxListEvent &event) {
	unsigned int *purchaseId = static_cast<unsigned int *>(purchasesListCtrl->GetSingleSelectionPointer());
	Edit(purchaseId);
}

void PurchasesModulePanel::RemoveEvent(wxMouseEvent &event) {
	Remove();
	SKIP();
}

void PurchasesModulePanel::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void PurchasesModulePanel::NewKeyEvent(wxCommandEvent &event) {
	New();
}

void PurchasesModulePanel::EditKeyEvent(wxCommandEvent &event) {
	unsigned int *purchaseId = static_cast<unsigned int *>(purchasesListCtrl->GetSingleSelectionPointer());
	Edit(purchaseId);
}

void PurchasesModulePanel::RemoveKeyEvent(wxCommandEvent &event) {
	Remove();
}

void PurchasesModulePanel::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void PurchasesModulePanel::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void PurchasesModulePanel::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void PurchasesModulePanel::CopyEvent(wxMouseEvent &event) {
	Copy(static_cast<unsigned int *>(purchasesListCtrl->GetSingleSelectionPointer()));
	SKIP();
}

void PurchasesModulePanel::CopyKeyEvent(wxCommandEvent &event) {
	Copy(static_cast<unsigned int *>(purchasesListCtrl->GetSingleSelectionPointer()));
}

void PurchasesModulePanel::New() {
	purchasesListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to create a new purchase."), this))
		return;

	PurchaseEditFrame *purchaseEditFrame = new PurchaseEditFrame(this, -1, rit, new PurchaseEntity(rit), this);
	purchaseEditFrame->Center();
	purchaseEditFrame->Show();
}

void PurchasesModulePanel::Edit(const unsigned int *purchaseId) {
	purchasesListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to open an purchase."), this))
		return;

	if(!purchaseId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the purchase you want to edit."),
			_("Edit purchase")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("purchases"), *purchaseId);
	if(blockType) {
		PurchaseEditFrame *purchaseEditFrame = static_cast<PurchaseEditFrame *>(blockType->GetData());
		if(purchaseEditFrame) {
			purchaseEditFrame->Raise();
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The purchase you want to edit is allready opened."),
				_("Edit purchase")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
		return;
	}

	PurchaseController purchaseController(rit);
	purchaseController.Get(DSCasts::ToString(*purchaseId));
	if(purchaseController.Run()) {
		PurchaseEntity *purchaseEntity = new PurchaseEntity(rit);
		purchaseController.Get(purchaseEntity);
		purchaseController.Reset();

		PurchaseEditFrame *purchaseEditFrame = new PurchaseEditFrame(this, -1, rit, purchaseEntity, this);
		purchaseEditFrame->Center();
		purchaseEditFrame->Show();

		rit->Block(_N("purchases"), purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt(), purchaseEditFrame);
	} else {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not find the purchase in the database."),
			_("Edit purchase")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}
}

void PurchasesModulePanel::Remove() {
	purchasesListCtrl->SetFocus();

	if(!rit->Access(A_DELETE, _("You have no permission to remove an purchase."), this))
		return;

	unsigned int *purchaseId = static_cast<unsigned int *>(purchasesListCtrl->GetSingleSelectionPointer());

	if(!purchaseId) {
		rit->Log(true, _N("PurchaseModulePanel"), _("Could not remove purchase."), _("No purchase selected."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the purchase you want to remove."),
			_("Remove purchase")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	PurchaseController purchaseController(rit);
	if(!purchaseController.GetDone(*purchaseId).IsEmpty()) {
		rit->Log(true, _N("PurchaseModulePanel"), _("Could not remove purchase."), _("Purchase is done."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The purchase is done. You cannot remove done purchases."),
			_("Remove purchase")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("purchases") , *purchaseId);
	if(blockType) {
		rit->Log(true, _N("PurchaseModulePanel"), _("Could not remove purchase."), _("Purchase is open."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The purchase you want to remove is open."),
			_("Remove purchase")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		PurchaseEditFrame *purchaseEditFrame = static_cast<PurchaseEditFrame *>(blockType->GetData());
		if(purchaseEditFrame)
			purchaseEditFrame->Raise();

		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you want to delete the selected purchase?"),
		_("Remove purchase"),
		_("Delete"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		PurchaseEntity purchaseEntity(rit);
		purchaseEntity.SetValue(_N("purchaseId"), *purchaseId);
		if(purchaseController.Delete(&purchaseEntity)) {
			purchasesListCtrl->DeleteItem(purchasesListCtrl->GetPosition(purchaseId));
			rit->Log(false, _N("PurchaseModulePanel"), _("Purchase is removed."), _("Purchase id: ") + DSCasts::ToString(*purchaseId));
			delete purchaseId;
			purchaseId = NULL;
		} else {
			rit->Log(true, _N("PurchaseModulePanel"), _("Could not remove purchase."), _("Purchase id: ") + DSCasts::ToString(*purchaseId));

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not remove purchase."),
				_("Remove purchase")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	yesNoDialog->Destroy();
}

void PurchasesModulePanel::Find() {
	purchasesListCtrl->SetFocus();

	if(!purchasesFindDialog) {
		purchasesFindDialog = new PurchasesFindDialog(this, -1, rit);
		purchasesFindDialog->CenterOnScreen();
	}

	if(purchasesFindDialog->ShowModal() == wxID_OK) {
		ClearPurchasesListCtrl();

		PurchaseEntity *purchaseEntity = purchasesFindDialog->GetPurchaseEntity();

		PurchaseController purchaseController(rit);
		purchaseController.Find(purchaseEntity);

		ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, purchaseController.GetCount());
		progressDialog->Center();
		progressDialog->Start();
		unsigned int i = 0;
		while(purchaseController.Run()) {
			purchaseController.Get(purchaseEntity);
			AddOrChangeToPurchasesListCtrl(purchaseEntity, true);
			progressDialog->SetValue(++i);
			if(progressDialog->IsCanceled()) {
				purchaseController.Reset();
				break;
			}
		}
		progressDialog->Stop();

		delete purchaseEntity;
		purchaseEntity = NULL;
	}

	purchasesListCtrl->SetFocus();
}

void PurchasesModulePanel::Callback(void *data) {
	PurchaseEntity *purchaseEntity = static_cast<PurchaseEntity *>(data);
	if(purchaseEntity)
		AddOrChangeToPurchasesListCtrl(purchaseEntity, false);
}

void PurchasesModulePanel::AddOrChangeToPurchasesListCtrl(PurchaseEntity *purchaseEntity, const bool justAppend) {
	unsigned int *foundPurchaseId = NULL;
	const unsigned int purchaseId = purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt();

	if(!justAppend) {
		const unsigned int count = purchasesListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			foundPurchaseId = static_cast<unsigned int *>(purchasesListCtrl->GetPointer(i, 0));
			if(foundPurchaseId && *foundPurchaseId == purchaseId)
				break;
			foundPurchaseId = NULL;
		}
	}
	if(!foundPurchaseId && !justAppend)
		return;

	if(!foundPurchaseId)
		foundPurchaseId = new unsigned int(purchaseId);

	wxColour backgroundColour;
	DSDateTime orderDate(DSQuery::GetDate(purchaseEntity->Get(_N("orderDate"))->ToString()));
	wxString done;
	if(!purchaseEntity->Get(_N("done"))->ToString().IsEmpty()) {
		DSDateTime doneDate(DSQuery::GetDate(purchaseEntity->Get(_N("done"))->ToString()));
		done = doneDate.ToDatabaseDate();
		backgroundColour.Set(0, 255, 0);
	}

	DSListCtrl::DSListCtrlRow *purchasesListCtrlRow = new DSListCtrl::DSListCtrlRow(foundPurchaseId);
	purchasesListCtrlRow->SetBackgroundColour(backgroundColour);

	purchasesListCtrlRow->Add(purchaseEntity->Get(_N("purchaseId"))->ToString());
	purchasesListCtrlRow->Add(done);
	purchasesListCtrlRow->Add(orderDate.ToDatabaseDate());
	purchasesListCtrlRow->Add(purchaseEntity->Get(_N("orderId"))->ToString());
	purchasesListCtrlRow->Add(purchaseEntity->Get(_N("distributorId"))->ToString());
	purchasesListCtrlRow->Add(purchaseEntity->Get(_N("clientId"))->ToString());
	purchasesListCtrlRow->Add(Tools::ConcatStringAndRemoveEmptyLines(purchaseEntity->Get(_N("distributorAddress"))->ToString()));
	purchasesListCtrlRow->Add(Tools::ConcatStringAndRemoveEmptyLines(purchaseEntity->Get(_N("billingAddress"))->ToString()));
	purchasesListCtrlRow->Add(Tools::ConcatStringAndRemoveEmptyLines(purchaseEntity->Get(_N("deliveryAddress"))->ToString()));
	purchasesListCtrlRow->Add(purchaseEntity->Get(_N("sum"))->ToCurrency().ToString());
	purchasesListCtrlRow->Add(purchaseEntity->Get(_N("purchaser"))->ToString());

	purchasesListCtrl->Set(purchasesListCtrlRow, true);
}

void PurchasesModulePanel::ClearPurchasesListCtrl() {
	purchasesListCtrl->CleanUp();
}

void PurchasesModulePanel::Export() {
	purchasesListCtrl->SetFocus();

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

			const unsigned int count = purchasesListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, true);

			for(unsigned int i = 0; i < count; i++) {
				exportLibrary.ExportAdd(PurchaseController::GetDirect(
					rit,
					*static_cast<unsigned int *>(purchasesListCtrl->GetPointer(i, 0))
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

void PurchasesModulePanel::Copy(const unsigned int *purchaseId) {
	purchasesListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to copy an purchase."), this))
		return;

	if(!purchaseId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the purchase you want to copy."),
			_("Copy purchase")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	PurchaseEntity *purchaseEntity = new PurchaseEntity(rit);
	PurchaseController purchaseController(rit);
	purchaseController.Get(DSCasts::ToString(*purchaseId));
	if(purchaseController.Run()) {
		purchaseController.Get(purchaseEntity);
		purchaseController.Reset();
		purchaseController.Change(purchaseEntity);
	} else {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Could not copy the purchase. Maybe the selected purchase is not in database anymore."),
			_("New purchase"),
			_("Create"),
			_("Cancel")
		);
		yesNoDialog->Center();
		yesNoDialog->ShowModal();
		yesNoDialog->Destroy();

		delete purchaseEntity;
		return;
	}

	PurchaseEditFrame *purchaseEditFrame = new PurchaseEditFrame(this, -1, rit, purchaseEntity, this, true, true);
	purchaseEditFrame->Center();
	purchaseEditFrame->Show();
}
