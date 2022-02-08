#include "OnlineContactEntity.h"

OnlineContactEntity::OnlineContactEntity(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLDatabaseEntity(
	db,
	_N("online_contact_messages"),
	_N("id")
) {
	Set(_N("id"), 0);
	Set(_N("salutation"));
	Set(_N("title"));
	Set(_N("firstname"));
	Set(_N("lastname"));
	Set(_N("company"));
	Set(_N("street"));
	Set(_N("zipcode"));
	Set(_N("city"));
	Set(_N("telephone"));
	Set(_N("fax"));
	Set(_N("email"));
	Set(_N("internet"));
	Set(_N("subject"));
	Set(_N("message"));
}
