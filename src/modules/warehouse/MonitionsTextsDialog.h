#ifndef MONITIONSTEXTSDIALOG_H_
#define MONITIONSTEXTSDIALOG_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/MonitionTextEntity.h"

using namespace DynSoft;

class MonitionsTextsDialog : public wxDialog {

	public:
		MonitionsTextsDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~MonitionsTextsDialog();
	
	protected:
		enum {
			SAVE_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void SaveEvent(wxMouseEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);
		void StateSelectEvent(wxCommandEvent &event);
		void TextEvent(wxCommandEvent &event);
		
		void Save();
		void Cancel();
		void TemplateSelect(wxObject *object);
		void StateSelect(MonitionTextEntity *monitionTextEntity);
		void Text(wxTextCtrl *textCtrl);
		void CleanUp();
		void ErrorText();

		RuntimeInformationType *rit;
		MonitionTextEntity *monitionTextEntity;
		bool fireEvent;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton  *saveToolbarButton;
		ToolbarButton  *cancelToolbarButton;
		wxListBox      *statesListBox;
		wxStaticText   *prependTextStaticText;
		wxTextCtrl     *prependTextTextCtrl;
		wxStaticText   *appendTextStaticText;
		wxTextCtrl     *appendTextTextCtrl;

};

#endif /* MONITIONSTEXTSDIALOG_H_ */
