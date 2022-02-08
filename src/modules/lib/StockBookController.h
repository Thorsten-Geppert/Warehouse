#ifndef STOCK_BOOK_CONTROLLER_H_
#define STOCK_BOOK_CONTROLLER_H_

#include <DSCurrency.h>

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "StockBookEntity.h"
#include "StockBookEntities.h"
#include "HtmlPrintoutLibrary.h"

using namespace DynSoft;

class StockBookController : public Controller {

	public:
		StockBookController(RuntimeInformationType *rit);

		bool GetByStockAndProduct(StockBookEntity &stockBookEntity);
		StockBookEntities *GetSeparately(const bool autoCleanUp = true);
		StockBookEntities *GetSummarized(const bool autoCleanUp = true);
		HtmlPrintoutLibrary *PrintList(DSListCtrl *listCtrl);
		DSCurrency GetQuantityByStockAndProduct(const wxString &stock, const unsigned int product);
		DSCurrency GetReservedByStockAndProduct(const wxString &stock, const unsigned int product);
	
};

#endif /* STOCK_BOOK_CONTROLLER_H_ */
