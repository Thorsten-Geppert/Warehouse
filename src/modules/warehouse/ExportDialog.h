#ifndef EXPORTDIALOG_H_
#define EXPORTDIALOG_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ExportType.h"

using namespace DynSoft;

class ExportDialog : public wxDialog {

	public:
		ExportDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			ExportType *exportType = NULL
		);

		ExportType *GetExportType() const;
		bool TransferToExportType() const;

	protected:
		enum {
			RUN_EVENT,
			BROWSE_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void RunEvent(wxMouseEvent &event);
		void RunKeyEvent(wxCommandEvent &event);
		void BrowseEvent(wxCommandEvent &event);
		void BrowseKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Run();
		void Browse();
		void Cancel();

		RuntimeInformationType *rit;
		ExportType *exportType;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *runToolbarButton;
		ToolbarButton *cancelToolbarButton;

		wxTextCtrl *fileTextCtrl;
		wxComboBox *fieldDelimiterComboBox;
		wxComboBox *textDelimiterComboBox;
		wxComboBox *lineFeedComboBox;
		wxCheckBox *replaceLineFeedCheckBox;
		wxTextCtrl *replaceLineFeedWithTextCtrl;

};

#endif /* EXPORTDIALOG_H_ */
