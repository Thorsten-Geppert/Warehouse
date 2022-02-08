#ifndef STOCK_BOOK_ENTITIES_H_
#define STOCK_BOOK_ENTITIES_H_

#include <DSTemplateContainer.h>
#include "StockBookEntity.h"

using namespace DynSoft;

class StockBookEntities : public DSTemplateContainer<StockBookEntity *> {

	public:
		StockBookEntities(const bool autoCleanUp = true);
		~StockBookEntities();
	
	protected:
		bool autoCleanUp;

};

#endif /* STOCK_BOOK_ENTITIES_H_ */
