#ifndef DYNSOFT_WAREHOUSE_ADMIN_H_
#define DYNSOFT_WAREHOUSE_ADMIN_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabase.h>

using namespace DynSoft;

class Admin : public wxApp {

	public:
		Admin();
		~Admin();

		bool OnInit();

		DSPostgreSQLDatabase *db;

};

#endif /* DYNSOFT_WAREHOUSE_ADMIN_H_ */
