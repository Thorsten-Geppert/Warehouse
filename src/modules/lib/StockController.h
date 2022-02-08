#ifndef STOCK_CONTROLLER_H_
#define STOCK_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class StockController : public Controller {

	public:
		StockController(RuntimeInformationType *rit);

		wxArrayString GetAllStockNames();
	
};

#endif /* STOCK_CONTROLLER_H_ */
