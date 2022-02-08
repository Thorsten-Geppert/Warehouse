#ifndef ENTERPRISE_CONTROLLER_H_
#define ENTERPRISE_CONTROLLER_H_

#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLEntityData.h>
#include "EnterpriseEntity.h"

using namespace DynSoft;

class EnterpriseController : public DSPostgreSQLEntityData {

	public:
		EnterpriseController(
			DSPostgreSQLDatabase *db
		);

};

#endif /* ENTERPRISE_CONTROLLER_H_ */
