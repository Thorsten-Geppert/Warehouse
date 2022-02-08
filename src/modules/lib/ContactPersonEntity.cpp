#include "ContactPersonEntity.h"

ContactPersonEntity::ContactPersonEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("contactpersons"),
	_N("contactPersonId")
) {
	Set(_N("contactPersonId"), 0U);
	Set(_N("clientId"), 0U);
	Set(_N("function"));
	Set(_N("department"));
	Set(_N("salutation"));
	Set(_N("title"));
	Set(_N("firstname"));
	Set(_N("lastname"));
	Set(_N("telephoneCountry"));
	Set(_N("telephonePrefix"));
	Set(_N("telephoneNumber"));
	Set(_N("mobileCountry"));
	Set(_N("mobilePrefix"));
	Set(_N("mobileNumber"));
	Set(_N("faxCountry"));
	Set(_N("faxPrefix"));
	Set(_N("faxNumber"));
	Set(_N("email"));
	Set(_N("internet"));
	Set(_N("information"));
}
