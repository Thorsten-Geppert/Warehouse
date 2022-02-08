#include "LinkedFileEntities.h"
#include <DSCasts.h>

LinkedFileEntities::LinkedFileEntities() : DSTemplateContainer<LinkedFileEntity *>() {
}

LinkedFileEntities::~LinkedFileEntities() {
	Clear(true);
	CleanRemoved();
}

void LinkedFileEntities::AddToRemove(LinkedFileEntity *linkedFileEntity) {
	removedLinkedFiles.Add(linkedFileEntity);
	Remove(linkedFileEntity, false);
}

unsigned int LinkedFileEntities::GetRemoveCount() const {
	return removedLinkedFiles.GetCount();
}

LinkedFileEntity *LinkedFileEntities::GetRemove(unsigned int position) {
	if(position >= GetRemoveCount())
		return 0;

	return removedLinkedFiles.Get(position);
}

void LinkedFileEntities::CleanRemoved() {
	removedLinkedFiles.Clear(true);
}

void LinkedFileEntities::ResetId() {
	LinkedFileEntity *linkedFileEntity = NULL;
	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		linkedFileEntity = Get(i);
		if(linkedFileEntity)
			linkedFileEntity->SetValue(_N("linkedFileId"), 0U);
	}
}
