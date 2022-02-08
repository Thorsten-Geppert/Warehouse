#include "ProductsFullFindDialog.h"
#include "ProgressDialog.h"
#include "../lib/ProductController.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ProductsFullFindDialog, wxDialog)
	EVT_LIST_ITEM_ACTIVATED(XRCID("productsListCtrl"), ProductsFullFindDialog::UseDirectEvent)
	EVT_MENU(CLOSE_EVENT, ProductsFullFindDialog::CancelKeyEvent)
	EVT_MENU(FIND_EVENT, ProductsFullFindDialog::FindKeyEvent)
	EVT_MENU(USE_EVENT, ProductsFullFindDialog::UseKeyEvent)
END_EVENT_TABLE()

ProductsFullFindDialog::ProductsFullFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const unsigned int distributorId
) : productId(
	0
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ProductsFullFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ProductsFullFindDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton     = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	useToolbarButton      = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	cancelToolbarButton   = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	productIdTextCtrl     = XRCCTRL(*this, "productIdTextCtrl", DynSoft::DSTextCtrl);
	articleNumberTextCtrl = XRCCTRL(*this, "articleNumberTextCtrl", wxTextCtrl);
	groupComboBox         = XRCCTRL(*this, "groupComboBox", wxComboBox);
	distributorComboBox   = XRCCTRL(*this, "distributorComboBox", wxComboBox);
	manufacturerComboBox  = XRCCTRL(*this, "manufacturerComboBox", wxComboBox);
	publisherComboBox     = XRCCTRL(*this, "publisherComboBox", wxComboBox);
	eanTextCtrl           = XRCCTRL(*this, "eanTextCtrl", wxTextCtrl);
	shortNameTextCtrl     = XRCCTRL(*this, "shortNameTextCtrl", wxTextCtrl);
	nameTextCtrl          = XRCCTRL(*this, "nameTextCtrl", wxTextCtrl);
	descriptionTextCtrl   = XRCCTRL(*this, "descriptionTextCtrl", wxTextCtrl);
	noteTextCtrl          = XRCCTRL(*this, "noteTextCtrl", wxTextCtrl);
	isbn10TextCtrl        = XRCCTRL(*this, "isbn10TextCtrl", wxTextCtrl);
	isbn13TextCtrl        = XRCCTRL(*this, "isbn13TextCtrl", wxTextCtrl);
	productsListCtrl      = XRCCTRL(*this, "productsListCtrl", DynSoft::DSListCtrl);

	productIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	productsListCtrl->InsertColumn(0, _("Product id"), wxLIST_FORMAT_RIGHT, 90);
	productsListCtrl->InsertColumn(1, _("Article number"), wxLIST_FORMAT_LEFT, 90);
	productsListCtrl->InsertColumn(2, _("EAN"), wxLIST_FORMAT_LEFT, 80);
	productsListCtrl->InsertColumn(3, _("Short name"), wxLIST_FORMAT_LEFT, 180);
	productsListCtrl->InsertColumn(4, _("Name"), wxLIST_FORMAT_LEFT, 270);
	productsListCtrl->InsertColumn(5, _("Price (without tax)"), wxLIST_FORMAT_RIGHT, 90);
	productsListCtrl->InsertColumn(6, _("Price (with tax)"), wxLIST_FORMAT_RIGHT, 90);
	productsListCtrl->InsertColumn(7, _("ISBN 10"), wxLIST_FORMAT_LEFT, 80);
	productsListCtrl->InsertColumn(8, _("ISBN 13"), wxLIST_FORMAT_LEFT, 100);

	CONNECT_TOOLBARBUTTON(findToolbarButton, ProductsFullFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(useToolbarButton, ProductsFullFindDialog::UseEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ProductsFullFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'u',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(640, 480));

	if(distributorId > 0) {
		distributorComboBox->SetValue(DSCasts::ToString(distributorId));
		distributorComboBox->Disable();
	}

	ICON();
}

ProductsFullFindDialog::~ProductsFullFindDialog() {
	ClearProductsListCtrl();
}

int ProductsFullFindDialog::ShowModal() {
	productIdTextCtrl->SetFocus();
	return wxDialog::ShowModal();
}

void ProductsFullFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void ProductsFullFindDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void ProductsFullFindDialog::UseDirectEvent(wxListEvent &event) {
	Use();
}

void ProductsFullFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ProductsFullFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void ProductsFullFindDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void ProductsFullFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ProductsFullFindDialog::Find() {
	ProductEntity productEntity(rit);

	productEntity.SetValue(_N("productId"), productIdTextCtrl->GetValue());
	productEntity.SetValue(_N("articleNumber"), articleNumberTextCtrl->GetValue());
	productEntity.SetValue(_N("group"), groupComboBox->GetValue());
	productEntity.SetValue(_N("distributorId"), DSCasts::ToUnsignedInt(distributorComboBox->GetValue()));
	productEntity.SetValue(_N("manufacturer"), manufacturerComboBox->GetValue());
	productEntity.SetValue(_N("publisher"), publisherComboBox->GetValue());
	productEntity.SetValue(_N("ean"), eanTextCtrl->GetValue());
	productEntity.SetValue(_N("shortName"), shortNameTextCtrl->GetValue());
	productEntity.SetValue(_N("name1"), nameTextCtrl->GetValue());
	productEntity.SetValue(_N("description"), descriptionTextCtrl->GetValue());
	productEntity.SetValue(_N("note"), noteTextCtrl->GetValue());
	productEntity.SetValue(_N("isbn10"), isbn10TextCtrl->GetValue());
	productEntity.SetValue(_N("isbn13"), isbn13TextCtrl->GetValue());

	ClearProductsListCtrl();

	ProductController productController(rit);
	productController.Find(&productEntity, true);

	ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, productController.GetCount());
	progressDialog->Center();
	progressDialog->Start();
	unsigned int i = 0;
	while(productController.Run()) {
		productController.Get(&productEntity);
		AddToProductsListCtrl(&productEntity);
		progressDialog->SetValue(++i);
		if(progressDialog->IsCanceled()) {
			productController.Reset();
			break;
		}
	}
	progressDialog->Stop();

	if(i > 0)
		productsListCtrl->SetFocus();
}

