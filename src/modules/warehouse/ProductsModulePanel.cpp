#include "ProductsModulePanel.h"

#include <wx/xrc/xmlres.h>

#include "ProductEditFrame.h"
#include "ProgressDialog.h"
#include "RemoveDialog.h"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"
#include "YesNoDialog.h"

#include "../lib/PermissionLibrary.h"
#include "../lib/ProductController.h"
#include "../lib/ReferencesType.h"
#include "../lib/OrderController.h"
#include "../lib/ConstantsLibrary.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"

BEGIN_EVENT_TABLE(ProductsModulePanel, ModulePanel)
	EVT_LIST_ITEM_ACTIVATED(XRCID("productsListCtrl"), ProductsModulePanel::DirectEditEvent)
	EVT_MENU(NEW_EVENT, ProductsModulePanel::NewKeyEvent)
	EVT_MENU(EDIT_EVENT, ProductsModulePanel::EditKeyEvent)
	EVT_MENU(REMOVE_EVENT, ProductsModulePanel::RemoveKeyEvent)
	EVT_MENU(FIND_EVENT, ProductsModulePanel::FindKeyEvent)
	EVT_MENU(EXPORT_EVENT, ProductsModulePanel::ExportKeyEvent)
	EVT_MENU(STOCKS_EVENT, ProductsModulePanel::StocksKeyEvent)
END_EVENT_TABLE()

ProductsModulePanel::ProductsModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("ProductsModulePanel")
) {
	// Toolbar
	newToolbarButton    = XRCCTRL(*this, "newToolbarButton", ToolbarButton);
	editToolbarButton   = XRCCTRL(*this, "editToolbarButton", ToolbarButton);
	removeToolbarButton = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	findToolbarButton   = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	stocksToolbarButton = XRCCTRL(*this, "stocksToolbarButton", ToolbarButton);
	exportToolbarButton = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);

	// Products
	productsListCtrl    = XRCCTRL(*this, "productsListCtrl", DynSoft::DSListCtrl);

	productsListCtrl->InsertColumn(0, _("Product id"), wxLIST_FORMAT_RIGHT, 120);
	productsListCtrl->InsertColumn(1, _("Article number"), wxLIST_FORMAT_RIGHT, 120);
	productsListCtrl->InsertColumn(2, _("EAN"), wxLIST_FORMAT_RIGHT, 80);
	productsListCtrl->InsertColumn(3, _("Short name"), wxLIST_FORMAT_LEFT, 180);
	productsListCtrl->InsertColumn(4, _("Name"), wxLIST_FORMAT_LEFT, 270);
	productsListCtrl->InsertColumn(5, _("Price (without tax)"), wxLIST_FORMAT_RIGHT, 150);
	productsListCtrl->InsertColumn(6, _("Price (with tax)"), wxLIST_FORMAT_RIGHT, 150);
	productsListCtrl->InsertColumn(7, _("ISBN 10"), wxLIST_FORMAT_LEFT, 80);
	productsListCtrl->InsertColumn(8, _("ISBN 13"), wxLIST_FORMAT_LEFT, 90);
	productsListCtrl->InsertColumn(9, _("Weight"), wxLIST_FORMAT_RIGHT, 90);

	// Events
	CONNECT_TOOLBARBUTTON(newToolbarButton, ProductsModulePanel::NewEvent);
	CONNECT_TOOLBARBUTTON(editToolbarButton, ProductsModulePanel::EditEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, ProductsModulePanel::RemoveEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, ProductsModulePanel::FindEvent);
	CONNECT_TOOLBARBUTTON(stocksToolbarButton, ProductsModulePanel::StocksEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, ProductsModulePanel::ExportEvent);

	wxAcceleratorEntry acceleratorEntries[6];
	acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 'n', NEW_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, (int) 'e', EDIT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL, (int) 'd', REMOVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL, (int) 'f', FIND_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL, (int) 's', STOCKS_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL, (int) 'k', EXPORT_EVENT);
	wxAcceleratorTable acceleratorTable(6, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Stuff
	productsFindDialog = NULL;

	stocksFrame = new StocksFrame(this, -1, rit);
	stocksFrame->CenterOnScreen();
}

ProductsModulePanel::~ProductsModulePanel() {
	ClearProductsListCtrl();
}

void ProductsModulePanel::Load() {
	productsListCtrl->SetFocus();
}

