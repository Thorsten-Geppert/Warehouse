#include "VouchersModulePanel.h"

#include <wx/xrc/xmlres.h>

#include "ProgressDialog.h"
#include "RemoveDialog.h"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"
#include "YesNoDialog.h"
#include "VoucherEditFrame.h"

#include "../lib/PermissionLibrary.h"
#include "../lib/VoucherController.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"

BEGIN_EVENT_TABLE(VouchersModulePanel, ModulePanel)
	EVT_LIST_ITEM_ACTIVATED(XRCID("vouchersListCtrl"), VouchersModulePanel::DirectEditEvent)
	EVT_MENU(NEW_EVENT, VouchersModulePanel::NewKeyEvent)
	EVT_MENU(EDIT_EVENT, VouchersModulePanel::EditKeyEvent)
	EVT_MENU(REMOVE_EVENT, VouchersModulePanel::RemoveKeyEvent)
	EVT_MENU(FIND_EVENT, VouchersModulePanel::FindKeyEvent)
	EVT_MENU(EXPORT_EVENT, VouchersModulePanel::ExportKeyEvent)
	EVT_MENU(CASH_JOURNAL_EVENT, VouchersModulePanel::CashJournalKeyEvent)
END_EVENT_TABLE()

VouchersModulePanel::VouchersModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("VouchersModulePanel")
), cashJournalFrame(
	NULL
) {
	// Toolbar
	newToolbarButton         = XRCCTRL(*this, "newToolbarButton", ToolbarButton);
	editToolbarButton        = XRCCTRL(*this, "editToolbarButton", ToolbarButton);
	removeToolbarButton      = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	findToolbarButton        = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	//cashJournalToolbarButton = XRCCTRL(*this, "cashJournalToolbarButton", ToolbarButton);
	exportToolbarButton      = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);

	// Products
	vouchersListCtrl = XRCCTRL(*this, "vouchersListCtrl", DynSoft::DSListCtrl);

	vouchersListCtrl->InsertColumn(0, _("Invoice date"), wxLIST_FORMAT_RIGHT, 130);
	vouchersListCtrl->InsertColumn(1, _("Voucher id"), wxLIST_FORMAT_RIGHT, 110);
	vouchersListCtrl->InsertColumn(2, _("Description"), wxLIST_FORMAT_LEFT, 120);
	vouchersListCtrl->InsertColumn(3, _("Invoice id"), wxLIST_FORMAT_RIGHT, 140);
	vouchersListCtrl->InsertColumn(4, _("Company"), wxLIST_FORMAT_LEFT, 220);
	vouchersListCtrl->InsertColumn(5, _("Price"), wxLIST_FORMAT_RIGHT, 100);
	vouchersListCtrl->InsertColumn(6, _("Reduced tax"), wxLIST_FORMAT_RIGHT, 140);
	vouchersListCtrl->InsertColumn(7, _("Full tax"), wxLIST_FORMAT_RIGHT, 140);
	vouchersListCtrl->InsertColumn(8, _("Sum"), wxLIST_FORMAT_RIGHT, 130);

	// Events
	CONNECT_TOOLBARBUTTON(newToolbarButton, VouchersModulePanel::NewEvent);
	CONNECT_TOOLBARBUTTON(editToolbarButton, VouchersModulePanel::EditEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, VouchersModulePanel::RemoveEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, VouchersModulePanel::FindEvent);
	//CONNECT_TOOLBARBUTTON(cashJournalToolbarButton, VouchersModulePanel::CashJournalEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, VouchersModulePanel::ExportEvent);

	wxAcceleratorEntry acceleratorEntries[6];
	acceleratorEntries[0].Set(wxACCEL_CTRL,    (int) 'n', NEW_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'e', EDIT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'd', REMOVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'f', FIND_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,    (int) 'k', EXPORT_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL,    (int) 'j', CASH_JOURNAL_EVENT);
	wxAcceleratorTable acceleratorTable(6, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Stuff
	vouchersFindDialog = NULL;
}

VouchersModulePanel::~VouchersModulePanel() {
	ClearVouchersListCtrl();
}

void VouchersModulePanel::Load() {
	vouchersListCtrl->SetFocus();
}

void VouchersModulePanel::Unload() {
}

void VouchersModulePanel::NewEvent(wxMouseEvent &event) {
	New(static_cast<unsigned int *>(vouchersListCtrl->GetSingleSelectionPointer()));
	SKIP();
}

void VouchersModulePanel::EditEvent(wxMouseEvent &event) {
	unsigned int *voucherId = static_cast<unsigned int *>(vouchersListCtrl->GetSingleSelectionPointer());
	Edit(voucherId);
	SKIP();
}

void VouchersModulePanel::DirectEditEvent(wxListEvent &event) {
	unsigned int *voucherId = static_cast<unsigned int *>(vouchersListCtrl->GetSingleSelectionPointer());
	Edit(voucherId);
}

