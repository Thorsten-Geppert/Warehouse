#ifndef PRODUCT_ENTTITY_H_
#define PRODUCT_ENTTITY_H_

#include "InformationEntity.h"
#include "RuntimeInformationType.h"
#include "LinkedFileEntities.h"

using namespace DynSoft;

class ProductEntity : public InformationEntity {

	public:
		ProductEntity(RuntimeInformationType *rit);
		LinkedFileEntities *GetLinkedFileEntities();

	protected:
		LinkedFileEntities linkedFileEntities;

};

#endif /* PRODUCT_ENTTITY_H_ */
