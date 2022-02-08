#ifndef CONTACT_H_
#define CONTACT_H_

#include <DSPostgreSQLDatabase.h>
#include <DSCgi.h>
#include <DSWeb.h>

using namespace DynSoft;

class DynSoftContact : public DSWeb {

	public:
		DynSoftContact();
		~DynSoftContact();

		int Init();
		int Run();

	private:
		DSPostgreSQLDatabase *db;

};

#endif /* CONTACT_H_ */
