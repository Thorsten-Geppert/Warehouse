#include "ProductEditFrame.h"
#include "../lib/ProductController.h"
#include "../lib/CommonPreferencesController.h"
#include "../lib/EnterprisePreferencesController.h"
#include "../lib/EnterprisePreferencesEntity.h"
#include "../lib/CalculationLibrary.h"
#include "../lib/ListEntryController.h"
#include "../lib/Tools.h"
#include "../lib/ReminderType.h"
#include "../lib/ClientEntity.h"
#include "../lib/ClientController.h"
#include "../lib/BlockType.h"
#include "../lib/KeyValueDatabaseLibrary.h"
#include "../lib/JasperReportsLibrary.h"
#include <DSCurrency.h>
#include "PrintPreviewDialog.h"
#include "TemplateSelectDialog.h"
#include "YesNoDialog.h"
#include "RemindersDialog.h"
#include "ClientsFullFindDialog.h"
#include "ClientEditFrame.h"
#include "ClientsModulePanel.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ProductEditFrame, wxFrame)
	EVT_CLOSE(ProductEditFrame::OnClose)
	EVT_MENU(CLOSE_EVENT, ProductEditFrame::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, ProductEditFrame::SaveKeyEvent)
	EVT_MENU(EDITED_INFORMATIONS_EVENT, ProductEditFrame::ShowEditedInformationsKeyEvent)
	EVT_MENU(PRINT_EVENT, ProductEditFrame::PrintKeyEvent)
	EVT_MENU(PRINT_PREVIEW_EVENT, ProductEditFrame::PrintPreviewKeyEvent)
	EVT_MENU(REMINDERS_EVENT, ProductEditFrame::RemindersKeyEvent)
	EVT_BUTTON(XRCID("calculateWithTaxButton"), ProductEditFrame::CalculateTaxEvent)
	EVT_BUTTON(XRCID("calculateWithoutTaxButton"), ProductEditFrame::CalculateTaxEvent)
	EVT_BUTTON(XRCID("calculatePurchasePriceWithTaxButton"), ProductEditFrame::CalculateTaxEvent)
	EVT_BUTTON(XRCID("calculatePurchasePriceWithoutTaxButton"), ProductEditFrame::CalculateTaxEvent)
	EVT_TEXT(XRCID("priceWithoutTaxTextCtrl"), ProductEditFrame::MarginEvent)
	EVT_TEXT(XRCID("priceWithTaxTextCtrl"), ProductEditFrame::MarginEvent)
	EVT_TEXT(XRCID("purchasePriceWithoutTaxTextCtrl"), ProductEditFrame::MarginEvent)
	EVT_TEXT(XRCID("purchasePriceWithTaxTextCtrl"), ProductEditFrame::MarginEvent)
	EVT_BUTTON(XRCID("distributorFindButton"), ProductEditFrame::DistributorFindEvent)
	EVT_BUTTON(XRCID("distributorOpenButton"), ProductEditFrame::DistributorOpenEvent)
	EVT_MENU(DISTRIBUTOR_OPEN_EVENT, ProductEditFrame::DistributorOpenEvent)
	EVT_MENU(DISTRIBUTOR_FIND_EVENT, ProductEditFrame::DistributorFindEvent)
END_EVENT_TABLE()

