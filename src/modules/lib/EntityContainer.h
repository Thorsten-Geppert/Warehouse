#ifndef ENTITY_CONTAINER_H_
#define ENTITY_CONTAINER_H_

#include <wx/wx.h>
#include <DSTemplateContainer.h>
#include <DSEntity.h>

using namespace DynSoft;

class EntityContainer : public DSTemplateContainer<DSEntity *> {

	public:
		EntityContainer(const bool cleanUp = true);
		~EntityContainer();

	protected:
		DSTemplateContainer<DSEntity *> entities;
		bool cleanUp;

};

#endif /* ENTITY_CONTAINER_H_ */
