#ifndef BUG_CONTROLLER_H_
#define BUG_CONTROLLER_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLEntityData.h>

using namespace DynSoft;

class BugController : public DSPostgreSQLEntityData {

	public:	
		BugController(DSPostgreSQLDatabase *db);

};

#endif /* BUG_CONTROLLER_H_ */
