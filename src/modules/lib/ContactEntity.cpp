#include "ContactEntity.h"

ContactEntity::ContactEntity(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLDatabaseEntity(
	db,
	_N("contacts"),
	_N("id")
) {
	Set(_N("id"), 0);
	Set(_N("software"));
	Set(_N("version"));
	Set(_N("kind"), _N("Contact"));
	Set(_N("yourName"));
	Set(_N("email"));
	Set(_N("subject"));
	Set(_N("message"));
}
