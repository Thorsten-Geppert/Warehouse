#include "PrintPreviewDialog.h"
#include <DSCasts.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(PrintPreviewDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, PrintPreviewDialog::CancelKeyEvent)
	EVT_MENU(FIRST_PAGE_EVENT, PrintPreviewDialog::FirstPageKeyEvent)
	EVT_MENU(PREVIOUS_PAGE_EVENT, PrintPreviewDialog::PreviousPageKeyEvent)
	EVT_MENU(NEXT_PAGE_EVENT, PrintPreviewDialog::NextPageKeyEvent)
	EVT_MENU(LAST_PAGE_EVENT, PrintPreviewDialog::LastPageKeyEvent)
	EVT_MENU(ZOOM_IN_EVENT, PrintPreviewDialog::ZoomInKeyEvent)
	EVT_MENU(ZOOM_NORMAL_EVENT, PrintPreviewDialog::ZoomNormalKeyEvent)
	EVT_MENU(ZOOM_OUT_EVENT, PrintPreviewDialog::ZoomOutKeyEvent)
	EVT_MENU(PRINT_EVENT, PrintPreviewDialog::PrintKeyEvent)
	EVT_TIMER(-1, PrintPreviewDialog::PageCountRefreshEvent)
END_EVENT_TABLE()

PrintPreviewDialog::PrintPreviewDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	HtmlPrintoutLibrary *htmlPrintoutLibrary,
	#if wxMAJOR_VERSION < 3
		const int orientation
	#else
		const wxPrintOrientation orientation
	#endif
) : pageCountRefreshTimer(
	this
) {
	SetId(id);
	this->rit = rit;
	this->htmlPrintoutLibrary = htmlPrintoutLibrary;
	this->printed = false;
	this->orientation = orientation;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("PrintPreviewDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("PrintPreviewDialog"));

	mainPanel                 = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer              = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	cancelToolbarButton       = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	firstPageToolbarButton    = XRCCTRL(*this, "firstPageToolbarButton", ToolbarButton);
	previousPageToolbarButton = XRCCTRL(*this, "previousPageToolbarButton", ToolbarButton);
	nextPageToolbarButton     = XRCCTRL(*this, "nextPageToolbarButton", ToolbarButton);
	lastPageToolbarButton     = XRCCTRL(*this, "lastPageToolbarButton", ToolbarButton);
	zoomInToolbarButton       = XRCCTRL(*this, "zoomInToolbarButton", ToolbarButton);
	zoomNormalToolbarButton   = XRCCTRL(*this, "zoomNormalToolbarButton", ToolbarButton);
	zoomOutToolbarButton      = XRCCTRL(*this, "zoomOutToolbarButton", ToolbarButton);
	printToolbarButton        = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	printPanel                = XRCCTRL(*this, "printPanel", wxPanel);
	mainStatusBar             = XRCCTRL(*this, "mainStatusBar", wxStatusBar);

	printBoxSizer             = static_cast<wxBoxSizer *>(printPanel->GetSizer());

	printData                 = new wxPrintData;
	printData->SetOrientation(orientation);
	printPreview              = new wxPrintPreview(htmlPrintoutLibrary, htmlPrintoutLibrary, printData);
	previewCanvas             = new wxPreviewCanvas(printPreview, printPanel, wxDefaultPosition, wxSize(800, 600));
	previewCanvas->Connect(wxEVT_MOUSEWHEEL, wxMouseEventHandler(PrintPreviewDialog::ZoomEvent), NULL, this);

	printPreview->SetCanvas(previewCanvas);
	printPreview->SetZoom(ZOOM_NORMAL);
	printBoxSizer->Add(previewCanvas, 1, wxEXPAND);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, PrintPreviewDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(firstPageToolbarButton, PrintPreviewDialog::FirstPageEvent);
	CONNECT_TOOLBARBUTTON(previousPageToolbarButton, PrintPreviewDialog::PreviousPageEvent);
	CONNECT_TOOLBARBUTTON(nextPageToolbarButton, PrintPreviewDialog::NextPageEvent);
	CONNECT_TOOLBARBUTTON(lastPageToolbarButton, PrintPreviewDialog::LastPageEvent);
	CONNECT_TOOLBARBUTTON(zoomInToolbarButton, PrintPreviewDialog::ZoomInEvent);
	CONNECT_TOOLBARBUTTON(zoomNormalToolbarButton, PrintPreviewDialog::ZoomNormalEvent);
	CONNECT_TOOLBARBUTTON(zoomOutToolbarButton, PrintPreviewDialog::ZoomOutEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, PrintPreviewDialog::PrintEvent);

	wxAcceleratorEntry acceleratorEntries[10];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'q',  FIRST_PAGE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,   (int) 'a',  PREVIOUS_PAGE_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,   (int) 'd',  NEXT_PAGE_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL,   (int) 'e',  LAST_PAGE_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL,   (int) '+',  ZOOM_IN_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL,   (int) '0',  ZOOM_NORMAL_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL,   (int) '-',  ZOOM_OUT_EVENT);
	acceleratorEntries[9].Set(wxACCEL_CTRL,   (int) 'p',  PRINT_EVENT);
	wxAcceleratorTable acceleratorTable(10, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(1000, 600));

	// Stuff
	minPage = printPreview->GetMinPage();
	maxPage = printPreview->GetMaxPage();
	curPage = printPreview->GetCurrentPage();
	zoom    = ZOOM_NORMAL;

	//RefreshStatusBar();

	ICON();

#if wxMAJOR_VERSION < 3
	pageCountRefreshTimer.Start(150, true);
#else
	pageCountRefreshTimer.StartOnce(150);
#endif
}

