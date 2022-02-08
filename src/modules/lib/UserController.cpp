#include "UserController.h"
#include <DSPostgreSQLQuery.h>

UserController::UserController(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLEntityData(
	db,
	_N("users"),
	_N("username"),
	_N("username")
) {
	rit = NULL;
}

UserController::UserController(
	RuntimeInformationType *rit
) : DSPostgreSQLEntityData(
	rit->GetDatabase(),
	_N("users"),
	_N("username"),
	_N("username")
) {
	this->rit = rit;
	LoadResource(rit->GRP(_N("sql"), _N("Users.res")));
}

bool UserController::Login(
	const wxString &username,
	const wxString &password
) {
	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Prepare(_N(
		"SELECT "
		"	COUNT(*) AS counter "
		"FROM "
		"	users "
		"WHERE "
		"	username = :username: "
		"AND "
		"	password = MD5(:password:) "
		"AND "
		"	status > 0"
	));
	query.Bind(_N("username"), username);
	query.Bind(_N("password"), password);
	query.Exec();
	query.NextRecord();
	bool loggedIn = query.GetField(_N("counter")).ToUnsignedInt() > 0;
	query.FreeResult();
	return loggedIn;
}

bool UserController::SetOpenToRemind(const wxString &username, const bool openToRemind) {
	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Prepare(GetResourceByName(_N("Users.SetOpenToRemind")));
	query.Bind(_N("table"), GetTable(), false);
	query.Bind(_N("openToRemind"), openToRemind);
	query.Bind(_N("username"), username);
	return query.Exec(false);
}

bool UserController::GetOpenToRemind(const wxString &username) {
	bool openToRemind = false;

	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Prepare(GetResourceByName(_N("Users.GetOpenToRemind")));
	query.Bind(_N("table"), GetTable(), false);
	query.Bind(_N("username"), username);
	query.Exec();
	if(query.NextRecord())
		openToRemind = query.GetField(_N("openToRemind")).ToBool();
	query.FreeResult();
	
	return openToRemind;
}
