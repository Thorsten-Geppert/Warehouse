#ifndef LOG_DIALOG_H_
#define LOG_DIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include <DSLogType.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "LogFindDialog.h"

using namespace DynSoft;

class LogDialog : public wxDialog {

	public:
		LogDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~LogDialog();
	
	protected:
		enum {
			RELOAD_EVENT,
			FIND_EVENT,
			CLOSE_EVENT,
			EXPORT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void ReloadEvent(wxMouseEvent &event);
		void FindEvent(wxMouseEvent &event);
		void CloseEvent(wxMouseEvent &event);
		void ReloadKeyEvent(wxCommandEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);

		void Reload(DSLogType *logType = NULL);
		void Find();
		void Close();
		void Export();

		RuntimeInformationType *rit;
		LogFindDialog *logFindDialog;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		ToolbarButton *reloadToolbarButton;
		ToolbarButton *findToolbarButton;
		ToolbarButton *exportToolbarButton;
		ToolbarButton *closeToolbarButton;
		DSListCtrl *logListCtrl;

};

#endif /* LOG_DIALOG_H_ */
