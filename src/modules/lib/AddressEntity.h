#ifndef ADDRESS_ENTTITY_H_
#define ADDRESS_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class ClientEntity;
class AddressEntity : public Entity {

	public:
		AddressEntity(RuntimeInformationType *rit, ClientEntity *clientEntity = NULL);

};

#endif /* ADDRESS_ENTTITY_H_ */
