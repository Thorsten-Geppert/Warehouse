#ifndef MONITION_ENTTITY_H_
#define MONITION_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class MonitionEntity : public Entity {

	public:
		MonitionEntity(RuntimeInformationType *rit);
	
};

#endif /* MONITION_ENTTITY_H_ */