PrintPreviewDialog::~PrintPreviewDialog() {
	delete printData;
}

void PrintPreviewDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void PrintPreviewDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void PrintPreviewDialog::FirstPageEvent(wxMouseEvent &event) {
	FirstPage();
	SKIP();
}

void PrintPreviewDialog::FirstPageKeyEvent(wxCommandEvent &event) {
	FirstPage();
}

void PrintPreviewDialog::PreviousPageEvent(wxMouseEvent &event) {
	PreviousPage();
	SKIP();
}

void PrintPreviewDialog::PreviousPageKeyEvent(wxCommandEvent &event) {
	PreviousPage();
}

void PrintPreviewDialog::NextPageEvent(wxMouseEvent &event) {
	NextPage();
	SKIP();
}

void PrintPreviewDialog::NextPageKeyEvent(wxCommandEvent &event) {
	NextPage();
}

void PrintPreviewDialog::LastPageEvent(wxMouseEvent &event) {
	LastPage();
	SKIP();
}

void PrintPreviewDialog::LastPageKeyEvent(wxCommandEvent &event) {
	LastPage();
}

void PrintPreviewDialog::ZoomInEvent(wxMouseEvent &event) {
	ZoomIn();
	SKIP();
}

void PrintPreviewDialog::ZoomInKeyEvent(wxCommandEvent &event) {
	ZoomIn();
}

void PrintPreviewDialog::ZoomNormalEvent(wxMouseEvent &event) {
	ZoomNormal();
	SKIP();
}

void PrintPreviewDialog::ZoomNormalKeyEvent(wxCommandEvent &event) {
	ZoomNormal();
}

void PrintPreviewDialog::ZoomOutEvent(wxMouseEvent &event) {
	ZoomOut();
	SKIP();
}

void PrintPreviewDialog::ZoomOutKeyEvent(wxCommandEvent &event) {
	ZoomOut();
}

void PrintPreviewDialog::ZoomEvent(wxMouseEvent &event) {
	if(!event.ControlDown())
		event.Skip();
	else
		Zoom(event.GetWheelRotation(), event.ControlDown());
}

void PrintPreviewDialog::PrintEvent(wxMouseEvent &event) {
	Print();
	SKIP();
}

void PrintPreviewDialog::PrintKeyEvent(wxCommandEvent &event) {
	Print();
}

void PrintPreviewDialog::PageCountRefreshEvent(wxTimerEvent &event) {
	PageCountRefresh();
}

void PrintPreviewDialog::PageCountRefresh() {
	minPage = printPreview->GetMinPage();
	maxPage = printPreview->GetMaxPage();
	curPage = printPreview->GetCurrentPage();
	zoom    = ZOOM_NORMAL;

	RefreshStatusBar();

	pageCountRefreshTimer.Start(1000);
}

void PrintPreviewDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void PrintPreviewDialog::FirstPage() {
	printPreview->SetCurrentPage(minPage);
	curPage = minPage;

	RefreshStatusBar();
}

void PrintPreviewDialog::PreviousPage() {
	if(curPage - 1 >= minPage)
		printPreview->SetCurrentPage(--curPage);

	RefreshStatusBar();
}

void PrintPreviewDialog::NextPage() {
	if(curPage + 1 <= maxPage)
		printPreview->SetCurrentPage(++curPage);

	RefreshStatusBar();
}

void PrintPreviewDialog::LastPage() {
	printPreview->SetCurrentPage(maxPage);
	curPage = maxPage;

	RefreshStatusBar();
}

void PrintPreviewDialog::ZoomIn() {
	if(zoom + ZOOM_STEP <= ZOOM_MAX) {
		zoom += ZOOM_STEP;
		printPreview->SetZoom(zoom);
	}

	RefreshStatusBar();
}

void PrintPreviewDialog::ZoomNormal() {
	zoom = ZOOM_NORMAL;
	printPreview->SetZoom(zoom);

	RefreshStatusBar();
}

void PrintPreviewDialog::ZoomOut() {
	if(zoom - ZOOM_STEP >= ZOOM_MIN) {
		zoom -= ZOOM_STEP;
		printPreview->SetZoom(zoom);
	}

	RefreshStatusBar();
}

void PrintPreviewDialog::Print() {
	// XXX Something's going wrong with htmlPrintoutLibrary by
	// putting it to preview pane. So let's do a copy to correct
	// this behavior.

	HtmlPrintoutLibrary *tmp = htmlPrintoutLibrary->Copy();

	wxPrintData printData;
	printData.SetOrientation(orientation);
	wxPrintDialogData printDialogData(printData);
	wxPrinter printer(&printDialogData);
	if(!printed)
		printed = printer.Print(this, tmp, true);
	else
		printer.Print(this, tmp, true);
	
	delete tmp;

	/*if(!printed)
		printed = printPreview->Print(true);
	else
		printPreview->Print(true);*/
}

bool PrintPreviewDialog::Printed() {
	return printed;
}

void PrintPreviewDialog::RefreshStatusBar() {
	mainStatusBar->SetStatusText(_("Page ") + DSCasts::ToString(curPage) + _(" of ") + DSCasts::ToString(maxPage), 0);
	mainStatusBar->SetStatusText(_("Zoom: ") + DSCasts::ToString(zoom) + _N("%"), 1);
}

void PrintPreviewDialog::Zoom(const int rotation, const bool control) {
	if(control && rotation != 0) {
		if(rotation < 0)
			ZoomOut();
		else
			ZoomIn();
	}
}
