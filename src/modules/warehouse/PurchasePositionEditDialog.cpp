#include "PurchasePositionEditDialog.h"
#include <DSCurrency.h>
#include "../lib/PurchaseController.h"
#include "../lib/CalculationLibrary.h"
#include "../lib/ProductController.h"
#include "../lib/CommonPreferencesController.h"
#include "ProductsFullFindDialog.h"
#include "TemplateSelectDialog.h"
#include "YesNoDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(PurchasePositionEditDialog, wxDialog)
	EVT_CLOSE(PurchasePositionEditDialog::OnClose)
	EVT_TEXT(XRCID("amountTextCtrl"), PurchasePositionEditDialog::RefreshSumEvent)
	EVT_TEXT(XRCID("productIdTextCtrl"), PurchasePositionEditDialog::EditProductIdEvent)
	EVT_TEXT(XRCID("priceTextCtrl"), PurchasePositionEditDialog::RefreshSumEvent)
	EVT_TEXT(XRCID("reductionTextCtrl"), PurchasePositionEditDialog::RefreshSumEvent)
	EVT_MENU(CLOSE_EVENT, PurchasePositionEditDialog::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, PurchasePositionEditDialog::SaveKeyEvent)
	EVT_MENU(FIND_PRODUCT_EVENT, PurchasePositionEditDialog::FindProductKeyEvent)
END_EVENT_TABLE()

PurchasePositionEditDialog::PurchasePositionEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	PurchasePositionEntity *purchasePositionEntity,
	const unsigned int distributorId
) : bind(
	purchasePositionEntity
), changed(
	false
), productEntity(
	rit
) {
	SetId(id);
	this->rit           = rit;
	this->distributorId = distributorId;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("PurchasePositionEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("PurchasePositionEditDialog"));

	mainPanel                   = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton           = XRCCTRL(*this, "saveToolbarButton",           ToolbarButton);
	closeToolbarButton          = XRCCTRL(*this, "closeToolbarButton",          ToolbarButton);
	findArticleToolbarButton    = XRCCTRL(*this, "findArticleToolbarButton",    ToolbarButton);
	amountTextCtrl              = XRCCTRL(*this, "amountTextCtrl",              DynSoft::DSTextCtrl);
	productIdTextCtrl           = XRCCTRL(*this, "productIdTextCtrl",           DynSoft::DSTextCtrl);
	priceTextCtrl               = XRCCTRL(*this, "priceTextCtrl",               DynSoft::DSTextCtrl);
	sumStaticText               = XRCCTRL(*this, "sumStaticText",               wxStaticText);
	productInformationsTextCtrl = XRCCTRL(*this, "productInformationsTextCtrl", wxTextCtrl);
	textStaticText              = XRCCTRL(*this, "textStaticText",              wxStaticText);
	textTextCtrl                = XRCCTRL(*this, "textTextCtrl",                DynSoft::DSTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, PurchasePositionEditDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(findArticleToolbarButton, PurchasePositionEditDialog::FindProductEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, PurchasePositionEditDialog::SaveEvent);

	textStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(PurchasePositionEditDialog::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'f',  FIND_PRODUCT_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling

	// Binding
	bind.Bind(_N("amount"),    amountTextCtrl);
	bind.Bind(_N("productId"), productIdTextCtrl);
	bind.Bind(_N("price"),     priceTextCtrl);
	bind.Bind(_N("text"),      textTextCtrl);

	amountTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	productIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	priceTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);

	amountTextCtrl->SetChange(&changed);
	productIdTextCtrl->SetChange(&changed);
	priceTextCtrl->SetChange(&changed);
	textTextCtrl->SetChange(&changed);

	productIdTextCtrl->SetFocus();

	if(purchasePositionEntity) {
		DSCurrency amount(purchasePositionEntity->Get(_N("amount"))->ToCurrency());
		purchasePositionEntity->SetValue(_N("amount"), amount);
		DSCurrency price(purchasePositionEntity->Get(_N("price"))->ToCurrency());
		purchasePositionEntity->SetValue(_N("price"), price);
		bind.Push();

		DSCurrency fullPrice(amount * price);

		if(DSCasts::ToUnsignedInt(productIdTextCtrl->GetValue()) == 0) {
			productIdTextCtrl->Clear();
			amountTextCtrl->SetValue(_N("1"));
		}
	}

	changed = false;

	SetSize(wxSize(480, 600));

	ICON();
}

// Events
void PurchasePositionEditDialog::OnClose(wxCloseEvent &event) {
	bool ok = true;
	if(changed) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to lose the changes?"),
			_("Lose changes"),
			_("Lose changes"),
			_("Cancel")
		);
		yesNoDialog->Center();
		ok = yesNoDialog->ShowModal() == wxID_YES;
		yesNoDialog->Destroy();
	}

	if(ok) {
		EndModal(wxID_CANCEL);
		event.Skip();
		Destroy();
	} else {
		event.Veto();
	}
}

void PurchasePositionEditDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void PurchasePositionEditDialog::FindProductEvent(wxMouseEvent &event) {
	FindProduct();
	SKIP();
}

void PurchasePositionEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void PurchasePositionEditDialog::RefreshSumEvent(wxCommandEvent &event) {
	RefreshSum();
}

void PurchasePositionEditDialog::EditProductIdEvent(wxCommandEvent &event) {
	EditProductId(DSCasts::ToUnsignedInt(productIdTextCtrl->GetValue()));
}

void PurchasePositionEditDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void PurchasePositionEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void PurchasePositionEditDialog::FindProductKeyEvent(wxCommandEvent &event) {
	FindProduct();
}

void PurchasePositionEditDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

// Methods
void PurchasePositionEditDialog::Close() {
	wxDialog::Close();
}

void PurchasePositionEditDialog::FindProduct() {
	ProductsFullFindDialog *productsFullFindDialog = new ProductsFullFindDialog(this, -1, rit, distributorId);
	productsFullFindDialog->Center();
	if(productsFullFindDialog->ShowModal() == wxID_OK) {
		ProductEntity *productEntity = productsFullFindDialog->GetProductEntity();
		if(productEntity)
			productIdTextCtrl->SetValue(productEntity->Get(_N("productId"))->ToString());
	}
}

bool PurchasePositionEditDialog::Save() {
	ProductController productController(rit);
	if(!productController.Exists(_N("productId"), DSCasts::ToString(DSCasts::ToUnsignedInt(productIdTextCtrl->GetValue())))) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The product id you entered does not exist."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return false;
	}

	if(!productEntity.Get(_N("available"))->ToBool()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The product is not available."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return false;
	}

	if(
		amountTextCtrl->GetCurrencyValue() == 0 ||
		priceTextCtrl->GetValue().IsEmpty()
	) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The following fields are necessary: amount (greater than zero), price"),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return false;
	}
	
	PurchasePositionEntity *purchasePositionEntity = static_cast<PurchasePositionEntity *>(bind.GetEntity());
	if(!purchasePositionEntity)
		return false;

	bind.Pull();
	DSCurrency price;
	price = purchasePositionEntity->Get(_N("amount"))->ToCurrency();
	purchasePositionEntity->SetValue(_N("amount"), price);
	price = purchasePositionEntity->Get(_N("price"))->ToCurrency();
	purchasePositionEntity->SetValue(_N("price"), price);

	purchasePositionEntity->SetValue(_N("articleNumber"), productEntity.Get(_N("articleNumber"))->ToString());
	purchasePositionEntity->SetValue(_N("group"), productEntity.Get(_N("group"))->ToString());
	purchasePositionEntity->SetValue(_N("package"), productEntity.Get(_N("package"))->ToString());
	purchasePositionEntity->SetValue(_N("available"), productEntity.Get(_N("available"))->ToString());
	purchasePositionEntity->SetValue(_N("distributorId"), productEntity.Get(_N("distributorId"))->ToString());
	purchasePositionEntity->SetValue(_N("manufacturer"), productEntity.Get(_N("manufacturer"))->ToString());
	purchasePositionEntity->SetValue(_N("publisher"), productEntity.Get(_N("publisher"))->ToString());
	purchasePositionEntity->SetValue(_N("ean"), productEntity.Get(_N("ean"))->ToString());
	purchasePositionEntity->SetValue(_N("shortName"), productEntity.Get(_N("shortName"))->ToString());
	purchasePositionEntity->SetValue(_N("name1"), productEntity.Get(_N("name1"))->ToString());
	purchasePositionEntity->SetValue(_N("name2"), productEntity.Get(_N("name2"))->ToString());
	purchasePositionEntity->SetValue(_N("name3"), productEntity.Get(_N("name3"))->ToString());
	purchasePositionEntity->SetValue(_N("name4"), productEntity.Get(_N("name4"))->ToString());
	purchasePositionEntity->SetValue(_N("description"), productEntity.Get(_N("description"))->ToString());
	purchasePositionEntity->SetValue(_N("note"), productEntity.Get(_N("note"))->ToString());
	purchasePositionEntity->SetValue(_N("isbn10"), productEntity.Get(_N("isbn10"))->ToString());
	purchasePositionEntity->SetValue(_N("isbn13"), productEntity.Get(_N("isbn13"))->ToString());
	purchasePositionEntity->SetValue(_N("weight"), productEntity.Get(_N("weight"))->ToDouble());

	price = productEntity.Get(_N("priceWithoutTax"))->ToCurrency();
	purchasePositionEntity->SetValue(_N("priceWithoutTax"), price);
	price = productEntity.Get(_N("priceWithTax"))->ToCurrency();
	purchasePositionEntity->SetValue(_N("priceWithTax"), price);
	price = productEntity.Get(_N("purchasePriceWithoutTax"))->ToCurrency();
	purchasePositionEntity->SetValue(_N("purchasePriceWithoutTax"), price);
	price = productEntity.Get(_N("purchasePriceWithTax"))->ToCurrency();
	purchasePositionEntity->SetValue(_N("purchasePriceWithTax"), price);

	changed = false;

	EndModal(wxID_OK);

	return true;
}

