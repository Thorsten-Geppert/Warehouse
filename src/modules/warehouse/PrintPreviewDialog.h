#ifndef PRINTPREVIEWDIALOGDIALOG_H_
#define PRINTPREVIEWDIALOGDIALOG_H_

#include <wx/wx.h>
#include <wx/print.h>
#include <wx/html/htmprint.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/HtmlPrintoutLibrary.h"

using namespace DynSoft;

class PrintPreviewDialog : public wxDialog {

	public:
		static const unsigned int ZOOM_MIN    = 50;
		static const unsigned int ZOOM_MAX    = 400;
		static const unsigned int ZOOM_NORMAL = 100;
		static const unsigned int ZOOM_STEP   = 50;

	public:
		PrintPreviewDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			HtmlPrintoutLibrary *htmlPrintoutLibrary,
			#if wxMAJOR_VERSION < 3
				const int orientation = wxPORTRAIT
			#else
				const wxPrintOrientation orientation = wxPORTRAIT
			#endif
		);
		~PrintPreviewDialog();
		bool Printed();
	
	protected:
		enum {
			CANCEL_EVENT,
			FIRST_PAGE_EVENT,
			PREVIOUS_PAGE_EVENT,
			NEXT_PAGE_EVENT,
			LAST_PAGE_EVENT,
			ZOOM_IN_EVENT,
			ZOOM_NORMAL_EVENT,
			ZOOM_OUT_EVENT,
			PRINT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void FirstPageEvent(wxMouseEvent &event);
		void FirstPageKeyEvent(wxCommandEvent &event);
		void PreviousPageEvent(wxMouseEvent &event);
		void PreviousPageKeyEvent(wxCommandEvent &event);
		void NextPageEvent(wxMouseEvent &event);
		void NextPageKeyEvent(wxCommandEvent &event);
		void LastPageEvent(wxMouseEvent &event);
		void LastPageKeyEvent(wxCommandEvent &event);
		void ZoomInEvent(wxMouseEvent &event);
		void ZoomInKeyEvent(wxCommandEvent &event);
		void ZoomNormalEvent(wxMouseEvent &event);
		void ZoomNormalKeyEvent(wxCommandEvent &event);
		void ZoomOutEvent(wxMouseEvent &event);
		void ZoomOutKeyEvent(wxCommandEvent &event);
		void ZoomEvent(wxMouseEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void PageCountRefreshEvent(wxTimerEvent &event);

		void Cancel();
		void FirstPage();
		void PreviousPage();
		void NextPage();
		void LastPage();
		void Zoom(const int rotation, const bool control);
		void ZoomIn();
		void ZoomNormal();
		void ZoomOut();
		void Print();
		void RefreshStatusBar();
		void PageCountRefresh();

		RuntimeInformationType *rit;
		HtmlPrintoutLibrary *htmlPrintoutLibrary;
		bool deleteHtmlPrintoutAfterClosing;
		int minPage, maxPage, curPage;
		unsigned int zoom;
		bool printed;
		wxTimer pageCountRefreshTimer;
		#if wxMAJOR_VERSION < 3
			int orientation;
		#else
			wxPrintOrientation orientation;
		#endif

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton   *cancelToolbarButton;
		ToolbarButton   *firstPageToolbarButton;
		ToolbarButton   *previousPageToolbarButton;
		ToolbarButton   *nextPageToolbarButton;
		ToolbarButton   *lastPageToolbarButton;
		ToolbarButton   *printToolbarButton;
		ToolbarButton   *zoomInToolbarButton;
		ToolbarButton   *zoomNormalToolbarButton;
		ToolbarButton   *zoomOutToolbarButton;
		wxPanel         *printPanel;
		wxBoxSizer      *printBoxSizer;
		wxPreviewCanvas *previewCanvas;
		wxPrintData     *printData;
		wxPrintPreview  *printPreview;
		wxStatusBar     *mainStatusBar;

};

#endif /* PRINTPREVIEWDIALOG_H_ */
