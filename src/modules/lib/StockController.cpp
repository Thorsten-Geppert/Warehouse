#include "StockController.h"
#include "StockEntity.h"

StockController::StockController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("stocks"), // Table
	_N("name"),  // Primary key
	_N("name")
) {
	//LoadResource(rit->GRP(_N("sql"), _N("Stocks.res")));
}

wxArrayString StockController::GetAllStockNames() {
	wxArrayString stockNames;

	StockEntity stockEntity(rit);
	StockController stockController(rit);
	stockController.Get();
	while(stockController.Run()) {
		stockController.Get(&stockEntity);
		if(stockEntity.Get(_N("active"))->ToBool())
			stockNames.Add(stockEntity.Get(_N("name"))->ToString());
	}

	return stockNames;
}
