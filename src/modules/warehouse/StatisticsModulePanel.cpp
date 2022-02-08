#include "StatisticsModulePanel.h"
#include <wx/xrc/xmlres.h>
#include "ClientsFullFindDialog.h"
#include "ProductsFullFindDialog.h"
#include "PrintPreviewDialog.h"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"

#include "../lib/PermissionLibrary.h"
#include "../lib/OrderController.h"
#include "../lib/ClientController.h"
#include "../lib/ClientEntity.h"
#include "../lib/VoucherController.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"
#include "../lib/KeyValueDatabaseLibrary.h"

BEGIN_EVENT_TABLE(StatisticsModulePanel, ModulePanel)
	EVT_MENU(RUN_EVENT, StatisticsModulePanel::RunKeyEvent)
	EVT_MENU(FIND_PRODUCT_EVENT, StatisticsModulePanel::FindProductKeyEvent)
	EVT_MENU(FIND_CLIENT_EVENT, StatisticsModulePanel::FindClientKeyEvent)
	EVT_MENU(PRINT_EVENT, StatisticsModulePanel::PrintKeyEvent)
	EVT_MENU(PRINT_PREVIEW_EVENT, StatisticsModulePanel::PrintPreviewKeyEvent)
	EVT_MENU(EXPORT_EVENT, StatisticsModulePanel::ExportKeyEvent)
	EVT_TEXT(XRCID("clientIdTextCtrl"), StatisticsModulePanel::InputClientEvent)
END_EVENT_TABLE()

StatisticsModulePanel::StatisticsModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("StatisticsModulePanel")
) {
	// Toolbar
	runToolbarButton          = XRCCTRL(*this, "runToolbarButton", ToolbarButton);
	findProductToolbarButton  = XRCCTRL(*this, "findProductToolbarButton", ToolbarButton);
	findClientToolbarButton   = XRCCTRL(*this, "findClientToolbarButton", ToolbarButton);
	printToolbarButton        = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	printPreviewToolbarButton = XRCCTRL(*this, "printPreviewToolbarButton", ToolbarButton);
	exportToolbarButton       = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);
	productIdTextCtrl         = XRCCTRL(*this, "productIdTextCtrl", DynSoft::DSTextCtrl);
	clientIdTextCtrl          = XRCCTRL(*this, "clientIdTextCtrl", DynSoft::DSTextCtrl);
	ownClientIdTextCtrl       = XRCCTRL(*this, "ownClientIdTextCtrl", DynSoft::DSTextCtrl);
	dateFromDatePickerCtrl    = XRCCTRL(*this, "dateFromDatePickerCtrl", wxDatePickerCtrl);
	dateToDatePickerCtrl      = XRCCTRL(*this, "dateToDatePickerCtrl", wxDatePickerCtrl);
	selectionComboBox         = XRCCTRL(*this, "selectionComboBox", wxComboBox);

	productIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	clientIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	// Statistics
	statisticsListCtrl = XRCCTRL(*this, "statisticsListCtrl", DynSoft::DSListCtrl);

	statisticsListCtrl->InsertColumn(0, _("Date"), wxLIST_FORMAT_LEFT, 90);
	statisticsListCtrl->InsertColumn(1, _("Client id"), wxLIST_FORMAT_RIGHT, 120);
	statisticsListCtrl->InsertColumn(2, _("Order id"), wxLIST_FORMAT_RIGHT, 120);
	statisticsListCtrl->InsertColumn(3, _("Invoice id"), wxLIST_FORMAT_RIGHT, 120);
	statisticsListCtrl->InsertColumn(4, _("Product id"), wxLIST_FORMAT_RIGHT, 150);
	statisticsListCtrl->InsertColumn(5, _("Price (without tax)"), wxLIST_FORMAT_RIGHT, 150);
	statisticsListCtrl->InsertColumn(6, _("Full tax"), wxLIST_FORMAT_RIGHT, 150);
	statisticsListCtrl->InsertColumn(7, _("Reduced tax"), wxLIST_FORMAT_RIGHT, 150);
	statisticsListCtrl->InsertColumn(8, _("Price (with tax)"), wxLIST_FORMAT_RIGHT, 150);

	// Events
	CONNECT_TOOLBARBUTTON(runToolbarButton, StatisticsModulePanel::RunEvent);
	CONNECT_TOOLBARBUTTON(findClientToolbarButton, StatisticsModulePanel::FindClientEvent);
	CONNECT_TOOLBARBUTTON(findProductToolbarButton, StatisticsModulePanel::FindProductEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, StatisticsModulePanel::PrintEvent);
	CONNECT_TOOLBARBUTTON(printPreviewToolbarButton, StatisticsModulePanel::PrintPreviewEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, StatisticsModulePanel::ExportEvent);

	wxAcceleratorEntry acceleratorEntries[6];
	acceleratorEntries[0].Set(wxACCEL_CTRL,               (int) 'r',  RUN_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,               (int) 'f',  FIND_PRODUCT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,               (int) 'g',  FIND_CLIENT_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,               (int) 'p',  PRINT_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,               (int) 'k',  EXPORT_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL | wxACCEL_ALT, (int) 'p',  PRINT_PREVIEW_EVENT);
	wxAcceleratorTable acceleratorTable(6, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	KeyValueDatabaseLibrary keyValueDatabaseLibrary(rit, _N("advanced_preferences"));
	const int year = keyValueDatabaseLibrary.Get(_N("global.year")).ToInt();

	wxDateTime now(wxDateTime::Now());
	wxDateTime fromDateTime(now);
	wxDateTime toDateTime(now);
	if(year > 0) {
		fromDateTime.SetYear(year);
		toDateTime.SetYear(year);
	}
	fromDateTime.SetDay(1);
	dateFromDatePickerCtrl->SetValue(fromDateTime);
	dateToDatePickerCtrl->SetValue(toDateTime);

	ownClientIdTextCtrl->Disable();
}

