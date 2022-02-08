#include "UserEntity.h"

UserEntity::UserEntity(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLDatabaseEntity(
	db,
	_N("users"),
	_N("username")
) {
	Set(_N("username"));
	Set(_N("password"));
	Set(_N("realName"));
	Set(_N("email"));
	Set(_N("description"));
	Set(_N("status"), 0);
	Set(_N("openToRemind"), false);
}
