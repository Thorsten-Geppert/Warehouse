#ifndef PURCHASE_ENTITIES_H_
#define PURCHASE_ENTITIES_H_

#include <DSTemplateContainer.h>

using namespace DynSoft;

class PurchaseEntity;
class PurchaseEntities : public DSTemplateContainer<PurchaseEntity *> {
	
	public:
		PurchaseEntities();

};

#endif /* PURCHASEENTITIES_H_ */