StatisticsModulePanel::~StatisticsModulePanel() {
	ClearStatisticsListCtrl();
}

void StatisticsModulePanel::Load() {
	productIdTextCtrl->SetFocus();
}

void StatisticsModulePanel::Unload() {
}

void StatisticsModulePanel::RunEvent(wxMouseEvent &event) {
	Run();
	SKIP();
}

void StatisticsModulePanel::FindProductEvent(wxMouseEvent &event) {
	FindProduct();
	SKIP();
}

void StatisticsModulePanel::FindClientEvent(wxMouseEvent &event) {
	FindClient();
	SKIP();
}

void StatisticsModulePanel::PrintEvent(wxMouseEvent &event) {
	Print();
	SKIP();
}

void StatisticsModulePanel::PrintPreviewEvent(wxMouseEvent &event) {
	Print(true);
	SKIP();
}

void StatisticsModulePanel::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void StatisticsModulePanel::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void StatisticsModulePanel::RunKeyEvent(wxCommandEvent &event) {
	Run();
}

void StatisticsModulePanel::FindProductKeyEvent(wxCommandEvent &event) {
	FindProduct();
}

void StatisticsModulePanel::FindClientKeyEvent(wxCommandEvent &event) {
	FindClient();
}

void StatisticsModulePanel::PrintKeyEvent(wxCommandEvent &event) {
	Print();
}

void StatisticsModulePanel::PrintPreviewKeyEvent(wxCommandEvent &event) {
	Print(true);
}

void StatisticsModulePanel::InputClientEvent(wxCommandEvent &event) {
	InputClient();
}

