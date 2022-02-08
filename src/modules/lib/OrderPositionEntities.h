#ifndef ORDER_POSITION_ENTITIES_H_
#define ORDER_POSITION_ENTITIES_H_

#include <DSTemplateContainer.h>
#include "OrderPositionEntity.h"

using namespace DynSoft;

class OrderPositionEntities : public DSTemplateContainer<OrderPositionEntity *> {
	
	public:
		OrderPositionEntities();

};

#endif /* ORDER_POSITION_ENTITIES_H_ */
