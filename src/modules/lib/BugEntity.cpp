#include "BugEntity.h"

BugEntity::BugEntity(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLDatabaseEntity(
	db,
	_N("bugs"), 
	_N("id")
) {
	Set(_N("id"), 0);
	Set(_N("software"));
	Set(_N("version"));
	Set(_N("kind"), _N("Bug"));
	Set(_N("yourName"));
	Set(_N("email"));
	Set(_N("bug"));
	Set(_N("module"));
	Set(_N("priority"), 1);
	Set(_N("description"));
}
