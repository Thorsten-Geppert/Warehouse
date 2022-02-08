#ifndef PAYEDDIALOG_H_
#define PAYEDDIALOG_H_

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/xrc/xmlres.h>
#include <DSDateTime.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class PayedDialog : public wxDialog {

	public:
		PayedDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &label
		);

		DSDateTime GetValue();

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

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton    *cancelToolbarButton;
		ToolbarButton    *saveToolbarButton;
		wxDatePickerCtrl *payedDatePickerCtrl;

};

#endif /* PAYEDDIALOG_H_ */