ProductEditFrame::ProductEditFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	ProductEntity *productEntity,
	GuiCallbackInterface *callbackInterface
) : bind(
	productEntity
), changed(
	false
) {
	SetId(id);
	this->rit = rit;
	this->callbackInterface = callbackInterface;
	productId = productEntity ? productEntity->Get(_N("productId"))->ToInt() : 0;
	mandatoryArticleNumber = false;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ProductEditFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("ProductEditFrame"));

	mainPanel                        = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                     = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton                = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	closeToolbarButton               = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	editedInformationsToolbarButton  = XRCCTRL(*this, "editedInformationsToolbarButton", ToolbarButton);
	printToolbarButton               = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	printPreviewToolbarButton        = XRCCTRL(*this, "printPreviewToolbarButton", ToolbarButton);
	remindersToolbarButton           = XRCCTRL(*this, "remindersToolbarButton", ToolbarButton);
	productIdTextCtrl                = XRCCTRL(*this, "productIdTextCtrl", DynSoft::DSTextCtrl);
	articleNumberStaticText          = XRCCTRL(*this, "articleNumberStaticText", wxStaticText);
	articleNumberTextCtrl            = XRCCTRL(*this, "articleNumberTextCtrl", DynSoft::DSTextCtrl);
	groupComboBox                    = XRCCTRL(*this, "groupComboBox", DynSoft::DSComboBox);
	packageComboBox                  = XRCCTRL(*this, "packageComboBox", DynSoft::DSComboBox);
	availableCheckBox                = XRCCTRL(*this, "availableCheckBox", DynSoft::DSCheckBox);
	distributorTextCtrl              = XRCCTRL(*this, "distributorTextCtrl", DynSoft::DSTextCtrl);
	distributorFindButton            = XRCCTRL(*this, "distributorFindButton", wxButton);
	distributorOpenButton            = XRCCTRL(*this, "distributorOpenButton", wxButton);
	manufacturerComboBox             = XRCCTRL(*this, "manufacturerComboBox", DynSoft::DSComboBox);
	publisherComboBox                = XRCCTRL(*this, "publisherComboBox", DynSoft::DSComboBox);
	eanTextCtrl                      = XRCCTRL(*this, "eanTextCtrl", DynSoft::DSTextCtrl);
	shortNameTextCtrl                = XRCCTRL(*this, "shortNameTextCtrl", DynSoft::DSTextCtrl);
	name1TextCtrl                    = XRCCTRL(*this, "name1TextCtrl", DynSoft::DSTextCtrl);
	name2TextCtrl                    = XRCCTRL(*this, "name2TextCtrl", DynSoft::DSTextCtrl);
	name3TextCtrl                    = XRCCTRL(*this, "name3TextCtrl", DynSoft::DSTextCtrl);
	name4TextCtrl                    = XRCCTRL(*this, "name4TextCtrl", DynSoft::DSTextCtrl);
	descriptionStaticText            = XRCCTRL(*this, "descriptionStaticText", wxStaticText);
	descriptionTextCtrl              = XRCCTRL(*this, "descriptionTextCtrl", DynSoft::DSTextCtrl);
	noteStaticText                   = XRCCTRL(*this, "noteStaticText", wxStaticText);
	noteTextCtrl                     = XRCCTRL(*this, "noteTextCtrl", DynSoft::DSTextCtrl);
	isbn10TextCtrl                   = XRCCTRL(*this, "isbn10TextCtrl", DynSoft::DSTextCtrl);
	isbn13TextCtrl                   = XRCCTRL(*this, "isbn13TextCtrl", DynSoft::DSTextCtrl);
	taxComboBox                      = XRCCTRL(*this, "taxComboBox", DynSoft::DSComboBox);
	priceWithoutTaxTextCtrl          = XRCCTRL(*this, "priceWithoutTaxTextCtrl", DynSoft::DSTextCtrl);
	priceWithTaxTextCtrl             = XRCCTRL(*this, "priceWithTaxTextCtrl", DynSoft::DSTextCtrl);
	purchasePriceWithoutTaxTextCtrl  = XRCCTRL(*this, "purchasePriceWithoutTaxTextCtrl", DynSoft::DSTextCtrl);
	purchasePriceWithTaxTextCtrl     = XRCCTRL(*this, "purchasePriceWithTaxTextCtrl", DynSoft::DSTextCtrl);
	marginWithoutTaxTextCtrl         = XRCCTRL(*this, "marginWithoutTaxTextCtrl", wxTextCtrl);
	marginWithTaxTextCtrl            = XRCCTRL(*this, "marginWithTaxTextCtrl", wxTextCtrl);
	weightTextCtrl                   = XRCCTRL(*this, "weightTextCtrl", DynSoft::DSTextCtrl);
	
	KeyValueDatabaseLibrary keyValueLibrary(rit, _N("advanced_preferences"));
	const wxString mediaDefault(keyValueLibrary.Get(_N("products.media.default_description")).ToString());
	wxPanel *tmpMediaPanel           = XRCCTRL(*this, "mediaPanel", wxPanel);
	mediaBoxSizer                    = static_cast<wxBoxSizer *>(tmpMediaPanel->GetSizer());
	mediaPanel                       = new MediaPanel(tmpMediaPanel, -1, rit, _N("linked_files_products"), productEntity ? productEntity->GetLinkedFileEntities() : NULL, &changed, mediaDefault);
	mediaBoxSizer->Add(mediaPanel, 1, wxEXPAND | wxALL, 5);

	wxPanel *tmpMoreDataPanel        = XRCCTRL(*this, "moreDataPanel", wxPanel);
	moreDataBoxSizer                 = static_cast<wxBoxSizer *>(tmpMoreDataPanel->GetSizer());
	moreDataPanel                    = new MoreDataPanel(tmpMoreDataPanel, -1, rit, productEntity->GetKeysValuesType(), &changed);
	moreDataBoxSizer->Add(moreDataPanel, 1, wxEXPAND | wxALL, 5);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, ProductEditFrame::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, ProductEditFrame::SaveEvent);
	CONNECT_TOOLBARBUTTON(editedInformationsToolbarButton, ProductEditFrame::ShowEditedInformationsEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, ProductEditFrame::PrintEvent);
	CONNECT_TOOLBARBUTTON(printPreviewToolbarButton, ProductEditFrame::PrintPreviewEvent);
	CONNECT_TOOLBARBUTTON(remindersToolbarButton, ProductEditFrame::RemindersEvent);

	descriptionStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ProductEditFrame::TemplateSelectEvent), NULL, this);
	noteStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ProductEditFrame::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[9];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,                WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,                  (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,                  (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,                  (int) 'i',  EDITED_INFORMATIONS_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,                  (int) 'p',  PRINT_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'p',  PRINT_PREVIEW_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'w',  REMINDERS_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'd',  DISTRIBUTOR_FIND_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'o',  DISTRIBUTOR_OPEN_EVENT);
	wxAcceleratorTable acceleratorTable(9, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling
	taxComboBox->Append(_("normal"));
	taxComboBox->Append(_("reduced"));
	taxComboBox->Append(_("without"));
	taxComboBox->SetSelection(0);

	ListEntryController::FillComboBox(rit, groupComboBox,        _N("product_groups"),   _N("group"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, packageComboBox,      _N("product_packages"), _N("package"),      ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, manufacturerComboBox, _N("manufacturers"),    _N("manufacturer"), ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, publisherComboBox,    _N("publishers"),       _N("publisher"),    ListEntryController::FILL_NEW, true);

	// Binding
	bind.Bind(_N("productId"),               productIdTextCtrl);
	bind.Bind(_N("articleNumber"),           articleNumberTextCtrl);
	bind.Bind(_N("group"),                   groupComboBox);
	bind.Bind(_N("package"),                 packageComboBox);
	bind.Bind(_N("available"),               availableCheckBox);
	bind.Bind(_N("distributorId"),           distributorTextCtrl);
	bind.Bind(_N("manufacturer"),            manufacturerComboBox);
	bind.Bind(_N("publisher"),               publisherComboBox);
	bind.Bind(_N("ean"),                     eanTextCtrl);
	bind.Bind(_N("shortName"),               shortNameTextCtrl);
	bind.Bind(_N("name1"),                   name1TextCtrl);
	bind.Bind(_N("name2"),                   name2TextCtrl);
	bind.Bind(_N("name3"),                   name3TextCtrl);
	bind.Bind(_N("name4"),                   name4TextCtrl);
	bind.Bind(_N("description"),             descriptionTextCtrl);
	bind.Bind(_N("note"),                    noteTextCtrl);
	bind.Bind(_N("isbn10"),                  isbn10TextCtrl);
	bind.Bind(_N("isbn13"),                  isbn13TextCtrl);
	bind.Bind(_N("tax"),                     taxComboBox, DSBind::SELECTION);
	bind.Bind(_N("priceWithoutTax"),         priceWithoutTaxTextCtrl);
	bind.Bind(_N("priceWithTax"),            priceWithTaxTextCtrl);
	bind.Bind(_N("purchasePriceWithoutTax"), purchasePriceWithoutTaxTextCtrl);
	bind.Bind(_N("purchasePriceWithTax"),    purchasePriceWithTaxTextCtrl);
	bind.Bind(_N("weight"),                  weightTextCtrl);

	if(productEntity) {
		rit->Block(_N("products"), productId, this);
		DSCurrency price;
		price = productEntity->Get(_N("priceWithoutTax"))->ToCurrency();
		productEntity->SetValue(_N("priceWithoutTax"), price);
		price = productEntity->Get(_N("priceWithTax"))->ToCurrency();
		productEntity->SetValue(_N("priceWithTax"), price);
		price = productEntity->Get(_N("purchasePriceWithoutTax"))->ToCurrency();
		productEntity->SetValue(_N("purchasePriceWithoutTax"), price);
		price = productEntity->Get(_N("purchasePriceWithTax"))->ToCurrency();
		productEntity->SetValue(_N("purchasePriceWithTax"), price);
		bind.Push();
		wxString weight(productEntity->Get(_N("weight"))->ToString());
		weight.Replace(_N("."), _N(","));
		weightTextCtrl->SetValue(Tools::RemoveLastZeros(weight));
		
		if(productId == 0)
			productIdTextCtrl->SetValue(_("<automatic>"));
		if(productEntity->Get(_N("distributorId"))->ToUnsignedInt() == 0)
			distributorTextCtrl->Clear();
	}

	SetSize(wxSize(450, 350));

	productIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	distributorTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	priceWithoutTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	priceWithTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	purchasePriceWithoutTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	purchasePriceWithTaxTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	weightTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_DOUBLE);

	productIdTextCtrl->SetChange(&changed);
	articleNumberTextCtrl->SetChange(&changed);
	groupComboBox->SetChange(&changed);
	packageComboBox->SetChange(&changed);
	availableCheckBox->SetChange(&changed);
	distributorTextCtrl->SetChange(&changed);
	manufacturerComboBox->SetChange(&changed);
	publisherComboBox->SetChange(&changed);
	eanTextCtrl->SetChange(&changed);
	shortNameTextCtrl->SetChange(&changed);
	name1TextCtrl->SetChange(&changed);
	name2TextCtrl->SetChange(&changed);
	name3TextCtrl->SetChange(&changed);
	name4TextCtrl->SetChange(&changed);
	descriptionTextCtrl->SetChange(&changed);
	noteTextCtrl->SetChange(&changed);
	isbn10TextCtrl->SetChange(&changed);
	isbn13TextCtrl->SetChange(&changed);
	taxComboBox->SetChange(&changed);
	priceWithoutTaxTextCtrl->SetChange(&changed);
	priceWithTaxTextCtrl->SetChange(&changed);
	purchasePriceWithoutTaxTextCtrl->SetChange(&changed);
	purchasePriceWithTaxTextCtrl->SetChange(&changed);
	weightTextCtrl->SetChange(&changed);

	this->editedInformationsDialog = new EditedInformationsDialog(this, -1, rit, productEntity);
	this->editedInformationsDialog->Show(false);

	shortNameTextCtrl->SetFocus();

	changed = false;

	Margin();

	mainBoxSizer->SetSizeHints(this);

	mediaBoxSizer->RecalcSizes();
	moreDataBoxSizer->RecalcSizes();

	//
	EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());
	EnterprisePreferencesController enterprisePreferencesController(rit->GetDatabase());
	enterprisePreferencesController.Get();
	if(enterprisePreferencesController.Run()) {
		enterprisePreferencesController.Get(&enterprisePreferencesEntity);
		enterprisePreferencesController.Reset();
		
		mandatoryArticleNumber = enterprisePreferencesEntity.Get(_N("mandatoryProductsArticleNumber"))->ToBool();
		if(mandatoryArticleNumber)
			articleNumberStaticText->SetForegroundColour(MANDATORY_FIELD_COLOUR);
	}

	ICON();
}

