#ifndef PRODUCTSMODULEPANEL_H_
#define PRODUCTSMODULEPANEL_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/Toolbar.h"
#include "GuiCallbackInterface.h"
#include "ProductsFindDialog.h"
#include "StocksFrame.h"

using namespace DynSoft;

class ProductsModulePanel : public ModulePanel, public GuiCallbackInterface {

	public:
		ProductsModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~ProductsModulePanel();

		virtual void Load();
		virtual void Unload();
		virtual void Callback(void *data);
	
	protected:
		enum {
			NEW_EVENT,
			EDIT_EVENT,
			REMOVE_EVENT,
			FIND_EVENT,
			STOCKS_EVENT,
			EXPORT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void NewEvent(wxMouseEvent &event);
		void EditEvent(wxMouseEvent &event);
		void DirectEditEvent(wxListEvent &event);
		void RemoveEvent(wxMouseEvent &event);
		void FindEvent(wxMouseEvent &event);
		void NewKeyEvent(wxCommandEvent &event);
		void EditKeyEvent(wxCommandEvent &event);
		void RemoveKeyEvent(wxCommandEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void StocksEvent(wxMouseEvent &event);
		void StocksKeyEvent(wxCommandEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);

		void New(unsigned int *productId);
		void Edit(unsigned int *productId);
		void Remove();
		void Find();
		void Stocks();
		void Export();

		void AddOrChangeToProductsListCtrl(ProductEntity *productEntity, const bool justAppend);
		void ClearProductsListCtrl();

		// Stuff
		ProductsFindDialog *productsFindDialog;
		StocksFrame        *stocksFrame;

		ToolbarButton *newToolbarButton;
		ToolbarButton *editToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *findToolbarButton;
		ToolbarButton *informationToolbarButton;
		ToolbarButton *stocksToolbarButton;
		ToolbarButton *exportToolbarButton;
		DSListCtrl    *productsListCtrl;

};

#endif /* PRODUCTSMODULEPANEL_H_ */