void PurchasePositionEditDialog::RefreshSum() {
	DSCurrency amount(DSCasts::ToDouble(amountTextCtrl->GetValue()));
	DSCurrency price(DSCasts::ToDouble(priceTextCtrl->GetValue()));
	DSCurrency fullPrice(amount * price);
	DSCurrency sum(fullPrice);

	sumStaticText->SetLabel(sum.ToString());
	mainBoxSizer->RecalcSizes();
}

void PurchasePositionEditDialog::EditProductId(const unsigned int productId) {
	ProductController productController(rit);
	productController.Get(DSCasts::ToString(productId));
	if(productController.Run()) {
		productController.Get(&productEntity);
		productController.Reset();

		if(productEntity.Get(_N("distributorId"))->ToUnsignedInt() != distributorId) {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The product is not from the selected distributor."),
				_("Wrong distributor")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();

			return;
		}

		if(!productEntity.Get(_N("available"))->ToBool()) {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The product is not available."),
				_("Product not available")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();

			return;
		}

		productInformationsTextCtrl->SetValue(
			_("Short name: ") + productEntity.Get(_N("shortName"))->ToString() + _N("\n") +
			_("Article number: ") + productEntity.Get(_N("articleNumber"))->ToString() + _N("\n") +
			_("Group: ") + productEntity.Get(_N("group"))->ToString() + _N("\n") +
			_("Package: ") + productEntity.Get(_N("package"))->ToString() + _N("\n") +
			_("Distributor: ") + productEntity.Get(_N("distributorId"))->ToString() + _N("\n") +
			_("Manufacturer: ") + productEntity.Get(_N("manufacturer"))->ToString() + _N("\n") +
			_("Publisher: ") + productEntity.Get(_N("publisher"))->ToString() + _N("\n") +
			_("Name 1: ") + productEntity.Get(_N("name1"))->ToString() + _N("\n") +
			_("Name 2: ") + productEntity.Get(_N("name2"))->ToString() + _N("\n") +
			_("Name 3: ") + productEntity.Get(_N("name3"))->ToString() + _N("\n") +
			_("Name 4: ") + productEntity.Get(_N("name4"))->ToString() + _N("\n") +
			_("ISBN 10: ") + productEntity.Get(_N("isbn10"))->ToString() + _N("\n") +
			_("ISBN 13: ") + productEntity.Get(_N("isbn13"))->ToString() + _N("\n") +
			_("Price without tax: ") + productEntity.Get(_N("priceWithoutTax"))->ToCurrency().ToString() + _N("\n") +
			_("Price with tax: ") + productEntity.Get(_N("priceWithTax"))->ToCurrency().ToString() + _N("\n") +
			_("Purchase price without tax: ") + productEntity.Get(_N("purchasePriceWithoutTax"))->ToCurrency().ToString() + _N("\n") +
			_("Purchase price with tax: ") + productEntity.Get(_N("purchasePriceWithTax"))->ToCurrency().ToString() + _N("\n") +
			_("Weight: ") + productEntity.Get(_N("weight"))->ToString() + _N("\n") +
			_("Description:\n") + productEntity.Get(_N("description"))->ToString() + _N("\n") +
			_("Note:\n") + productEntity.Get(_N("note"))->ToString()
		);

		priceTextCtrl->SetValue(productEntity.Get(
			_N("priceWithoutTax")// : _N("priceWithTax")
		)->ToCurrency().ToString());
	}
		
	RefreshSum();
}

void PurchasePositionEditDialog::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == textStaticText)
		textCtrl = textTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}
