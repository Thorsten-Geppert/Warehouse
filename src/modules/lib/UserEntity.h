#ifndef USER_ENTITY_H_
#define USER_ENTITY_H_

#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLDatabaseEntity.h>

using namespace DynSoft;

class UserEntity : public DSPostgreSQLDatabaseEntity {

	public:
		UserEntity(DSPostgreSQLDatabase *db);

};


#endif /* USER_ENTITY_H_ */
