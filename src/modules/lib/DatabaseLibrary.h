#ifndef DATABASE_LIBRARY_H_
#define DATABASE_LIBRARY_H_

#include "DatabaseType.h"

#define DATABASE_LIBRARY_OK               0
#define DATABASE_LIBRARY_ERROR_CONNECTION 1
#define DATABASE_LIBRARY_ERROR_CREATION   2
#define DATABASE_LIBRARY_ERROR_CONNECT    3
#define DATABASE_LIBRARY_ERROR_INSERT     4
#define DATABASE_LIBRARY_ERROR_EXISTS     5
#define DATABASE_LIBRARY_ERROR_DELETION   6
#define DATABASE_LIBRARY_ERROR_VACUUM     7

#include <DSType.h>
#include <DSPostgreSQLDatabase.h>
#include "RuntimeInformationType.h"

using namespace DynSoft;

class DatabaseLibrary : public DSType {

	public:
		DatabaseLibrary();
		DatabaseLibrary(RuntimeInformationType *rit);

		unsigned int CreateDatabase(DatabaseType &databaseType, const wxString &sql = wxEmptyString);
		unsigned int DropDatabase(DatabaseType &databaseType);
		bool DatabaseExists(DSPostgreSQLDatabase *db, const wxString &name);
		static wxString BuildDatabaseName(const wxString &fullName);
		unsigned int ReorganizeDatabase(DSPostgreSQLDatabase *db, DatabaseType &databaseType);
		bool ReorganizeDatabase(DSPostgreSQLDatabase &db);
	
	private:
		RuntimeInformationType *rit;

};

#endif /* DATABASE_LIBRARY_H_ */
