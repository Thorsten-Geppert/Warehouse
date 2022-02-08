#ifndef ORDERPOSITIONEDITDIALOG_H_
#define ORDERPOSITIONEDITDIALOG_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSBind.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include <DSCurrency.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/OrderPositionEntity.h"
#include "../lib/ProductEntity.h"

using namespace DynSoft;

class OrderPositionEditDialog : public wxDialog {

	public:
		OrderPositionEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			OrderPositionEntity *orderPositionEntity,
			const unsigned int tax
		);

	protected:
		enum {
			CLOSE_EVENT,
			SAVE_EVENT,
			FIND_PRODUCT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void FindProductEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void RefreshSumEvent(wxCommandEvent &event);
		void EditProductIdEvent(wxCommandEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void FindProductKeyEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);
		void GetStockQuantityEvent(wxCommandEvent &event);

		void Close();
		void FindProduct();
		bool Save();
		void RefreshSum();
		void EditProductId(const unsigned int productId);
		void TemplateSelect(wxObject *object);
		void LoadStocks();
		void GetStockQuantity();

		RuntimeInformationType *rit;
		DSBind bind;
		bool changed;
		unsigned int tax;
		DSCurrency taxPercent;
		unsigned int taxLevel;
		ProductEntity productEntity;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *saveToolbarButton;
		ToolbarButton *findArticleToolbarButton;
		ToolbarButton *closeToolbarButton;
		DSComboBox    *stocksComboBox;
		wxStaticText  *stockQuantityStaticText;
		DSTextCtrl    *amountTextCtrl;
		DSTextCtrl    *productIdTextCtrl;
		DSTextCtrl    *priceTextCtrl;
		DSTextCtrl    *reductionTextCtrl;
		wxStaticText  *sumStaticText;
		wxTextCtrl    *productInformationsTextCtrl;
		wxStaticText  *textStaticText;
		DSTextCtrl    *textTextCtrl;

};

#endif /* ORDERPOSITIONEDITDIALOG_H_ */