void ProductsModulePanel::Unload() {
}

void ProductsModulePanel::NewEvent(wxMouseEvent &event) {
	New(static_cast<unsigned int *>(productsListCtrl->GetSingleSelectionPointer()));
	SKIP();
}

void ProductsModulePanel::EditEvent(wxMouseEvent &event) {
	unsigned int *productId = static_cast<unsigned int *>(productsListCtrl->GetSingleSelectionPointer());
	Edit(productId);
	SKIP();
}

void ProductsModulePanel::DirectEditEvent(wxListEvent &event) {
	unsigned int *productId = static_cast<unsigned int *>(productsListCtrl->GetSingleSelectionPointer());
	Edit(productId);
}

void ProductsModulePanel::RemoveEvent(wxMouseEvent &event) {
	Remove();
	SKIP();
}

void ProductsModulePanel::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void ProductsModulePanel::NewKeyEvent(wxCommandEvent &event) {
	New(static_cast<unsigned int *>(productsListCtrl->GetSingleSelectionPointer()));
}

void ProductsModulePanel::EditKeyEvent(wxCommandEvent &event) {
	unsigned int *productId = static_cast<unsigned int *>(productsListCtrl->GetSingleSelectionPointer());
	Edit(productId);
}

void ProductsModulePanel::RemoveKeyEvent(wxCommandEvent &event) {
	Remove();
}

void ProductsModulePanel::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void ProductsModulePanel::StocksEvent(wxMouseEvent &event) {
	Stocks();
	SKIP();
}

void ProductsModulePanel::StocksKeyEvent(wxCommandEvent &event) {
	Stocks();
}

