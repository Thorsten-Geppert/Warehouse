#ifndef VOUCHER_POSITION_ENTITIES_H_
#define VOUCHER_POSITION_ENTITIES_H_

#include <DSTemplateContainer.h>
#include "VoucherPositionEntity.h"

using namespace DynSoft;

class VoucherPositionEntities : public DSTemplateContainer<VoucherPositionEntity *> {
	
	public:
		VoucherPositionEntities();

};

#endif /* VOUCHER_POSITION_ENTITIES_H_ */
