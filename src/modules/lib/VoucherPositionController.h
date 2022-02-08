#ifndef VOUCHER_POSITION_CONTROLLER_H_
#define VOUCHER_POSITION_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "VoucherPositionEntity.h"

using namespace DynSoft;

class VoucherPositionController : public Controller {

	public:
		VoucherPositionController(
			RuntimeInformationType *rit
		);

};

#endif /* VOUCHER_POSITION_CONTROLLER_H_ */
