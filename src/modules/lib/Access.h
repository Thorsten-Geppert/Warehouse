#ifndef ACCESS_H_
#define ACCESS_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabase.h>

using namespace DynSoft;

class Access {

	public:
		static bool Has(
			DSPostgreSQLDatabase *db,
			const wxString &username,
			const unsigned int status
		);

};

#endif /* ACCESS_H_ */
