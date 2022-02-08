#ifndef STOCKDIALOG_H_
#define STOCKDIALOG_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/StockEntity.h"
#include "../lib/Toolbar.h"

class StockDialog : public wxDialog {

	public:
		StockDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			StockEntity *stockEntity = NULL
		);

		StockEntity *GetStock() const;

	protected:
		enum {
			SAVE_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);

		void Cancel();
		void Save();
		void TemplateSelect(wxObject *object);

		RuntimeInformationType *rit;
		StockEntity *stockEntity;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *saveToolbarButton;
		wxTextCtrl    *nameTextCtrl;
		wxTextCtrl    *addressTextCtrl;
		wxTextCtrl    *numberTextCtrl;
		wxTextCtrl    *zipcodeTextCtrl;
		wxTextCtrl    *cityTextCtrl;
		wxTextCtrl    *commentTextCtrl;
		wxCheckBox    *activeCheckBox;

};

#endif /* STOCKDIALOG_H_ */
