#ifndef MONITION_TEXT_CONTROLLER_H_
#define MONITION_TEXT_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "MonitionTextEntity.h"

using namespace DynSoft;

class MonitionTextController : public Controller {

	public:
		MonitionTextController(
			RuntimeInformationType *rit
		);

		MonitionTextEntity *GetDirect(const int number, const bool createNewInstanceIfNotFound = false);

};

#endif /* MONITION_TEXT_CONTROLLER_H_ */
