#ifndef STOCK_BOOK_ENTTITY_H_
#define STOCK_BOOK_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class StockBookEntity : public Entity {

	public:
		StockBookEntity(RuntimeInformationType *rit);

		bool GetByStockAndProduct(StockBookEntity &stockBookEntity);

};

#endif /* STOCK_BOOK_ENTTITY_H_ */
