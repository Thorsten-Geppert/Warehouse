#ifndef STOCKSFRAME_H_
#define STOCKSFRAME_H_

#include <wx/wx.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include "../lib/Toolbar.h"
#include "../lib/RuntimeInformationType.h"

using namespace DynSoft;

class StocksFrame : public wxFrame {

	public:
		StocksFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~StocksFrame();
		bool Show(const bool show);

	protected:
		enum {
			FIND_PRODUCT_EVENT,
			BOOK_EVENT,
			LIST_EVENT,
			CLOSE_EVENT,
			LIST_SUMMARIZED_EVENT,
			LIST_SEPARATELY_EVENT
		};

		DECLARE_EVENT_TABLE()

		void FindProductEvent(wxMouseEvent &event);
		void FindProductKeyEvent(wxCommandEvent &event);
		void BookEvent(wxMouseEvent &event);
		void BookKeyEvent(wxCommandEvent &event);
		void ListEvent(wxMouseEvent &event);
		void ListKeyEvent(wxCommandEvent &event);
		void CloseEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void HideEvent(wxCloseEvent &event);
		void ListSummarizedEvent(wxCommandEvent &event);
		void ListSeparatelyEvent(wxCommandEvent &event);
		void GetSumEvent(wxCommandEvent &event);

		void FindProduct();
		void Book();
		void List();
		void List(const int type);
		void Close();
		void GetSum();

		void ReloadStocks(wxString selectedName = wxEmptyString);

		RuntimeInformationType *rit;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		ToolbarButton    *findProductToolbarButton;
		ToolbarButton    *bookToolbarButton;
		ToolbarButton    *listToolbarButton;
		ToolbarButton    *closeToolbarButton;
		DSComboBox       *stocksComboBox;
		DSTextCtrl       *productTextCtrl;
		DSTextCtrl       *quantityTextCtrl;
		DSTextCtrl       *commentTextCtrl;
		wxTextCtrl       *fullQuantityTextCtrl;
		wxTextCtrl       *reservedTextCtrl;
		wxTextCtrl       *sumTextCtrl;

};

#endif /* STOCKSFRAME_H_ */