ProductEditFrame::~ProductEditFrame() {
	editedInformationsDialog->Destroy();
}

// Events
void ProductEditFrame::OnClose(wxCloseEvent &event) {
	bool ok = true;
	if(changed) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to save the changes?"),
			_("Close"),
			_("Save"),
			_("Close")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES)
			ok = Save();
		yesNoDialog->Destroy();
	}

	if(ok) {
		ProductEntity *productEntity = static_cast<ProductEntity *>(bind.GetEntity());
		delete productEntity;
		rit->Unblock(_N("products"), productId);

		Destroy();
		event.Skip();
	} else {
		event.Veto();
	}
}

void ProductEditFrame::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void ProductEditFrame::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void ProductEditFrame::PrintEvent(wxMouseEvent &event) {
	Print();
	SKIP();
}

void ProductEditFrame::RemindersEvent(wxMouseEvent &event) {
	Reminders();
	SKIP();
}

void ProductEditFrame::PrintPreviewEvent(wxMouseEvent &event) {
	Print(true);
	SKIP();
}

void ProductEditFrame::ShowEditedInformationsEvent(wxMouseEvent &event) {
	ShowEditedInformations();
	SKIP();
}

void ProductEditFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void ProductEditFrame::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void ProductEditFrame::PrintKeyEvent(wxCommandEvent &event) {
	Print();
}

