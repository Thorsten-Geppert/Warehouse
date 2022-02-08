#include "OrderPositionEditDialog.h"
#include <DSCurrency.h>
#include "../lib/OrderController.h"
#include "../lib/CalculationLibrary.h"
#include "../lib/ProductController.h"
#include "../lib/CommonPreferencesController.h"
#include "../lib/StockController.h"
#include "../lib/StockBookController.h"
#include "ProductsFullFindDialog.h"
#include "TemplateSelectDialog.h"
#include "YesNoDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(OrderPositionEditDialog, wxDialog)
	EVT_CLOSE(OrderPositionEditDialog::OnClose)
	EVT_TEXT(XRCID("amountTextCtrl"), OrderPositionEditDialog::RefreshSumEvent)
	EVT_TEXT(XRCID("productIdTextCtrl"), OrderPositionEditDialog::EditProductIdEvent)
	EVT_TEXT(XRCID("priceTextCtrl"), OrderPositionEditDialog::RefreshSumEvent)
	EVT_TEXT(XRCID("reductionTextCtrl"), OrderPositionEditDialog::RefreshSumEvent)
	EVT_MENU(CLOSE_EVENT, OrderPositionEditDialog::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, OrderPositionEditDialog::SaveKeyEvent)
	EVT_MENU(FIND_PRODUCT_EVENT, OrderPositionEditDialog::FindProductKeyEvent)
	EVT_COMBOBOX(XRCID("stocksComboBox"), OrderPositionEditDialog::GetStockQuantityEvent)
END_EVENT_TABLE()

OrderPositionEditDialog::OrderPositionEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	OrderPositionEntity *orderPositionEntity,
	const unsigned int tax
) : bind(
	orderPositionEntity
), changed(
	false
), productEntity(
	rit
) {
	SetId(id);
	this->rit      = rit;
	this->tax      = tax;
	this->taxLevel = 0;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("OrderPositionEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("OrderPositionEditDialog"));

	mainPanel                   = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton           = XRCCTRL(*this, "saveToolbarButton",           ToolbarButton);
	closeToolbarButton          = XRCCTRL(*this, "closeToolbarButton",          ToolbarButton);
	findArticleToolbarButton    = XRCCTRL(*this, "findArticleToolbarButton",    ToolbarButton);
	stocksComboBox              = XRCCTRL(*this, "stocksComboBox",              DynSoft::DSComboBox);
	stockQuantityStaticText     = XRCCTRL(*this, "stockQuantityStaticText",     wxStaticText);
	amountTextCtrl              = XRCCTRL(*this, "amountTextCtrl",              DynSoft::DSTextCtrl);
	productIdTextCtrl           = XRCCTRL(*this, "productIdTextCtrl",           DynSoft::DSTextCtrl);
	priceTextCtrl               = XRCCTRL(*this, "priceTextCtrl",               DynSoft::DSTextCtrl);
	reductionTextCtrl           = XRCCTRL(*this, "reductionTextCtrl",           DynSoft::DSTextCtrl);
	sumStaticText               = XRCCTRL(*this, "sumStaticText",               wxStaticText);
	productInformationsTextCtrl = XRCCTRL(*this, "productInformationsTextCtrl", wxTextCtrl);
	textStaticText              = XRCCTRL(*this, "textStaticText",              wxStaticText);
	textTextCtrl                = XRCCTRL(*this, "textTextCtrl",                DynSoft::DSTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, OrderPositionEditDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(findArticleToolbarButton, OrderPositionEditDialog::FindProductEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, OrderPositionEditDialog::SaveEvent);

	textStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(OrderPositionEditDialog::TemplateSelectEvent), NULL, this);

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
	bind.Bind(_N("reduction"), reductionTextCtrl);
	bind.Bind(_N("text"),      textTextCtrl);
	bind.Bind(_N("stock"),     stocksComboBox, DSBind::VALUE);

	amountTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	productIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	priceTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	reductionTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);

	amountTextCtrl->SetChange(&changed);
	productIdTextCtrl->SetChange(&changed);
	priceTextCtrl->SetChange(&changed);
	reductionTextCtrl->SetChange(&changed);
	textTextCtrl->SetChange(&changed);
	stocksComboBox->SetChange(&changed);

	productIdTextCtrl->SetFocus();

	LoadStocks();

	if(orderPositionEntity) {
		DSCurrency price;
		price = orderPositionEntity->Get(_N("amount"))->ToCurrency();
		orderPositionEntity->SetValue(_N("amount"), price);
		price = orderPositionEntity->Get(_N("price"))->ToCurrency();
		orderPositionEntity->SetValue(_N("price"), price);
		price = orderPositionEntity->Get(_N("reduction"))->ToCurrency();
		orderPositionEntity->SetValue(_N("reduction"), price);
		bind.Push();

		if(DSCasts::ToUnsignedInt(productIdTextCtrl->GetValue()) == 0) {
			productIdTextCtrl->Clear();
			amountTextCtrl->SetValue(_N("1"));
		}
	}

	changed = false;

	GetStockQuantity();

	SetSize(wxSize(480, 600));

	ICON();
}

