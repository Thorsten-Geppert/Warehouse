#ifndef TEMPLATE_CONTROLLER_H_
#define TEMPLATE_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class TemplateController : public Controller {

	public:
		TemplateController(
			RuntimeInformationType *rit
		);

		bool ExistsByName(const wxString &name, const wxString &oldName);
	
};

#endif /* TEMPLATE_CONTROLLER_H_ */
