#ifndef TEXTEDITDIALOG_H_
#define TEXTEDITDIALOG_H_

#include <wx/wx.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class TextEditDialog : public wxDialog {

	public:
		TextEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			bool *changed = NULL,
			wxString *value = NULL
		);
		wxString GetValue() const;
		void SetValue(const wxString &text);
		DSTextCtrl *GetTextCtrl() const;
		void ResetValue();

	protected:
		enum {
			SAVE_EVENT,
			PRINT_EVENT,
			PRINT_PREVIEW_EVENT,
			COPY_EVENT,
			CUT_EVENT,
			PASTE_EVENT,
			IMPORT_EVENT,
			EXPORT_EVENT,
			CANCEL_EVENT
		};
		
		DECLARE_EVENT_TABLE();
		
		void OnClose(wxCloseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void PrintPreviewEvent(wxMouseEvent &event);
		void CopyEvent(wxMouseEvent &event);
		void CutEvent(wxMouseEvent &event);
		void PasteEvent(wxMouseEvent &event);
		void ImportEvent(wxMouseEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void CancelEvent(wxMouseEvent &event);
		
		// restlichen events
		void SaveKeyEvent(wxCommandEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void PrintPreviewKeyEvent(wxCommandEvent &event);
		void CopyKeyEvent(wxCommandEvent &event);
		void CutKeyEvent(wxCommandEvent &event);
		void PasteKeyEvent(wxCommandEvent &event);
		void ImportKeyEvent(wxCommandEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		
		bool Save();
		void Print(const bool preview = false);
		void Copy();
		void Cut();
		void Paste();
		void Import();
		void Export();
		void Cancel();

		RuntimeInformationType *rit;
		wxString lastExportFile, lastImportFile;
		bool *changed;
		bool innerChanged;
		wxString *value;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		Toolbar       *mainToolbar;
		ToolbarButton *saveToolbarButton;
		ToolbarButton *printToolbarButton;
		ToolbarButton *printPreviewToolbarButton;
		ToolbarButton *copyToolbarButton;
		ToolbarButton *cutToolbarButton;
		ToolbarButton *pasteToolbarButton;
		ToolbarButton *importToolbarButton;
		ToolbarButton *exportToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSTextCtrl *informationTextCtrl;

};

#endif /* TEXTEDITDIALOG_H_ */
