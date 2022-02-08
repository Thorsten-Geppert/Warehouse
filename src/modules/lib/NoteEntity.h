#ifndef NOTE_ENTITY_H_
#define NOTE_ENTITY_H_

#include "InformationEntity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class NoteEntity : public InformationEntity {

	public:
		NoteEntity(RuntimeInformationType *rit);

};

#endif /* NOTE_ENTITY_H_ */