void ProductsFullFindDialog::Use() {
	unsigned int *productId = static_cast<unsigned int *>(productsListCtrl->GetSingleSelectionPointer());
	if(!productId) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a product."),
			_("Use")
		);
		errorMessage->ShowModal();
	} else {
		this->productId = *productId;
		EndModal(wxID_OK);
	}
}

void ProductsFullFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

ProductEntity *ProductsFullFindDialog::GetProductEntity() const {
	if(productId == 0)
		return NULL;

	ProductEntity *productEntity = NULL;
	ProductController productController(rit);
	productController.Get(DSCasts::ToString(productId));
	if(productController.Run()) {
		productEntity = new ProductEntity(rit);
		productController.Get(productEntity);
		productController.Reset();
	}

	return productEntity;
}

void ProductsFullFindDialog::ClearProductsListCtrl() {
	productsListCtrl->CleanUp();
}

void ProductsFullFindDialog::AddToProductsListCtrl(ProductEntity *productEntity) {
	if(!productEntity)
		return;

	DSListCtrl::DSListCtrlRow *productsListCtrlRow = new DSListCtrl::DSListCtrlRow(new unsigned int(productEntity->Get(_N("productId"))->ToUnsignedInt()));

	productsListCtrlRow->Add(productEntity->Get(_N("productId"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("articleNumber"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("ean"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("shortName"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("name1"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("priceWithoutTax"))->ToCurrency().ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("priceWithTax"))->ToCurrency().ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("isbn10"))->ToString());
	productsListCtrlRow->Add(productEntity->Get(_N("isbn13"))->ToString());

	productsListCtrl->Set(productsListCtrlRow, true);
}
