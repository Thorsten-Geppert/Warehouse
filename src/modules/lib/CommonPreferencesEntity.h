#ifndef COMMON_PREFERENCES_ENTITY_H_
#define COMMON_PREFERENCES_ENTITY_H_

#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLDatabaseEntity.h>

using namespace DynSoft;

class CommonPreferencesEntity : public DSPostgreSQLDatabaseEntity {

	public:
		CommonPreferencesEntity(DSPostgreSQLDatabase *db);

};

#endif /* COMMON_PREFERENCES_ENTITY_H_ */