void ProductEditFrame::RemindersKeyEvent(wxCommandEvent &event) {
	Reminders();
}

void ProductEditFrame::PrintPreviewKeyEvent(wxCommandEvent &event) {
	Print(true);
}

void ProductEditFrame::ShowEditedInformationsKeyEvent(wxCommandEvent &event) {
	ShowEditedInformations();
}

void ProductEditFrame::CalculateTaxEvent(wxCommandEvent &event) {
	CalculateTax(event.GetId());
}

void ProductEditFrame::MarginEvent(wxCommandEvent &event) {
	Margin();
}

void ProductEditFrame::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

void ProductEditFrame::DistributorFindEvent(wxCommandEvent &event) {
	DistributorFind();
}

void ProductEditFrame::DistributorOpenEvent(wxCommandEvent &event) {
	DistributorOpen();
}

// Methods
void ProductEditFrame::Close() {
	wxWindow::Close();
}

bool ProductEditFrame::Save() {
	productIdTextCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to save this product."), this))
		return false;

	if((mandatoryArticleNumber && articleNumberTextCtrl->GetValue().IsEmpty()) || name1TextCtrl->IsEmpty()) {
		rit->Log(true, _N("ProductEditFrame"), _("The product is not saved."), _("Not all necessary fiels are filled out."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill in the mandatory fields."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	ProductEntity *productEntity = static_cast<ProductEntity *>(bind.GetEntity());
	if(!productEntity) {
		rit->Log(true, _N("ProductEditFrame"), _("The product is not saved."), _("Null Pointer."));
		return false;
	}

	ClientController clientController(rit);
	const unsigned int distributorId = DSCasts::ToUnsignedInt(distributorTextCtrl->GetValue());
	if(distributorId > 0 && !clientController.IsDistributor(distributorId)) {
		rit->Log(true, _N("ProductEditFrame"), _("The product is not saved."), _("The distributor you entered is not a valid distributor or is inactive \"") + distributorTextCtrl->GetValue() + _N("\"."));

		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("The distributor you entered is not a valid distributor or is inactive."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	}

	bind.Pull();
	productEntity->SetValue(_N("productId"), productId);
	DSCurrency price;
	price = productEntity->Get(_N("priceWithoutTax"))->ToCurrency();
	productEntity->SetValue(_N("priceWithoutTax"), price);
	price = productEntity->Get(_N("priceWithTax"))->ToCurrency();
	productEntity->SetValue(_N("priceWithTax"), price);
	price = productEntity->Get(_N("purchasePriceWithoutTax"))->ToCurrency();
	productEntity->SetValue(_N("purchasePriceWithoutTax"), price);
	price = productEntity->Get(_N("purchasePriceWithTax"))->ToCurrency();
	productEntity->SetValue(_N("purchasePriceWithTax"), price);
	productEntity->SetValue(_N("weight"), productEntity->Get(_N("weight"))->ToDouble());

	ProductController productController(rit);
	bool ok = productController.Save(productEntity);

	if(ok) {
		productId = productEntity->Get(_N("productId"))->ToInt();
		productController.Get(DSCasts::ToString(productId)); // Autofelder refreshen
		productController.Run();
		productController.Get(productEntity);
		productController.Reset();
		
		price = productEntity->Get(_N("priceWithoutTax"))->ToCurrency();
		productEntity->SetValue(_N("priceWithoutTax"), price);
		price = productEntity->Get(_N("priceWithTax"))->ToCurrency();
		productEntity->SetValue(_N("priceWithTax"), price);
		price = productEntity->Get(_N("purchasePriceWithoutTax"))->ToCurrency();
		productEntity->SetValue(_N("purchasePriceWithoutTax"), price);
		price = productEntity->Get(_N("purchasePriceWithTax"))->ToCurrency();
		productEntity->SetValue(_N("purchasePriceWithTax"), price);

		bind.Push();

		if(callbackInterface)
			callbackInterface->Callback(static_cast<void *>(productEntity));

		rit->Block(_N("products"), productId, this);

		rit->Log(false, _N("ProductEditFrame"), _("The product is saved."), _("The product with product id \"") + DSCasts::ToString(productId) +  _("\" is saved."));

		editedInformationsDialog->Update();

		if(productEntity->Get(_N("distributorId"))->ToUnsignedInt() == 0)
			distributorTextCtrl->Clear();

		changed = false;
	} else {
		rit->Log(true, _N("ProductEditFrame"), _("The product is not saved."), _("Database error."));

		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not save."),
			_("Save product")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}

	return ok;
}

void ProductEditFrame::ShowEditedInformations() {
	editedInformationsDialog->Get(!editedInformationsDialog->IsShown());
}

void ProductEditFrame::Print(const bool preview) {
	productIdTextCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to print."), this))
		return;

	if(changed) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Save first this entry and press print again. Only saved entries can be printed."),
			_("Print")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	if(!JasperReportsLibrary::DoReport(
		this,
		rit,
		_N("products"),
		productId,
		preview ? JasperReportsController::SHOW : JasperReportsController::PRINT
	)) {
		ProductController productController(rit);
		HtmlPrintoutLibrary *htmlPrintoutLibrary = productController.Print(static_cast<ProductEntity *>(bind.GetEntity()));

		if(preview) {
			PrintPreviewDialog *printPreviewDialog = new PrintPreviewDialog(this, -1, rit, htmlPrintoutLibrary);
			printPreviewDialog->Center();
			printPreviewDialog->ShowModal();
			printPreviewDialog->Destroy();
		} else {
			wxPrinter printer;
			printer.Print(this, htmlPrintoutLibrary, true);
		}

		delete htmlPrintoutLibrary;
	}
}

void ProductEditFrame::CalculateTax(const int id) {
	DSTextCtrl *from = NULL;
	DSTextCtrl *to   = NULL;

	bool addTax = true;

	if(id == XRCID("calculateWithTaxButton")) {
		from   = priceWithoutTaxTextCtrl;
		to     = priceWithTaxTextCtrl;
	} else if(id == XRCID("calculateWithoutTaxButton")) {
		from   = priceWithTaxTextCtrl;
		to     = priceWithoutTaxTextCtrl;
		addTax = false;
	} else if(id == XRCID("calculatePurchasePriceWithTaxButton")) {
		from   = purchasePriceWithoutTaxTextCtrl;
		to     = purchasePriceWithTaxTextCtrl;
	} else if(id == XRCID("calculatePurchasePriceWithoutTaxButton")) {
		from   = purchasePriceWithTaxTextCtrl;
		to     = purchasePriceWithoutTaxTextCtrl;
		addTax = false;
	}

	const int selection = taxComboBox->GetSelection();

	wxString tmpPrice(from->GetValue());
	DSCurrency tax, fullTax, reducedTax, price(DSCasts::ToDouble(tmpPrice));
	CommonPreferencesController::GetTax(rit, &reducedTax, &fullTax);
	tax = selection == 0 ? reducedTax : fullTax;

	if(selection != 2)
		price = !addTax ? CalculationLibrary::SubTax(price, tax) : CalculationLibrary::AddTax(price, tax);
	
	to->SetValue(price.ToString());
	to->SetFocus();
}

void ProductEditFrame::Margin() {
	DSCurrency priceWithoutTax(priceWithoutTaxTextCtrl->GetCurrencyValue() - purchasePriceWithoutTaxTextCtrl->GetCurrencyValue());
	DSCurrency priceWithTax(priceWithTaxTextCtrl->GetCurrencyValue() - purchasePriceWithTaxTextCtrl->GetCurrencyValue());

	marginWithoutTaxTextCtrl->SetValue(priceWithoutTax.ToString());
	marginWithTaxTextCtrl->SetValue(priceWithTax.ToString());

	marginWithoutTaxTextCtrl->SetBackgroundColour(priceWithoutTax < 0 ? *wxRED : *wxGREEN);
	marginWithTaxTextCtrl->SetBackgroundColour(priceWithTax < 0 ? *wxRED : *wxGREEN);
}

void ProductEditFrame::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == descriptionStaticText)
		textCtrl = descriptionTextCtrl;
	else if(object == noteStaticText)
		textCtrl = noteTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}

void ProductEditFrame::Reminders() {
	if(!rit->Access(A_READ, _("You have no permission to open reminders."), this))
		return;

	if(productId == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please save first."),
			_("Reminders")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	RemindersDialog *remindersDialog = new RemindersDialog(
		this,
		-1,
		rit,
		ReminderType::PRODUCTS,
		productId
	);
	remindersDialog->Center();
	remindersDialog->ShowModal();
	remindersDialog->Destroy();
}

void ProductEditFrame::DistributorFind() {
	ClientsFullFindDialog *clientsFullFindDialog = new ClientsFullFindDialog(this, -1, rit, ClientsFullFindDialog::KIND_DISTRIBUTOR);
	clientsFullFindDialog->Center();
	if(clientsFullFindDialog->ShowModal() == wxID_OK) {
		ClientEntity *clientEntity = clientsFullFindDialog->GetClientEntity();
		if(clientEntity)
			distributorTextCtrl->SetValue(clientEntity->Get(_N("clientId"))->ToString());
	}
}

void ProductEditFrame::DistributorOpen() {
	if(!rit->Access(A_READ, _("You have no permission to open a client."), this))
		return;

	const wxString distributor(distributorTextCtrl->GetValue());
	BlockType *blockType = rit->IsBlocked(_N("clients"), DSCasts::ToUnsignedInt(distributor));
	if(blockType) {
		ClientEditFrame *clientEditFrame = static_cast<ClientEditFrame *>(blockType->GetData());
		if(clientEditFrame) {
			clientEditFrame->Raise();
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The client you want to edit is already open."),
				_("Edit client")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
		return;
	}

	ClientController clientController(rit);
	clientController.Get(distributor);
	if(clientController.Run()) {
		ClientEntity *clientEntity = new ClientEntity(rit);
		clientController.Get(clientEntity);
		clientController.Reset();

		ClientEditFrame *clientEditFrame = new ClientEditFrame(this, -1, rit, clientEntity, static_cast<ClientsModulePanel *>(rit->GetObject(_N("clientsModulePanel"))));
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
