#ifndef CATEGORY_ENTTITY_H_
#define CATEGORY_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

class CategoryEntity : public Entity {

	public:
		CategoryEntity(
			RuntimeInformationType *rit,
			const wxString &table
		);

};

#endif /* CATEGORY_ENTTITY_H_ */