void ProductsModulePanel::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void ProductsModulePanel::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void ProductsModulePanel::New(unsigned int *productId) {
	productsListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to create a new product."), this))
		return;

	ProductEntity *productEntity = NULL;
	if(productId) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("Do you want to create an product based on the selected one?"),
			_("New product"),
			_("Create based on"),
			_("Do not create based on")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES) {
			ProductController productController(rit);
			productEntity = productController.Load(*productId);
			if(productEntity) {
				productController.Change(productEntity);
			} else {
				YesNoDialog *yesNoDialog = new YesNoDialog(
					this,
					-1,
					rit,
					_("Could not create an product based on the selected one. Will create a new product. Maybe the selected product is not in database anymore."),
					_("New product"),
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

	if(!productEntity)
		productEntity = new ProductEntity(rit);

	ProductEditFrame *productEditFrame = new ProductEditFrame(this, -1, rit, productEntity, this);
	productEditFrame->Center();
	productEditFrame->Show();
}

void ProductsModulePanel::Edit(unsigned int *productId) {
	productsListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to open a product."), this))
		return;

	if(!productId) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the product you want to edit."),
			_("Edit product")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("products"), *productId);
	if(blockType) {
		ProductEditFrame *productEditFrame = static_cast<ProductEditFrame *>(blockType->GetData());
		if(productEditFrame) {
			productEditFrame->Raise();
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The product you want to edit is allready opened."),
				_("Edit product")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
		return;
	}

	ProductController productController(rit);
	ProductEntity *productEntity = productController.Load(*productId);
	if(productEntity) {
		ProductEditFrame *productEditFrame = new ProductEditFrame(this, -1, rit, productEntity, this);
		productEditFrame->Center();
		productEditFrame->Show();

		rit->Block(_N("products"), productEntity->Get(_N("productId"))->ToUnsignedInt(), productEditFrame);
	} else {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not find the product in the database."),
			_("Edit product")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}
}

void ProductsModulePanel::Remove() {
	productsListCtrl->SetFocus();

	if(!rit->Access(A_DELETE, _("You have no permission to remove a product."), this))
		return;

	unsigned int *productId = static_cast<unsigned int *>(productsListCtrl->GetSingleSelectionPointer());

	if(!productId) {
		rit->Log(true, _N("ProductsModulePanel"), _("Could not remove product."), _("No entry selected."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the product you want to remove."),
			_("Remove product")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	BlockType *blockType = rit->IsBlocked(_N("products") , *productId);
	if(blockType) {
		rit->Log(true, _N("ProductsModulePanel"), _("Could not remove product."), _("The product is open."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The product you want to remove is open."),
			_("Remove product")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		ProductEditFrame *productEditFrame = static_cast<ProductEditFrame *>(blockType->GetData());
		if(productEditFrame)
			productEditFrame->Raise();

		return;
	}

	ReferencesType referencesType;
	OrderController orderController(rit);
	orderController.GetReferences(ConstantsLibrary::PRODUCT_KIND, *productId, &referencesType);
	if(referencesType.GetCount() != 0) {
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
		_("Do you want to delete the selected product?"),
		_("Remove product"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		ProductEntity productEntity(rit);
		productEntity.SetValue(_N("productId"), *productId);
		ProductController productController(rit);
		if(productController.Delete(&productEntity)) {
			productsListCtrl->DeleteItem(productsListCtrl->GetPosition(productId));
			rit->Log(false, _N("ProductsModulePanel"), _("The product is removed."), _("Product id: ") + DSCasts::ToString(*productId));
			delete productId;
			productId = NULL;
		} else {
			rit->Log(true, _N("ProductsModulePanel"), _("Could not remove product."), _("Product id: ") + DSCasts::ToString(*productId));

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not remove product."),
				_("Remove product")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	yesNoDialog->Destroy();
}

void ProductsModulePanel::Find() {
	productsListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to find products."), this))
		return;

	if(!productsFindDialog) {
		productsFindDialog = new ProductsFindDialog(this, -1, rit);
		productsFindDialog->CenterOnScreen();
	}

	if(productsFindDialog->ShowModal() == wxID_OK) {
		ClearProductsListCtrl();

		ProductEntity *productEntity = productsFindDialog->GetProductEntity();

		ProductController productController(rit);
		productController.Find(productEntity);

		ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, productController.GetCount());
		progressDialog->Center();
		progressDialog->Start();
		unsigned int i = 0;
		while(productController.Run()) {
			productController.Get(productEntity);
			AddOrChangeToProductsListCtrl(productEntity, true);
			progressDialog->SetValue(++i);
			if(progressDialog->IsCanceled()) {
				productController.Reset();
				break;
			}
		}
		progressDialog->Stop();

		delete productEntity;
		productEntity = NULL;
	}

	productsListCtrl->SetFocus();
}

void ProductsModulePanel::Callback(void *data) {
	ProductEntity *productEntity = static_cast<ProductEntity *>(data);
	if(productEntity) {
		AddOrChangeToProductsListCtrl(productEntity, false);
	}
}

void ProductsModulePanel::AddOrChangeToProductsListCtrl(ProductEntity *productEntity, const bool justAppend) {
	unsigned int *foundProductId = NULL;
	const unsigned int productId = productEntity->Get(_N("productId"))->ToUnsignedInt();

	if(!justAppend) {
		const unsigned int count = productsListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			foundProductId = static_cast<unsigned int *>(productsListCtrl->GetPointer(i, 0));
			if(foundProductId && *foundProductId == productId)
				break;
			foundProductId = NULL;
		}
	}

	if(!foundProductId && !justAppend)
		return;

	if(!foundProductId)
		foundProductId = new unsigned int(productId);

	DSListCtrl::DSListCtrlRow *productsListCtrlRow = new DSListCtrl::DSListCtrlRow(foundProductId);
	productsListCtrlRow->SetBackgroundColour(wxColour(productEntity->Get(_N("available"))->ToBool() ? wxColour(165, 255, 150) : wxColour(255, 177, 150)));

	productsListCtrlRow->Add(productEntity->Get(_N("productId"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("articleNumber"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("ean"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("shortName"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("name1"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("priceWithoutTax"))->ToCurrency().ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("priceWithTax"))->ToCurrency().ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("isbn10"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("isbn13"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("weight"))->ToCurrency().ToString());

	productsListCtrl->Set(productsListCtrlRow, true);
}

void ProductsModulePanel::ClearProductsListCtrl() {
	productsListCtrl->CleanUp();
}

void ProductsModulePanel::Export() {
	productsListCtrl->SetFocus();

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

			const unsigned int count = productsListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, true);

			for(unsigned int i = 0; i < count; i++) {
				exportLibrary.ExportAdd(ProductController::GetDirect(
					rit,
					*static_cast<unsigned int *>(productsListCtrl->GetPointer(i, 0))
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

void ProductsModulePanel::Stocks() {
	if(!rit->Access(A_READ, _("You have no permission to view bookings."), this))
		return;

	stocksFrame->Show(!stocksFrame->IsShown());
}
