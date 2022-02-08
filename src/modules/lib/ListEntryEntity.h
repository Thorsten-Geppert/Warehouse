#ifndef LISTENTRY_ENTITY_H_
#define LISTENTRY_ENTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class ListEntryEntity : public Entity {

	public:
		ListEntryEntity(
			RuntimeInformationType *rit,
			const wxString &table,
			const wxString &field
		);

	protected:
		wxString pk;

};

#endif /* LISTENTRY_ENTITY_H_ */
