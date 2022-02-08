#ifndef PURCHASE_POSITION_ENTITIES_H_
#define PURCHASE_POSITION_ENTITIES_H_

#include <DSTemplateContainer.h>
#include "PurchasePositionEntity.h"

using namespace DynSoft;

class PurchasePositionEntities : public DSTemplateContainer<PurchasePositionEntity *> {
	
	public:
		PurchasePositionEntities();

};

#endif /* PURCHASE_POSITION_ENTITIES_H_ */
