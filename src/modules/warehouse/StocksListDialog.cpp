#include "StocksListDialog.h"
#include <wx/xrc/xmlres.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"
#include "ExportDialog.h"
#include "ExportProgressDialog.h"
#include "PrintPreviewDialog.h"
#include "../lib/StockBookController.h"
#include "../lib/StockBookEntity.h"
#include "../lib/StockBookEntities.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"

BEGIN_EVENT_TABLE(StocksListDialog, wxDialog)
	EVT_MENU(CLOSE_EVENT, StocksListDialog::CloseKeyEvent)
	EVT_MENU(EXPORT_EVENT, StocksListDialog::ExportKeyEvent)
	EVT_MENU(PRINT_EVENT, StocksListDialog::PrintKeyEvent)
	EVT_MENU(PREVIEW_EVENT, StocksListDialog::PreviewKeyEvent)
END_EVENT_TABLE()

StocksListDialog::StocksListDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const bool summarized
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("StocksListDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("StocksListDialog"));

	mainPanel            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar          = XRCCTRL(*this, "mainToolbar", Toolbar);
	printToolbarButton   = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	previewToolbarButton = XRCCTRL(*this, "previewToolbarButton", ToolbarButton);
	exportToolbarButton  = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);
	closeToolbarButton   = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	resultsListCtrl      = XRCCTRL(*this, "resultsListCtrl", DynSoft::DSListCtrl);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(800, 600));

	CONNECT_TOOLBARBUTTON(printToolbarButton, StocksListDialog::PrintEvent);
	CONNECT_TOOLBARBUTTON(previewToolbarButton, StocksListDialog::PreviewEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, StocksListDialog::ExportEvent);
	CONNECT_TOOLBARBUTTON(closeToolbarButton, StocksListDialog::CloseEvent);

	wxAcceleratorEntry acceleratorEntries[5];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'e',  EXPORT_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'p',  PRINT_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,    (int) 'v',  PREVIEW_EVENT);
	wxAcceleratorTable acceleratorTable(5, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();

	wxColour colour;
	DSCurrency quantity;
	StockBookController stockBookController(rit);
	StockBookEntities *stockBookEntities = NULL;
	StockBookEntity *stockBookEntity     = NULL;
	if(summarized) {
		resultsListCtrl->InsertColumn(0, _("Stock"), wxLIST_FORMAT_LEFT, 200);
		resultsListCtrl->InsertColumn(1, _("Product"), wxLIST_FORMAT_RIGHT, 180);
		resultsListCtrl->InsertColumn(2, _("Quantity"), wxLIST_FORMAT_RIGHT, 120);

		stockBookEntities = stockBookController.GetSummarized(false);
		const unsigned int count = stockBookEntities->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			stockBookEntity = stockBookEntities->Get(i);
			if(stockBookEntity) {
				quantity = stockBookEntity->Get(_N("quantity"))->ToCurrency();

				DSListCtrl::DSListCtrlRow *resultsListCtrlRow = new DSListCtrl::DSListCtrlRow(stockBookEntity);
				if(quantity == 0)     colour = *wxCYAN;
				else if(quantity > 0) colour = *wxGREEN;
				else                  colour = *wxRED;

				resultsListCtrlRow->SetBackgroundColour(colour);

				resultsListCtrlRow->Add(stockBookEntity->Get(_N("stock"))->ToString());
				resultsListCtrlRow->Add(stockBookEntity->Get(_N("product"))->ToString());
				resultsListCtrlRow->Add(quantity.ToString());

				resultsListCtrl->Append(resultsListCtrlRow, true);
			}
		}
	} else {
		resultsListCtrl->InsertColumn(0, _("Booked"), wxLIST_FORMAT_LEFT, 180);
		resultsListCtrl->InsertColumn(1, _("Stock"), wxLIST_FORMAT_LEFT, 200);
		resultsListCtrl->InsertColumn(2, _("Product"), wxLIST_FORMAT_RIGHT, 180);
		resultsListCtrl->InsertColumn(3, _("Quantity"), wxLIST_FORMAT_RIGHT, 120);
		resultsListCtrl->InsertColumn(4, _("Comment"), wxLIST_FORMAT_LEFT, 200);
		resultsListCtrl->InsertColumn(5, _("User"), wxLIST_FORMAT_LEFT, 130);

		stockBookEntities = stockBookController.GetSeparately(false);
		const unsigned int count = stockBookEntities->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			stockBookEntity = stockBookEntities->Get(i);
			if(stockBookEntity) {
				quantity = stockBookEntity->Get(_N("quantity"))->ToCurrency();

				DSListCtrl::DSListCtrlRow *resultsListCtrlRow = new DSListCtrl::DSListCtrlRow(stockBookEntity);
				if(quantity == 0)     colour = *wxCYAN;
				else if(quantity > 0) colour = *wxGREEN;
				else                  colour = *wxRED;

				resultsListCtrlRow->SetBackgroundColour(colour);

				resultsListCtrlRow->Add(stockBookEntity->Get(_N("created"))->ToString());
				resultsListCtrlRow->Add(stockBookEntity->Get(_N("stock"))->ToString());
				resultsListCtrlRow->Add(stockBookEntity->Get(_N("product"))->ToString());
				resultsListCtrlRow->Add(quantity.ToString());
				resultsListCtrlRow->Add(stockBookEntity->Get(_N("comment"))->ToString());
				resultsListCtrlRow->Add(stockBookEntity->Get(_N("username"))->ToString());

				resultsListCtrl->Append(resultsListCtrlRow, true);
			}
		}
	}

	delete stockBookEntities;
}

StocksListDialog::~StocksListDialog() {
	CleanUp();
}

void StocksListDialog::CleanUp() {
	resultsListCtrl->CleanUp();
}

void StocksListDialog::PrintEvent(wxMouseEvent &event) {
	Print(false);
	SKIP();
}

void StocksListDialog::PrintKeyEvent(wxCommandEvent &event) {
	Print(false);
}

void StocksListDialog::PreviewEvent(wxMouseEvent &event) {
	Print(true);
	SKIP();
}

void StocksListDialog::PreviewKeyEvent(wxCommandEvent &event) {
	Print(true);
}

void StocksListDialog::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void StocksListDialog::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void StocksListDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void StocksListDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void StocksListDialog::Print(const bool preview) {
	resultsListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to print."), this))
		return;

	StockBookController stockBookController(rit);
	HtmlPrintoutLibrary *htmlPrintoutLibrary = stockBookController.PrintList(resultsListCtrl);

	if(preview) {
		PrintPreviewDialog *printPreviewDialog = new PrintPreviewDialog(this, -1, rit, htmlPrintoutLibrary);
		printPreviewDialog->Center();
		printPreviewDialog->ShowModal();
		printPreviewDialog->Destroy();
	} else {
		wxPrintData printData;

		wxPrintDialogData printDialogData(printData);
		
		wxPrinter printer(&printDialogData);
		printer.Print(this, htmlPrintoutLibrary, true);
	}

	delete htmlPrintoutLibrary;
}

void StocksListDialog::Export() {
	resultsListCtrl->SetFocus();

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

			const unsigned int count = resultsListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, false);

			for(unsigned int i = 0; i < count; i++)
				exportLibrary.ExportAdd(static_cast<DSEntity *>(resultsListCtrl->GetPointer(i, 0)));

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

void StocksListDialog::Close() {
	wxDialog::Close();
}
