#include "Access.h"
#include <DSPostgreSQLQuery.h>

bool Access::Has(
	DSPostgreSQLDatabase *db,
	const wxString &username,
	const unsigned int status
) {
	DSPostgreSQLQuery query(db);
	query.Prepare(_N("SELECT COUNT(*) AS counter FROM users WHERE username = :username: AND status >= :status:"));
	query.Bind(_N("username"), username);
	query.Bind(_N("status"), status);
	query.Exec();
	query.NextRecord();
	const bool access = query.GetField(_N("counter")).ToUnsignedInt() > 0;
	query.FreeResult();
	return access;
}
