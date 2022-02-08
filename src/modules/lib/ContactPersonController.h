#ifndef CONTACTPERSON_CONTROLLER_H_
#define CONTACTPERSON_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class ContactPersonController : public Controller {

	public:
		ContactPersonController(
			RuntimeInformationType *rit
		);
	
};

#endif /* CONTACTPERSON_CONTROLLER_H_ */
