#ifndef ENTERPRISE_PREFERENCES_CONTROLLER_H_
#define ENTERPRISE_PREFERENCES_CONTROLLER_H_

#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLEntityData.h>
#include "EnterprisePreferencesEntity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class EnterprisePreferencesController : public DSPostgreSQLEntityData {

	public:
		EnterprisePreferencesController(
			DSPostgreSQLDatabase *db
		);

		static bool CheckEnterpriseProfile(RuntimeInformationType *rit);

};

#endif /* ENTERPRISE_PREFERENCES_CONTROLLER_H_ */