void StatisticsModulePanel::Run() {
	ClearStatisticsListCtrl();

	DSCurrency sumBeforeTax;
	DSCurrency sumFullTax;
	DSCurrency sumReducedTax;
	DSCurrency sumAfterTax;

	const int selection = selectionComboBox->GetSelection();

	// Bills
	if(selection == 0 || selection == 1) {
		DSCurrency ordersSumBeforeTax;
		DSCurrency ordersSumFullTax;
		DSCurrency ordersSumReducedTax;
		DSCurrency ordersSumAfterTax;

		StatisticsEntity *statisticsEntity;
		OrderController orderController(rit);
		orderController.Statistics(
			DSCasts::ToUnsignedInt(productIdTextCtrl->GetValue()),
			DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue()),
			dateFromDatePickerCtrl->GetValue(),
			dateToDatePickerCtrl->GetValue()
		);
		while(orderController.Run()) {
			statisticsEntity = new StatisticsEntity;
			orderController.StatisticsGet(statisticsEntity);
			AddToStatisticsListCtrl(statisticsEntity);

			ordersSumBeforeTax  += statisticsEntity->Get(_N("beforeTax"))->ToCurrency();
			ordersSumFullTax    += statisticsEntity->Get(_N("fullTax"))->ToCurrency();
			ordersSumReducedTax += statisticsEntity->Get(_N("reducedTax"))->ToCurrency();
			ordersSumAfterTax   += statisticsEntity->Get(_N("afterTax"))->ToCurrency();
		}

		DSListCtrl::DSListCtrlRow *statisticsListCtrlRow = new DSListCtrl::DSListCtrlRow(new StatisticsEntity);

		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(_("Bill sums:"));
		statisticsListCtrlRow->Add(ordersSumBeforeTax.ToString());
		statisticsListCtrlRow->Add(ordersSumFullTax.ToString());
		statisticsListCtrlRow->Add(ordersSumReducedTax.ToString());
		statisticsListCtrlRow->Add(ordersSumAfterTax.ToString());

		statisticsListCtrl->Set(statisticsListCtrlRow, true);

		sumBeforeTax  += ordersSumBeforeTax;
		sumFullTax    += ordersSumFullTax;;
		sumReducedTax += ordersSumReducedTax;
		sumAfterTax   += ordersSumAfterTax;
	}

	if(selection == 0) {
		DSListCtrl::DSListCtrlRow *statisticsListCtrlRow = new DSListCtrl::DSListCtrlRow(new StatisticsEntity);

		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);

		statisticsListCtrl->Set(statisticsListCtrlRow, true);
	}

	// Vouchers
	if(selection == 0 || selection == 2) {
		DSCurrency vouchersSumBeforeTax;
		DSCurrency vouchersSumFullTax;
		DSCurrency vouchersSumReducedTax;
		DSCurrency vouchersSumAfterTax;

		StatisticsEntity *statisticsEntity;
		VoucherController voucherController(rit);
		voucherController.Statistics(
			dateFromDatePickerCtrl->GetValue(),
			dateToDatePickerCtrl->GetValue()
		);
		while(voucherController.Run()) {
			statisticsEntity = new StatisticsEntity;
			voucherController.StatisticsGet(statisticsEntity);
			AddToStatisticsListCtrl(statisticsEntity);

			vouchersSumBeforeTax  += statisticsEntity->Get(_N("beforeTax"))->ToCurrency();
			vouchersSumFullTax    += statisticsEntity->Get(_N("fullTax"))->ToCurrency();
			vouchersSumReducedTax += statisticsEntity->Get(_N("reducedTax"))->ToCurrency();
			vouchersSumAfterTax   += statisticsEntity->Get(_N("afterTax"))->ToCurrency();
		}

		DSListCtrl::DSListCtrlRow *statisticsListCtrlRow = new DSListCtrl::DSListCtrlRow(new StatisticsEntity);

		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(wxEmptyString);
		statisticsListCtrlRow->Add(_("Vouchers sums:"));
		statisticsListCtrlRow->Add(vouchersSumBeforeTax.ToString());
		statisticsListCtrlRow->Add(vouchersSumFullTax.ToString());
		statisticsListCtrlRow->Add(vouchersSumReducedTax.ToString());
		statisticsListCtrlRow->Add(vouchersSumAfterTax.ToString());

		statisticsListCtrl->Set(statisticsListCtrlRow, true);

		sumBeforeTax  += vouchersSumBeforeTax;
		sumFullTax    += vouchersSumFullTax;;
		sumReducedTax += vouchersSumReducedTax;
		sumAfterTax   += vouchersSumAfterTax;
	}

	DSListCtrl::DSListCtrlRow *statisticsListCtrlRow = new DSListCtrl::DSListCtrlRow(new StatisticsEntity);

	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);

	statisticsListCtrl->Set(statisticsListCtrlRow, true);

	statisticsListCtrlRow = new DSListCtrl::DSListCtrlRow(new StatisticsEntity);

	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(wxEmptyString);
	statisticsListCtrlRow->Add(_("Full sums:"));
	statisticsListCtrlRow->Add(sumBeforeTax.ToString());
	statisticsListCtrlRow->Add(sumFullTax.ToString());
	statisticsListCtrlRow->Add(sumReducedTax.ToString());
	statisticsListCtrlRow->Add(sumAfterTax.ToString());

	statisticsListCtrl->Set(statisticsListCtrlRow, true);

	statisticsListCtrl->SetFocus();
}

