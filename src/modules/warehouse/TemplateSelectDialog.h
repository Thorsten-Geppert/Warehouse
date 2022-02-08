#ifndef TEMPLATESELECTDIALOG_H_
#define TEMPLATESELECTDIALOG_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/TemplateEntity.h"

class TemplateSelectDialog : public wxDialog {

	public:
		TemplateSelectDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			wxTextCtrl *textCtrl = NULL
		);
		~TemplateSelectDialog();

		TemplateEntity *GetTemplateEntity();
		unsigned int GetTemplateId();
		wxString GetTemplateName();
		wxString GetTemplateContent();

		static bool ShowAndSet(wxWindow *parent, RuntimeInformationType *rit, wxTextCtrl *textCtrl);

	protected:
		DECLARE_EVENT_TABLE()
		
		enum {
			USE_EVENT,
			CLOSE_EVENT
		};

		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void UseEvent(wxMouseEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void SelectEvent(wxCommandEvent &event);

		void Cancel();
		void Use();
		void TextTemplateCleanUp();
		void TextTemplateRefresh();
		void Select();

		RuntimeInformationType *rit;
		wxTextCtrl *textCtrl;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *useToolbarButton;
		wxListBox     *textTemplateListBox;
		wxTextCtrl    *textTemplateTextCtrl;

};

#endif /* TEMPLATESELECTDIALOG_H_ */
