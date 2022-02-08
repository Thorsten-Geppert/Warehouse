#ifndef VOUCHERPOSITIONEDITDIALOG_H_
#define VOUCHERPOSITIONEDITDIALOG_H_

#include <wx/wx.h>
#include <DSBind.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/VoucherPositionEntity.h"

using namespace DynSoft;

class VoucherPositionEditDialog : public wxDialog {

	public:
		VoucherPositionEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			VoucherPositionEntity *voucherPositionEntity,
			const bool writeable = true
		);

	protected:
		enum {
			CLOSE_EVENT,
			SAVE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void CalculateTaxEvent(wxCommandEvent &event);

		void Close();
		bool Save();
		void CalculateTax();

		RuntimeInformationType *rit;
		DSBind bind;
		bool changed;
		bool writeable;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *saveToolbarButton;
		ToolbarButton *closeToolbarButton;
		DSTextCtrl    *quantityTextCtrl;
		DSTextCtrl    *descriptionTextCtrl;
		DSTextCtrl    *articleNumberTextCtrl;
		DSTextCtrl    *articleTextCtrl;
		DSComboBox    *taxComboBox;
		DSTextCtrl    *taxPriceTextCtrl;
		DSTextCtrl    *priceTextCtrl;

};

#endif /* VOUCHERPOSITIONEDITDIALOG_H_ */
