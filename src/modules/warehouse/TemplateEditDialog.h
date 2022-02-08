#ifndef TEMPLATEEDITDIALOG_H_
#define TEMPLATEEDITDIALOG_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

class TemplateEditDialog : public wxDialog {

	public:
		TemplateEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int templateId = 0
		);

	protected:
		DECLARE_EVENT_TABLE()
		
		enum {
			SAVE_EVENT,
			CLOSE_EVENT
		};

		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);

		void Cancel();
		void Save();

		RuntimeInformationType *rit;
		unsigned int templateId;
		wxString oldName;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *saveToolbarButton;

		wxTextCtrl *nameTextCtrl;
		wxTextCtrl *contentTextCtrl;

};

#endif /* TEMPLATEEDITDIALOG_H_ */
