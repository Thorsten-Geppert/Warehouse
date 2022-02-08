#ifndef BUG_ENTITY_H_
#define BUG_ENTITY_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLDatabaseEntity.h>

using namespace DynSoft;

class BugEntity : public DSPostgreSQLDatabaseEntity {

	public:
		BugEntity(DSPostgreSQLDatabase *db = NULL);

};

#endif /* BUG_ENTITY_H_ */
