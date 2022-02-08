#include "EnterpriseEntity.h"

EnterpriseEntity::EnterpriseEntity(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLDatabaseEntity(
	db,
	_N("enterprises")
) {
	Set(_N("id"), 0);
	Set(_N("company"));
	Set(_N("hostname"));
	Set(_N("database"));
	Set(_N("username"));
	Set(_N("password"));
	Set(_N("port"), 5432);
	SetPrimaryKey(_N("id"));
}
