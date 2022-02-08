#ifndef STOCK_ENTTITY_H_
#define STOCK_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class StockEntity : public Entity {

	public:
		StockEntity(RuntimeInformationType *rit);

};

#endif /* STOCK_ENTTITY_H_ */
