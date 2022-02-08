#include "ProductsFindDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ProductsFindDialog, wxDialog)
	EVT_MENU(FIND_EVENT, ProductsFindDialog::FindKeyEvent)
	EVT_MENU(CANCEL_EVENT, ProductsFindDialog::CancelKeyEvent)
END_EVENT_TABLE()

ProductsFindDialog::ProductsFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ProductsFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ProductsFindDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton     = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
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

	productIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(findToolbarButton, ProductsFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ProductsFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();
}

int ProductsFindDialog::ShowModal() {
	productIdTextCtrl->SetFocus();
	return wxDialog::ShowModal();
}

void ProductsFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void ProductsFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ProductsFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void ProductsFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ProductsFindDialog::Find() {
	EndModal(wxID_OK);
}

void ProductsFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

ProductEntity *ProductsFindDialog::GetProductEntity() const {
	ProductEntity *productEntity = new ProductEntity(rit);

	productEntity->SetValue(_N("productId"), productIdTextCtrl->GetValue());
	productEntity->SetValue(_N("articleNumber"), articleNumberTextCtrl->GetValue());
	productEntity->SetValue(_N("group"), groupComboBox->GetValue());
	productEntity->SetValue(_N("distributorId"), DSCasts::ToUnsignedInt(distributorComboBox->GetValue()));
	productEntity->SetValue(_N("manufacturer"), manufacturerComboBox->GetValue());
	productEntity->SetValue(_N("publisher"), publisherComboBox->GetValue());
	productEntity->SetValue(_N("ean"), eanTextCtrl->GetValue());
	productEntity->SetValue(_N("shortName"), shortNameTextCtrl->GetValue());
	productEntity->SetValue(_N("name1"), nameTextCtrl->GetValue());
	productEntity->SetValue(_N("description"), descriptionTextCtrl->GetValue());
	productEntity->SetValue(_N("note"), noteTextCtrl->GetValue());
	productEntity->SetValue(_N("isbn10"), isbn10TextCtrl->GetValue());
	productEntity->SetValue(_N("isbn13"), isbn13TextCtrl->GetValue());

	return productEntity;
}
