#ifndef ENTERPRISE_ENTITY_H_
#define ENTERPRISE_ENTITY_H_

#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLDatabaseEntity.h>

using namespace DynSoft;

class EnterpriseEntity : public DSPostgreSQLDatabaseEntity {

	public:
		EnterpriseEntity(DSPostgreSQLDatabase *db);

};

#endif /* ENTERPRISE_ENTITY_H_ */