void VouchersModulePanel::RemoveEvent(wxMouseEvent &event) {
	Remove();
	SKIP();
}

void VouchersModulePanel::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void VouchersModulePanel::NewKeyEvent(wxCommandEvent &event) {
	New(static_cast<unsigned int *>(vouchersListCtrl->GetSingleSelectionPointer()));
}

void VouchersModulePanel::EditKeyEvent(wxCommandEvent &event) {
	unsigned int *voucherId = static_cast<unsigned int *>(vouchersListCtrl->GetSingleSelectionPointer());
	Edit(voucherId);
}

void VouchersModulePanel::CashJournalEvent(wxMouseEvent &event) {
	CashJournal();
	SKIP();
}

void VouchersModulePanel::CashJournalKeyEvent(wxCommandEvent &event) {
	CashJournal();
}

void VouchersModulePanel::RemoveKeyEvent(wxCommandEvent &event) {
	Remove();
}

void VouchersModulePanel::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void VouchersModulePanel::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void VouchersModulePanel::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void VouchersModulePanel::New(unsigned int *voucherId) {
	if(!rit->Access(A_WRITE, _("You have no permission to create a new voucher."), this))
		return;

	VoucherEntity *voucherEntity = NULL;
	if(voucherId) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you want to create an voucher based on the selected one?"),
			_("New voucher"),
			_("Create based on"),
			_("Do not create based on")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES) {
			VoucherController voucherController(rit);
			voucherEntity = voucherController.Load(*voucherId);
			if(voucherEntity) {
				voucherController.Change(voucherEntity);
			} else {
				YesNoDialog *yesNoDialog = new YesNoDialog(
					this,
					-1,
					rit,
					_("Could not create an voucher based on the selected one. Will create a new voucher. Maybe the selected voucher is not in database anymore."),
					_("New voucher"),
					_("Create"),
					_("Cancel")
				);
				yesNoDialog->Center();
				const bool ok = yesNoDialog->ShowModal() == wxID_OK;
				yesNoDialog->Destroy();

				if(!ok)
					return;
			}
		}
		yesNoDialog->Destroy();
	}

	if(!voucherEntity)
		voucherEntity = new VoucherEntity(rit);
	VoucherEditFrame *voucherEditFrame = new VoucherEditFrame(this, -1, rit, voucherEntity, this);
	voucherEditFrame->Center();
	voucherEditFrame->Show();
}

