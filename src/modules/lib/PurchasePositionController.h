#ifndef PURCHASE_POSITION_CONTROLLER_H_
#define PURCHASE_POSITION_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "PurchasePositionEntity.h"

using namespace DynSoft;

class PurchasePositionController : public Controller {

	public:
		PurchasePositionController(
			RuntimeInformationType *rit
		);

};

#endif /* PURCHASE_POSITION_CONTROLLER_H_ */
