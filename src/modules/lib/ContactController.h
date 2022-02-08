#ifndef CONTACT_CONTROLLER_H_
#define CONTACT_CONTROLLER_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLEntityData.h>

using namespace DynSoft;

class ContactController : public DSPostgreSQLEntityData {

	public:	
		ContactController(DSPostgreSQLDatabase *db);

};

#endif /* CONTACT_CONTROLLER_H_ */
