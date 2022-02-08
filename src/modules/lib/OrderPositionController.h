#ifndef ORDER_POSITION_CONTROLLER_H_
#define ORDER_POSITION_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "OrderPositionEntity.h"

using namespace DynSoft;

class OrderPositionController : public Controller {

	public:
		OrderPositionController(
			RuntimeInformationType *rit
		);

};

#endif /* ORDER_POSITION_CONTROLLER_H_ */