// Events
void OrderPositionEditDialog::OnClose(wxCloseEvent &event) {
	bool ok = true;
	if(changed) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to save the changes?"),
			_("Close"),
			_("Save"),
			_("Cancel")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES)
			ok = Save();
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

void OrderPositionEditDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void OrderPositionEditDialog::FindProductEvent(wxMouseEvent &event) {
	FindProduct();
	SKIP();
}

void OrderPositionEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void OrderPositionEditDialog::RefreshSumEvent(wxCommandEvent &event) {
	RefreshSum();
}

void OrderPositionEditDialog::EditProductIdEvent(wxCommandEvent &event) {
	EditProductId(DSCasts::ToUnsignedInt(productIdTextCtrl->GetValue()));
}

void OrderPositionEditDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void OrderPositionEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void OrderPositionEditDialog::FindProductKeyEvent(wxCommandEvent &event) {
	FindProduct();
}

void OrderPositionEditDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

void OrderPositionEditDialog::GetStockQuantityEvent(wxCommandEvent &event) {
	GetStockQuantity();
}

// Methods
void OrderPositionEditDialog::Close() {
	wxDialog::Close();
}

void OrderPositionEditDialog::FindProduct() {
	ProductsFullFindDialog *productsFullFindDialog = new ProductsFullFindDialog(this, -1, rit);
	productsFullFindDialog->Center();
	if(productsFullFindDialog->ShowModal() == wxID_OK) {
		ProductEntity *productEntity = productsFullFindDialog->GetProductEntity();
		if(productEntity)
			productIdTextCtrl->SetValue(productEntity->Get(_N("productId"))->ToString());
	}
}

bool OrderPositionEditDialog::Save() {
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
	
	OrderPositionEntity *orderPositionEntity = static_cast<OrderPositionEntity *>(bind.GetEntity());
	if(!orderPositionEntity)
		return false;

	bind.Pull();
	DSCurrency price;
	price = orderPositionEntity->Get(_N("amount"))->ToCurrency();
	orderPositionEntity->SetValue(_N("amount"), price);
	price = orderPositionEntity->Get(_N("price"))->ToCurrency();
	orderPositionEntity->SetValue(_N("price"), price);
	price = orderPositionEntity->Get(_N("reduction"))->ToCurrency();
	orderPositionEntity->SetValue(_N("reduction"), price);

	orderPositionEntity->SetValue(_N("taxLevel"), taxLevel);
	orderPositionEntity->SetValue(_N("taxPercent"), taxPercent);
	orderPositionEntity->SetValue(_N("articleNumber"), productEntity.Get(_N("articleNumber"))->ToString());
	orderPositionEntity->SetValue(_N("group"), productEntity.Get(_N("group"))->ToString());
	orderPositionEntity->SetValue(_N("package"), productEntity.Get(_N("package"))->ToString());
	orderPositionEntity->SetValue(_N("available"), productEntity.Get(_N("available"))->ToString());
	orderPositionEntity->SetValue(_N("distributorId"), productEntity.Get(_N("distributorId"))->ToString());
	orderPositionEntity->SetValue(_N("manufacturer"), productEntity.Get(_N("manufacturer"))->ToString());
	orderPositionEntity->SetValue(_N("publisher"), productEntity.Get(_N("publisher"))->ToString());
	orderPositionEntity->SetValue(_N("ean"), productEntity.Get(_N("ean"))->ToString());
	orderPositionEntity->SetValue(_N("shortName"), productEntity.Get(_N("shortName"))->ToString());
	orderPositionEntity->SetValue(_N("name1"), productEntity.Get(_N("name1"))->ToString());
	orderPositionEntity->SetValue(_N("name2"), productEntity.Get(_N("name2"))->ToString());
	orderPositionEntity->SetValue(_N("name3"), productEntity.Get(_N("name3"))->ToString());
	orderPositionEntity->SetValue(_N("name4"), productEntity.Get(_N("name4"))->ToString());
	orderPositionEntity->SetValue(_N("description"), productEntity.Get(_N("description"))->ToString());
	orderPositionEntity->SetValue(_N("note"), productEntity.Get(_N("note"))->ToString());
	orderPositionEntity->SetValue(_N("isbn10"), productEntity.Get(_N("isbn10"))->ToString());
	orderPositionEntity->SetValue(_N("isbn13"), productEntity.Get(_N("isbn13"))->ToString());
	orderPositionEntity->SetValue(_N("weight"), productEntity.Get(_N("weight"))->ToDouble());

	price = productEntity.Get(_N("priceWithoutTax"))->ToCurrency();
	orderPositionEntity->SetValue(_N("priceWithoutTax"), price);
	price = productEntity.Get(_N("priceWithTax"))->ToCurrency();
	orderPositionEntity->SetValue(_N("priceWithTax"), price);
	price = productEntity.Get(_N("purchasePriceWithoutTax"))->ToCurrency();
	orderPositionEntity->SetValue(_N("purchasePriceWithoutTax"), price);
	price = productEntity.Get(_N("purchasePriceWithTax"))->ToCurrency();
	orderPositionEntity->SetValue(_N("purchasePriceWithTax"), price);

	changed = false;

	EndModal(wxID_OK);

	return true;
}

