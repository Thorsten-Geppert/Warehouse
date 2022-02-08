#include "StocksFrame.h"
#include "Configuration.h"
#include "YesNoDialog.h"
#include "ProductsFullFindDialog.h"
#include "StocksListDialog.h"
#include "resources/images/WarehouseIcon.xpm"
#include "../lib/StockController.h"
#include "../lib/StockBookEntity.h"
#include "../lib/StockBookController.h"

BEGIN_EVENT_TABLE(StocksFrame, wxFrame)
	EVT_MENU(CLOSE_EVENT, StocksFrame::CloseKeyEvent)
	EVT_MENU(FIND_PRODUCT_EVENT, StocksFrame::FindProductKeyEvent)
	EVT_MENU(BOOK_EVENT, StocksFrame::BookKeyEvent)
	EVT_MENU(LIST_EVENT, StocksFrame::ListKeyEvent)
	EVT_MENU(LIST_SUMMARIZED_EVENT, StocksFrame::ListSummarizedEvent)
	EVT_MENU(LIST_SEPARATELY_EVENT, StocksFrame::ListSeparatelyEvent)
	EVT_CLOSE(StocksFrame::HideEvent)

	EVT_COMBOBOX(XRCID("stocksComboBox"), StocksFrame::GetSumEvent)
	EVT_TEXT(XRCID("productTextCtrl"), StocksFrame::GetSumEvent)
END_EVENT_TABLE()

StocksFrame::StocksFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("StocksFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("StocksFrame"));

	mainPanel                = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer             = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findProductToolbarButton = XRCCTRL(*this, "findProductToolbarButton", ToolbarButton);
	bookToolbarButton        = XRCCTRL(*this, "bookToolbarButton", ToolbarButton);
	listToolbarButton        = XRCCTRL(*this, "listToolbarButton", ToolbarButton);
	closeToolbarButton       = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	stocksComboBox           = XRCCTRL(*this, "stocksComboBox", DynSoft::DSComboBox);
	productTextCtrl          = XRCCTRL(*this, "productTextCtrl", DynSoft::DSTextCtrl);
	quantityTextCtrl         = XRCCTRL(*this, "quantityTextCtrl", DynSoft::DSTextCtrl);
	commentTextCtrl          = XRCCTRL(*this, "commentTextCtrl", DynSoft::DSTextCtrl);
	fullQuantityTextCtrl     = XRCCTRL(*this, "fullQuantityTextCtrl", wxTextCtrl);
	reservedTextCtrl         = XRCCTRL(*this, "reservedTextCtrl", wxTextCtrl);
	sumTextCtrl              = XRCCTRL(*this, "sumTextCtrl", wxTextCtrl);

	productTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	quantityTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);

	mainBoxSizer->SetSizeHints(this);
	SetMinSize(GetSize());
	SetMaxSize(GetSize());

	CONNECT_TOOLBARBUTTON(findProductToolbarButton, StocksFrame::FindProductEvent);
	CONNECT_TOOLBARBUTTON(bookToolbarButton, StocksFrame::BookEvent);
	CONNECT_TOOLBARBUTTON(listToolbarButton, StocksFrame::ListEvent);
	CONNECT_TOOLBARBUTTON(closeToolbarButton, StocksFrame::CloseEvent);

	wxAcceleratorEntry acceleratorEntries[5];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'b',  BOOK_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,   (int) 'l',  LIST_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,   (int) 'f',  FIND_PRODUCT_EVENT);
	wxAcceleratorTable acceleratorTable(5, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();

	stocksComboBox->SetFocus();

	GetSum();
}

StocksFrame::~StocksFrame() {
}

bool StocksFrame::Show(const bool show) {
	if(show) {
		ReloadStocks();
		GetSum();
	}
		
	return wxFrame::Show(show);
}

void StocksFrame::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void StocksFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void StocksFrame::FindProductEvent(wxMouseEvent &event) {
	FindProduct();
	SKIP();
}

void StocksFrame::FindProductKeyEvent(wxCommandEvent &event) {
	FindProduct();
}

void StocksFrame::BookEvent(wxMouseEvent &event) {
	Book();
	SKIP();
}

void StocksFrame::BookKeyEvent(wxCommandEvent &event) {
	Book();
}

void StocksFrame::ListEvent(wxMouseEvent &event) {
	List();
	SKIP();
}

void StocksFrame::ListKeyEvent(wxCommandEvent &event) {
	List();
}

void StocksFrame::HideEvent(wxCloseEvent &event) {
	Close();
}

void StocksFrame::GetSumEvent(wxCommandEvent &event) {
	GetSum();
}

void StocksFrame::Close() {
	Hide();
}

void StocksFrame::ListSummarizedEvent(wxCommandEvent &event) {
	List(LIST_SUMMARIZED_EVENT);
}

