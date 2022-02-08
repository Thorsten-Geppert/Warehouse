#ifndef ONLINE_CONTACT_ENTTITY_H_
#define ONLINE_CONTACT_ENTTITY_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabaseEntity.h>
#include <DSPostgreSQLDatabase.h>

using namespace DynSoft;

class OnlineContactEntity : public DSPostgreSQLDatabaseEntity {

	public:
		OnlineContactEntity(DSPostgreSQLDatabase *db);

};

#endif /* ONLINE_CONTACT_ENTTITY_H_ */
