#include "EntityContainer.h"

EntityContainer::EntityContainer(const bool cleanUp) : DSTemplateContainer<DSEntity *>() {
	this->cleanUp = cleanUp;
}

EntityContainer::~EntityContainer() {
	if(cleanUp)
		Clear(true);
}
