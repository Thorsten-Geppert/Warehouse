#ifndef INVOICEDIALOG_H_
#define INVOICEDIALOG_H_

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <wx/xrc/xmlres.h>
#include <DSDateTime.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class InvoiceDialog : public wxDialog {

	public:
		InvoiceDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
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
		wxDatePickerCtrl *invoiceDatePickerCtrl;

};

#endif /* INVOICEDIALOG_H_ */
