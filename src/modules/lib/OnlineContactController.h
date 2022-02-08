#ifndef ONLINE_CONTACT_CONTROLLER_H_
#define ONLINE_CONTACT_CONTROLLER_H_

#include <wx/wx.h>
#include <DSPostgreSQLEntityData.h>
#include <DSPostgreSQLDatabase.h>
#include "OnlineContactEntity.h"

using namespace DynSoft;

class OnlineContactController : public DSPostgreSQLEntityData {

	public:
		OnlineContactController(DSPostgreSQLDatabase *db);
		bool Mail(const wxString &to, OnlineContactEntity *contact);

};

#endif /* ONLINE_CONTACT_CONTROLLER_H_ */
