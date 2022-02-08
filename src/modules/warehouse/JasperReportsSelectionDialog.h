#ifndef JASPER_REPORTS_SELECTIONDIALOG_H_
#define JASPER_REPORTS_SELECTIONDIALOG_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/JasperReportsEntities.h"

class JasperReportsSelectionDialog : public wxDialog {

	public:
		JasperReportsSelectionDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			JasperReportsEntities &jasperReportsEntities
		);

		JasperReportsEntity *GetSelectedEntry();

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
		void ChangeReportEvent(wxCommandEvent &event);

		void Cancel();
		void Use();
		void ChangeReport();

		RuntimeInformationType *rit;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *useToolbarButton;

		wxStaticText *reportStaticText;
		wxComboBox   *reportComboBox;
		wxStaticText *descriptionStaticText;
		wxTextCtrl   *descriptionTextCtrl;

};

#endif /* JASPER_REPORTS_SELECTIONDIALOG_H_ */
