#ifndef CONTACT_ENTTITY_H_
#define CONTACT_ENTTITY_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLDatabaseEntity.h>

using namespace DynSoft;

class ContactEntity : public DSPostgreSQLDatabaseEntity {

	public:
		ContactEntity(DSPostgreSQLDatabase *db = NULL);

};

#endif /* CONTACT_ENTTITY_H_ */
