#ifndef HTMLTABLECREATIONDIALOG_H_
#define HTMLTABLECREATIONDIALOG_H_

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/xrc/xmlres.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

class HtmlTableCreationDialog : public wxDialog {

	public:
		HtmlTableCreationDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int tabs
		);

		wxString GetTable();
	
	protected:
		DECLARE_EVENT_TABLE()
		
		enum {
			INSERT_EVENT,
			CANCEL_EVENT
		};

		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void InsertEvent(wxMouseEvent &event);
		void InsertKeyEvent(wxCommandEvent &event);

		void Cancel();
		void Insert();
		wxString CreateTabs(const unsigned int tabs);

		RuntimeInformationType *rit;
		unsigned int tabs;

		Toolbar *mainToolbar;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *insertToolbarButton;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		wxSpinCtrl *columnsSpinCtrl;
		wxSpinCtrl *rowsSpinCtrl;
		wxCheckBox *borderCheckBox;

};

#endif /* HTMLTABLECREATIONDIALOG_H_ */
