#ifndef PRODUCTSFINDDIALOG_H_
#define PRODUCTSFINDDIALOG_H_

#include <wx/wx.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ProductEntity.h"

using namespace DynSoft;

class ProductsFindDialog : public wxDialog {

	public:
		ProductsFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		ProductEntity *GetProductEntity() const;
		int ShowModal();
	
	protected:
		enum {
			FIND_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void FindEvent(wxMouseEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Find();
		void Cancel();

		RuntimeInformationType *rit;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *findToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSTextCtrl    *productIdTextCtrl;
		wxTextCtrl    *articleNumberTextCtrl;
		wxComboBox    *groupComboBox;
		wxComboBox    *distributorComboBox;
		wxComboBox    *manufacturerComboBox;
		wxComboBox    *publisherComboBox;
		wxTextCtrl    *eanTextCtrl;
		wxTextCtrl    *shortNameTextCtrl;
		wxTextCtrl    *nameTextCtrl;
		wxTextCtrl    *descriptionTextCtrl;
		wxTextCtrl    *noteTextCtrl;
		wxTextCtrl    *isbn10TextCtrl;
		wxTextCtrl    *isbn13TextCtrl;

};

#endif /* PRODUCTSDIALOG_H_ */
