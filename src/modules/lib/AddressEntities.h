#ifndef ADDRESS_ENTITIES_H_
#define ADDRESS_ENTITIES_H_

#include <DSTemplateContainer.h>
#include "AddressEntity.h"

using namespace DynSoft;

class AddressEntities : public DSTemplateContainer<AddressEntity *> {

	public:
		AddressEntities();

};

#endif /* ADDRESS_ENTITIES_H_ */
