#ifndef ORDER_ENTITIES_H_
#define ORDER_ENTITIES_H_

#include <DSTemplateContainer.h>

using namespace DynSoft;

class OrderEntity;
class OrderEntities : public DSTemplateContainer<OrderEntity *> {
	
	public:
		OrderEntities();

};

#endif /* ORDERENTITIES_H_ */
