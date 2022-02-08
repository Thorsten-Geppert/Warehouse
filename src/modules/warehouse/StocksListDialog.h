#ifndef STOCKS_LIST_DIALOG_H_
#define STOCKS_LIST_DIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class StocksListDialog : public wxDialog {

	public:
		StocksListDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const bool summarized
		);
		~StocksListDialog();

	protected:
		DECLARE_EVENT_TABLE()
		
		enum {
			PRINT_EVENT,
			PREVIEW_EVENT,
			EXPORT_EVENT,
			CLOSE_EVENT
		};

		void PrintEvent(wxMouseEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void PreviewEvent(wxMouseEvent &event);
		void PreviewKeyEvent(wxCommandEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);
		void CloseEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);

		void Print(const bool preview);
		void Export();
		void Close();
		void CleanUp();

		RuntimeInformationType *rit;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *printToolbarButton;
		ToolbarButton *previewToolbarButton;
		ToolbarButton *exportToolbarButton;
		ToolbarButton *closeToolbarButton;

		DSListCtrl    *resultsListCtrl;

};

#endif /* STOCKS_LIST_DIALOG_H_ */
