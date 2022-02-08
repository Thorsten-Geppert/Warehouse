#ifndef ADDRESS_CONTROLLER_H_
#define ADDRESS_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class AddressController : public Controller {

	public:
		AddressController(
			RuntimeInformationType *rit
		);
	
};

#endif /* ADDRESS_CONTROLLER_H_ */
