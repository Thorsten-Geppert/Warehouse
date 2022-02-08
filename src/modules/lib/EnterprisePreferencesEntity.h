#ifndef ENTERPRISE_PREFERENCES_ENTITY_H_
#define ENTERPRISE_PREFERENCES_ENTITY_H_

#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLDatabaseEntity.h>

using namespace DynSoft;

class EnterprisePreferencesEntity : public DSPostgreSQLDatabaseEntity {

	public:
		EnterprisePreferencesEntity(DSPostgreSQLDatabase *db);
		bool IsComplete();
		wxString AsPostAddress();

};

#endif /* ENTERPRISE_PREFERENCES_ENTITY_H_ */
