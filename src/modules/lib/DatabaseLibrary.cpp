#include "DatabaseLibrary.h"
#include <DSPostgreSQLQuery.h>

using namespace DynSoft;

DatabaseLibrary::DatabaseLibrary() : DSType() {
	this->rit = NULL;
}

DatabaseLibrary::DatabaseLibrary(RuntimeInformationType *rit) : DSType() {
	this->rit = rit;
}

unsigned int DatabaseLibrary::CreateDatabase(DatabaseType &databaseType, const wxString &sql) {
	DSPostgreSQLDatabase createDb;

	createDb.SetHostname(databaseType.GetHostname());
	createDb.SetDatabase(_N("postgres"));
	createDb.SetUsername(databaseType.GetUsername());
	createDb.SetPassword(databaseType.GetPassword());
	createDb.SetPort(databaseType.GetPort());

	if(!createDb.Connect()) {
		createDb.SetDatabase(_N("template1"));
		if(!createDb.Connect())
			createDb.SetDatabase(_N("template0"));
		if(!createDb.Connect())
			return DATABASE_LIBRARY_ERROR_CONNECTION;
	}

	if(DatabaseExists(&createDb, databaseType.GetDatabase())) {
		createDb.Disconnect();
		return DATABASE_LIBRARY_ERROR_EXISTS;
	}

	unsigned int result = DATABASE_LIBRARY_OK;

	DSPostgreSQLQuery createQuery(&createDb);

	createQuery.Prepare(_N(
		"CREATE DATABASE "
		"	\":database:\" "
		"OWNER "
		"	\":owner:\" "
		"ENCODING "
		"	'unicode' "
		"TEMPLATE "
		"	\"template0\""
	));
	createQuery.Bind(_N("database"), databaseType.GetDatabase(), false);
	createQuery.Bind(_N("owner"), databaseType.GetUsername(), false);
	if(!createQuery.Exec(false))
		result = DATABASE_LIBRARY_ERROR_CREATION;

	createDb.Disconnect();

	wxSleep(2);

	DSPostgreSQLDatabase insertDb;

	insertDb.SetHostname(databaseType.GetHostname());
	insertDb.SetDatabase(databaseType.GetDatabase());
	insertDb.SetUsername(databaseType.GetUsername());
	insertDb.SetPassword(databaseType.GetPassword());
	insertDb.SetPort(databaseType.GetPort());

	if(!insertDb.Connect())
		return DATABASE_LIBRARY_ERROR_CONNECT;

	DSPostgreSQLQuery insertQuery(&insertDb);
	if(!sql.IsEmpty() && result == DATABASE_LIBRARY_OK) {
		if(!insertQuery.Query(sql, false))
			result = DATABASE_LIBRARY_ERROR_INSERT;
	}
	
	insertDb.Disconnect();

	return result;
}

unsigned int DatabaseLibrary::DropDatabase(DatabaseType &databaseType) {
	DSPostgreSQLDatabase dropDb;

	dropDb.SetHostname(databaseType.GetHostname());
	dropDb.SetDatabase(_N("postgres"));
	dropDb.SetUsername(databaseType.GetUsername());
	dropDb.SetPassword(databaseType.GetPassword());
	dropDb.SetPort(databaseType.GetPort());

	if(!dropDb.Connect()) {
		dropDb.SetDatabase(_N("template1"));
		if(!dropDb.Connect())
			return DATABASE_LIBRARY_ERROR_CONNECTION;
	}

	if(!DatabaseExists(&dropDb, databaseType.GetDatabase())) {
		dropDb.Disconnect();
		return DATABASE_LIBRARY_ERROR_EXISTS;
	}

	unsigned int result = DATABASE_LIBRARY_OK;

	DSPostgreSQLQuery dropQuery(&dropDb);

	dropQuery.Prepare(_N("DROP DATABASE \":database:\""));
	dropQuery.Bind(_N("database"), databaseType.GetDatabase(), false);
	if(!dropQuery.Exec(false))
		result = DATABASE_LIBRARY_ERROR_DELETION;

	dropDb.Disconnect();

	return result;
}

bool DatabaseLibrary::DatabaseExists(DSPostgreSQLDatabase *db, const wxString &name) {
	DSPostgreSQLQuery query(db);
	query.Prepare(_N(
		"SELECT "
		"	COUNT(pg_database.datname) AS counter "
		"FROM "
		"	pg_database "
		"WHERE "
		"	pg_database.datname = :name: "
	));
	query.Bind(_N("name"), name);
	query.Exec();
	query.NextRecord();
	const bool exists = query.GetField(_N("counter")).ToUnsignedInt() > 0;
	query.FreeResult();
	return exists;
}

wxString DatabaseLibrary::BuildDatabaseName(const wxString &fullName) {
	wxString database;
	wxString name(fullName.Lower());
	wxChar c;
	const unsigned int length = name.Length();
	for(unsigned int i = 0; i < length; i++) {
		c = name[i];
		if(c == ' ')
			database.Append(_N("_"));
		else if((c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || c == '_')
			database.Append(c);
	}

	return database;
}

unsigned int DatabaseLibrary::ReorganizeDatabase(DSPostgreSQLDatabase *db, DatabaseType &databaseType) {
	DSPostgreSQLDatabase reorgDb;

	reorgDb.SetHostname(databaseType.GetHostname());
	reorgDb.SetDatabase(databaseType.GetDatabase());
	reorgDb.SetUsername(databaseType.GetUsername());
	reorgDb.SetPassword(databaseType.GetPassword());
	reorgDb.SetPort(databaseType.GetPort());

	if(!DatabaseExists(db, databaseType.GetDatabase())) {
		reorgDb.Disconnect();
		return DATABASE_LIBRARY_ERROR_EXISTS;
	}

	if(!reorgDb.Connect())
		return DATABASE_LIBRARY_ERROR_CONNECTION;

	unsigned int result = DATABASE_LIBRARY_OK;

	DSPostgreSQLQuery reorgQuery(&reorgDb);

	if(!reorgQuery.Query(_N("VACUUM FULL ANALYZE"), false))
		result = DATABASE_LIBRARY_ERROR_VACUUM;

	reorgDb.Disconnect();

	return result;
}

bool DatabaseLibrary::ReorganizeDatabase(DSPostgreSQLDatabase &db) {
	if(!db.IsConnected())
		return false;
	
	DSPostgreSQLQuery query(&db);
	return query.Query(_N("VACUUM FULL ANALYZE"), false);
}
