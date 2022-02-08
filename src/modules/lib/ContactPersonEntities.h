#ifndef CONTACTPERSON_ENTITIES_H_
#define CONTACTPERSON_ENTITIES_H_

#include <DSTemplateContainer.h>
#include "ContactPersonEntity.h"

using namespace DynSoft;

class ContactPersonEntities : public DSTemplateContainer<ContactPersonEntity *> {

	public:
		ContactPersonEntities();

};

#endif /* CONTACTPERSON_ENTITIES_H_ */