void StocksFrame::ListSeparatelyEvent(wxCommandEvent &event) {
	List(LIST_SEPARATELY_EVENT);
}

void StocksFrame::FindProduct() {
	ProductsFullFindDialog *productsFullFindDialog = new ProductsFullFindDialog(this, -1, rit);
	productsFullFindDialog->Center();
	if(productsFullFindDialog->ShowModal() == wxID_OK) {
		ProductEntity *productEntity = productsFullFindDialog->GetProductEntity();
		if(productEntity) {
			productTextCtrl->SetValue(productEntity->Get(_N("productId"))->ToString());
			delete productEntity;
		}
	}

	productTextCtrl->SetFocus();
}

void StocksFrame::Book() {
	if(!rit->Access(A_WRITE, _("You have no permission to book."), this))
		return;

	DSCurrency quantity = quantityTextCtrl->GetCurrencyValue();
	const wxString stock(stocksComboBox->GetStringSelection());
	const unsigned int productId = DSCasts::ToUnsignedInt(productTextCtrl->GetValue());

	if(stock.IsEmpty() || productId == 0 || quantity == 0) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the stock and the product you want to book. The quanity must be different to 0."),
			_("Error")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	StockBookEntity stockBookEntity(rit);
	stockBookEntity.SetValue(_N("stock"), stock);
	stockBookEntity.SetValue(_N("product"), productId);
	stockBookEntity.SetValue(_N("quantity"), quantity);
	stockBookEntity.SetValue(_N("comment"), commentTextCtrl->GetValue());
	stockBookEntity.SetValue(_N("username"), rit->GetUserConfigurationFileEntity()->GetLastUsername());

	bool ok = false;
	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you want to execute booking?"),
		_("Booking"),
		_("Execute"),
		_("Abort")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		StockBookController stockBookController(rit);
		ok = stockBookController.Insert(&stockBookEntity);
	}
	yesNoDialog->Destroy();

	if(ok) {
		rit->Log(false, _N("StocksFrame"), _("The booking was successful."), _("The booking \"") + stockBookEntity.ToString() + _("\" was successful."));

		quantityTextCtrl->Clear();
		commentTextCtrl->Clear();
		quantityTextCtrl->SetFocus();
	} else {
		rit->Log(true, _N("StocksFrame"), _("The booking was not successful."), _("The booking \"") + stockBookEntity.ToString() + _("\" was not successful."));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not execute booking. Maybe there is a database error or the product or the stock does not exist any more."),
			_("Error")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	GetSum();
}

void StocksFrame::List() {
	if(!rit->Access(A_READ, _("You have no permission to view bookings."), this))
		return;

	wxMenu popupMenu;
	popupMenu.Append(LIST_SUMMARIZED_EVENT, _("Summerized"));
	popupMenu.Append(LIST_SEPARATELY_EVENT, _("Separately"));
	PopupMenu(&popupMenu);
}

void StocksFrame::List(const int type) {
	if(!rit->Access(A_READ, _("You have no permission to view bookings."), this))
		return;
	
	StocksListDialog *stocksListDialog = new StocksListDialog(
		this,
		-1,
		rit,
		type == LIST_SUMMARIZED_EVENT
	);
	stocksListDialog->Center();
	stocksListDialog->ShowModal();
	stocksListDialog->Destroy();
}

void StocksFrame::ReloadStocks(wxString selectedName) {
	selectedName = selectedName.IsEmpty() ? stocksComboBox->GetStringSelection() : selectedName;
	stocksComboBox->Clear();
	stocksComboBox->Append(wxEmptyString);
	stocksComboBox->SetSelection(0);
	
	StockController stockController(rit);
	wxArrayString stockNames(stockController.GetAllStockNames());
	const unsigned int count = stockNames.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		stocksComboBox->Append(stockNames[i]);
		if(selectedName == stockNames[i])
			stocksComboBox->SetSelection(i + 1);
	}
}

void StocksFrame::GetSum() {
	StockBookController stockBookController(rit);

	DSCurrency fullQuantity(stockBookController.GetQuantityByStockAndProduct(
		stocksComboBox->GetStringSelection(),
		DSCasts::ToUnsignedInt(productTextCtrl->GetValue())
	));

	DSCurrency reserved(stockBookController.GetReservedByStockAndProduct(
		stocksComboBox->GetStringSelection(),
		DSCasts::ToUnsignedInt(productTextCtrl->GetValue())
	));

	DSCurrency sum(fullQuantity - reserved);

	fullQuantityTextCtrl->SetValue(fullQuantity.ToString());
	reservedTextCtrl->SetValue(reserved.ToString());
	sumTextCtrl->SetValue(sum.ToString());
}