void VouchersModulePanel::Edit(unsigned int *voucherId) {
	vouchersListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to open a voucher."), this))
		return;

	if(!voucherId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the voucher you want to edit."),
			_("Edit voucher")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("vouchers"), *voucherId);
	if(blockType) {
		VoucherEditFrame *voucherEditFrame = static_cast<VoucherEditFrame *>(blockType->GetData());
		if(voucherEditFrame) {
			voucherEditFrame->Raise();
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The voucher you want to edit is allready opened."),
				_("Edit voucher")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
		return;
	}

	VoucherController voucherController(rit);
	VoucherEntity *voucherEntity = voucherController.Load(*voucherId);
	if(voucherEntity) {
		VoucherEditFrame *voucherEditFrame = new VoucherEditFrame(this, -1, rit, voucherEntity, this);
		voucherEditFrame->Center();
		voucherEditFrame->Show();

		rit->Block(_N("vouchers"), voucherEntity->Get(_N("voucherId"))->ToUnsignedInt(), voucherEditFrame);
	} else {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not find the voucher in the database."),
			_("Edit voucher")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}
}

void VouchersModulePanel::Remove() {
	vouchersListCtrl->SetFocus();

	if(!rit->Access(A_DELETE, _("You have no permission to remove a voucher."), this))
		return;

	unsigned int *voucherId = static_cast<unsigned int *>(vouchersListCtrl->GetSingleSelectionPointer());

	if(!voucherId) {
		rit->Log(true, _N("VouchersModulePanel"), _("Could not remove voucher."), _("No entry selected."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the voucher you want to remove."),
			_("Remove voucher")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("vouchers") , *voucherId);
	if(blockType) {
		rit->Log(true, _N("VouchersModulePanel"), _("Could not remove voucher."), _("The voucher is open."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The voucher you want to remove is open."),
			_("Remove voucher")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		VoucherEditFrame *voucherEditFrame = static_cast<VoucherEditFrame *>(blockType->GetData());
		if(voucherEditFrame)
			voucherEditFrame->Raise();

		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you want to delete the selected voucher?"),
		_("Remove voucher"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		VoucherEntity voucherEntity(rit);
		voucherEntity.SetValue(_N("voucherId"), *voucherId);
		VoucherController voucherController(rit);
		if(voucherController.Delete(&voucherEntity)) {
			vouchersListCtrl->DeleteItem(vouchersListCtrl->GetPosition(voucherId));
			rit->Log(false, _N("VouchersModulePanel"), _("The voucher is removed."), _("Voucher id: ") + DSCasts::ToString(*voucherId));
			delete voucherId;
			voucherId = NULL;
		} else {
			rit->Log(true, _N("VouchersModulePanel"), _("Could not remove voucher."), _("Voucher id: ") + DSCasts::ToString(*voucherId));

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not remove voucher."),
				_("Remove voucher")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	yesNoDialog->Destroy();
}

void VouchersModulePanel::Find() {
	vouchersListCtrl->SetFocus();
	if(!rit->Access(A_READ, _("You have no permission to find vouchers."), this))
		return;

	if(!vouchersFindDialog) {
		vouchersFindDialog = new VouchersFindDialog(this, -1, rit);
		vouchersFindDialog->CenterOnScreen();
	}

	if(vouchersFindDialog->ShowModal() == wxID_OK) {
		ClearVouchersListCtrl();

		VoucherEntity *voucherEntity = vouchersFindDialog->GetVoucherEntity();

		VoucherController voucherController(rit);
		voucherController.Find(voucherEntity);

		ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, voucherController.GetCount());
		progressDialog->Center();
		progressDialog->Start();
		unsigned int i = 0;
		while(voucherController.Run()) {
			voucherController.Get(voucherEntity);
			AddOrChangeToVouchersListCtrl(voucherEntity, true);
			progressDialog->SetValue(++i);
			if(progressDialog->IsCanceled()) {
				voucherController.Reset();
				break;
			}
		}
		progressDialog->Stop();

		delete voucherEntity;
		voucherEntity = NULL;
	}

	vouchersListCtrl->SetFocus();
}

void VouchersModulePanel::Callback(void *data) {
	VoucherEntity *voucherEntity = static_cast<VoucherEntity *>(data);
	if(voucherEntity) {
		AddOrChangeToVouchersListCtrl(voucherEntity, false);
	}
}

void VouchersModulePanel::AddOrChangeToVouchersListCtrl(VoucherEntity *voucherEntity, const bool justAppend) {
	unsigned int *foundVoucherId = NULL;
	const unsigned int voucherId = voucherEntity->Get(_N("voucherId"))->ToUnsignedInt();

	if(!justAppend) {
		const unsigned int count = vouchersListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			foundVoucherId = static_cast<unsigned int *>(vouchersListCtrl->GetPointer(i, 0));
			if(foundVoucherId && *foundVoucherId == voucherId)
				break;
			foundVoucherId = NULL;
		}
	}

	if(!foundVoucherId && !justAppend)
		return;

	if(!foundVoucherId)
		foundVoucherId = new unsigned int(voucherId);

	DSListCtrl::DSListCtrlRow *vouchersListCtrlRow = new DSListCtrl::DSListCtrlRow(foundVoucherId);

	wxString company(voucherEntity->Get(_N("company"))->ToString());
	company.Replace(_N("\n"), _N(", "));

	vouchersListCtrlRow->Add(voucherEntity->Get(_N("invoiceDate"))->ToString());
	vouchersListCtrlRow->Add(voucherEntity->Get(_N("voucherId"))->ToString());
	vouchersListCtrlRow->Add(voucherEntity->Get(_N("description"))->ToString());
	vouchersListCtrlRow->Add(voucherEntity->Get(_N("invoiceId"))->ToString());
	vouchersListCtrlRow->Add(company);
	vouchersListCtrlRow->Add(voucherEntity->Get(_N("priceWithoutTax"))->ToCurrency().ToString());
	vouchersListCtrlRow->Add(voucherEntity->Get(_N("reducedTax"))->ToCurrency().ToString());
	vouchersListCtrlRow->Add(voucherEntity->Get(_N("fullTax"))->ToCurrency().ToString());
	vouchersListCtrlRow->Add(voucherEntity->Get(_N("priceWithTax"))->ToCurrency().ToString());

	vouchersListCtrl->Set(vouchersListCtrlRow, true);
}

void VouchersModulePanel::ClearVouchersListCtrl() {
	vouchersListCtrl->CleanUp();
}

void VouchersModulePanel::Export() {
	vouchersListCtrl->SetFocus();

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

			const unsigned int count = vouchersListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, true);

			for(unsigned int i = 0; i < count; i++) {
				exportLibrary.ExportAdd(VoucherController::GetDirect(
					rit,
					*static_cast<unsigned int *>(vouchersListCtrl->GetPointer(i, 0))
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

void VouchersModulePanel::CashJournal() {
	if(!cashJournalFrame) {
		cashJournalFrame = new CashJournalFrame(this, -1, rit);
		cashJournalFrame->Center();
	}
	
	cashJournalFrame->Show(!cashJournalFrame->IsShown());
}
