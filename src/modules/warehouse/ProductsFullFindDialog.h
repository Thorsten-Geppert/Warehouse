#ifndef PRODUCTSFULLFINDDIALOG_H_
#define PRODUCTSFULLFINDDIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ProductEntity.h"

using namespace DynSoft;

class ProductsFullFindDialog : public wxDialog {

	public:
		ProductsFullFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int distributorId = 0
		);
		~ProductsFullFindDialog();
		ProductEntity *GetProductEntity() const;
		int ShowModal();
	
	protected:
		enum {
			CLOSE_EVENT,
			FIND_EVENT,
			USE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void FindEvent(wxMouseEvent &event);
		void UseEvent(wxMouseEvent &event);
		void UseDirectEvent(wxListEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Find();
		void Use();
		void Cancel();
		void ClearProductsListCtrl();
		void AddToProductsListCtrl(ProductEntity *productEntity);

		RuntimeInformationType *rit;
		unsigned int productId;
		unsigned int distributorId;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *findToolbarButton;
		ToolbarButton *useToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSTextCtrl    *productIdTextCtrl;
		wxTextCtrl    *articleNumberTextCtrl;
		wxTextCtrl    *eanTextCtrl;
		wxComboBox    *groupComboBox;
		wxComboBox    *distributorComboBox;
		wxComboBox    *manufacturerComboBox;
		wxComboBox    *publisherComboBox;
		wxTextCtrl    *shortNameTextCtrl;
		wxTextCtrl    *nameTextCtrl;
		wxTextCtrl    *descriptionTextCtrl;
		wxTextCtrl    *noteTextCtrl;
		wxTextCtrl    *isbn10TextCtrl;
		wxTextCtrl    *isbn13TextCtrl;
		DSListCtrl    *productsListCtrl;

};

#endif /* PRODUCTSFULLDIALOG_H_ */