void StatisticsModulePanel::FindProduct() {
	ProductsFullFindDialog *productsFullFindDialog = new ProductsFullFindDialog(this, -1, rit);
	productsFullFindDialog->Center();
	if(productsFullFindDialog->ShowModal() == wxID_OK) {
		ProductEntity *productEntity = productsFullFindDialog->GetProductEntity();
		if(productEntity) {
			productIdTextCtrl->SetValue(productEntity->Get(_N("productId"))->ToString());
			delete productEntity;
		}
	}

	productIdTextCtrl->SetFocus();
}

void StatisticsModulePanel::FindClient() {
	ClientsFullFindDialog *clientsFullFindDialog = new ClientsFullFindDialog(this, -1, rit);
	clientsFullFindDialog->Center();
	if(clientsFullFindDialog->ShowModal() == wxID_OK) {
		ClientEntity *clientEntity = clientsFullFindDialog->GetClientEntity();
		if(clientEntity) {
			clientIdTextCtrl->SetValue(clientEntity->Get(_N("clientId"))->ToString());
			delete clientEntity;
		}
	}

	clientIdTextCtrl->SetFocus();
}

void StatisticsModulePanel::Print(const bool preview) {
	statisticsListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to print."), this))
		return;

	OrderController orderController(rit);
	HtmlPrintoutLibrary *htmlPrintoutLibrary = orderController.PrintStatistic(
		DSCasts::ToUnsignedInt(productIdTextCtrl->GetValue()),
		DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue()),
		dateFromDatePickerCtrl->GetValue(),
		dateToDatePickerCtrl->GetValue(),
		statisticsListCtrl
	);

	if(preview) {
		PrintPreviewDialog *printPreviewDialog = new PrintPreviewDialog(this, -1, rit, htmlPrintoutLibrary, wxLANDSCAPE);
		printPreviewDialog->Center();
		printPreviewDialog->ShowModal();
		printPreviewDialog->Destroy();
	} else {
		wxPrintData printData;
		printData.SetOrientation(wxLANDSCAPE);

		wxPrintDialogData printDialogData(printData);
		
		wxPrinter printer(&printDialogData);
		printer.Print(this, htmlPrintoutLibrary, true);
	}

	delete htmlPrintoutLibrary;
}

void StatisticsModulePanel::Export() {
	statisticsListCtrl->SetFocus();

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

			const unsigned int count = statisticsListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, false);

			for(unsigned int i = 0; i < count; i++)
				exportLibrary.ExportAdd(static_cast<DSEntity *>(statisticsListCtrl->GetPointer(i, 0)));

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

void StatisticsModulePanel::AddToStatisticsListCtrl(StatisticsEntity *statisticsEntity) {
	DSListCtrl::DSListCtrlRow *statisticsListCtrlRow = new DSListCtrl::DSListCtrlRow(statisticsEntity);

	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("invoiceDate"))->ToString());
	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("clientId"))->ToString());
	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("orderId"))->ToString());
	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("invoiceId"))->ToString());
	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("productId"))->ToString());
	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("beforeTax"))->ToString());
	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("fullTax"))->ToString());
	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("reducedTax"))->ToString());
	statisticsListCtrlRow->Add(statisticsEntity->Get(_N("afterTax"))->ToString());

	statisticsListCtrl->Set(statisticsListCtrlRow, true);
}

void StatisticsModulePanel::ClearStatisticsListCtrl() {
	statisticsListCtrl->CleanUp();
}

void StatisticsModulePanel::InputClient() {
	ClientController clientController(rit);
	clientController.Get(DSCasts::ToUnsignedInt(clientIdTextCtrl->GetValue()));
	if(clientController.Run()) {
		ClientEntity clientEntity(rit);
		clientController.Get(&clientEntity);
		clientController.Reset();

		ownClientIdTextCtrl->SetValue(clientEntity.Get(_N("ownClientId"))->ToString());
	}
}
