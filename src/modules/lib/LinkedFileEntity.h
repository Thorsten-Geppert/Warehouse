#ifndef LINKED_FILE_ENTTITY_H_
#define LINKED_FILE_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

class LinkedFileEntity : public Entity {

	public:
		LinkedFileEntity(RuntimeInformationType *rit, const wxString &table);

};

#endif /* LINKED_FILE_ENTTITY_H_ */
