#ifndef LINKED_FILES_ENTITIES_H_
#define LINKED_FILES_ENTITIES_H_

#include <DSTemplateContainer.h>
#include "LinkedFileEntity.h"

using namespace DynSoft;

class LinkedFileEntities : public DSTemplateContainer<LinkedFileEntity *> {

	public:
		LinkedFileEntities();
		~LinkedFileEntities();
		void AddToRemove(LinkedFileEntity *linkedFileEntity);
		unsigned int GetRemoveCount() const;
		LinkedFileEntity *GetRemove(unsigned int position);
		void CleanRemoved();
		void ResetId();

	protected:
		DSTemplateContainer<LinkedFileEntity *> removedLinkedFiles;

};

#endif /* LINKED_FILES_ENTITIES_H_ */
