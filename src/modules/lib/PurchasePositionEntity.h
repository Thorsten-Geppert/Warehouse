#ifndef PURCHASE_POSITION_ENTTITY_H_
#define PURCHASE_POSITION_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class PurchasePositionEntity : public Entity {

	public:
		PurchasePositionEntity(RuntimeInformationType *rit);
		void Recalc();

};

#endif /* PURCHASE_POSITION_ENTTITY_H_ */