void OrderPositionEditDialog::RefreshSum() {
	DSCurrency amount(DSCasts::ToDouble(amountTextCtrl->GetValue()));
	DSCurrency price(DSCasts::ToDouble(priceTextCtrl->GetValue()));
	DSCurrency fullPrice(amount * price);
	DSCurrency reduction(DSCasts::ToDouble(reductionTextCtrl->GetValue()));
	DSCurrency reducted(CalculationLibrary::Reduction(fullPrice, reduction));
	DSCurrency sum(fullPrice - reducted);

	sumStaticText->SetLabel(sum.ToString());
	mainBoxSizer->RecalcSizes();
}

void OrderPositionEditDialog::EditProductId(const unsigned int productId) {
	ProductController productController(rit);
	productController.Get(DSCasts::ToString(productId));
	if(productController.Run()) {
		productController.Get(&productEntity);
		productController.Reset();

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
			tax == 0 ? _N("priceWithoutTax") : _N("priceWithTax")
		)->ToCurrency().ToString());
		taxLevel = productEntity.Get(_N("tax"))->ToUnsignedInt();
		
		DSCurrency fullTaxPercent, reducedTaxPercent;
		CommonPreferencesController::GetTax(rit, &fullTaxPercent, &reducedTaxPercent);
		switch(taxLevel) {
			case 0:
				taxPercent = fullTaxPercent;
				break;
			case 1:
				taxPercent = reducedTaxPercent;
				break;
			case 2:
				taxPercent = 0.0;
				break;
		}
	}

	RefreshSum();
	GetStockQuantity();
}

void OrderPositionEditDialog::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == textStaticText)
		textCtrl = textTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}

void OrderPositionEditDialog::LoadStocks() {
	stocksComboBox->Append(wxEmptyString);
	stocksComboBox->SetSelection(0);
	
	StockController stockController(rit);
	wxArrayString stockNames(stockController.GetAllStockNames());
	const unsigned int count = stockNames.GetCount();
	for(unsigned int i = 0; i < count; i++)
		stocksComboBox->Append(stockNames[i]);
}

void OrderPositionEditDialog::GetStockQuantity() {
	const wxString stock(stocksComboBox->GetStringSelection());
	const unsigned int productId = DSCasts::ToUnsignedInt(productIdTextCtrl->GetValue());

	StockBookController stockBookController(rit);

	DSCurrency fullQuantity(stockBookController.GetQuantityByStockAndProduct(stock, productId));
	stockQuantityStaticText->SetLabel(_("Quantity: ") + fullQuantity.ToString() + _(" available"));
}
