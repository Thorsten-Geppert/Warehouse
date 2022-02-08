#include "AddressEntity.h"
#include "ClientEntity.h"

AddressEntity::AddressEntity(
	RuntimeInformationType *rit,
	ClientEntity *clientEntity
) : Entity(
	rit,
	_N("addresses"),
	_N("addressId")
) {
	Set(_N("addressId"), 0U);
	Set(_N("clientId"), 0U);
	Set(_N("description"));
	Set(_N("salutation"));
	Set(_N("title"));
	Set(_N("firstname"));
	Set(_N("name1"));
	Set(_N("name2"));
	Set(_N("name3"));
	Set(_N("name4"));
	Set(_N("address"));
	Set(_N("number"));
	Set(_N("zipcode"));
	Set(_N("city"));
	Set(_N("federalState"));
	Set(_N("land"));
	Set(_N("information"));

	if(clientEntity) {
		SetValue(_N("salutation"), clientEntity->Get(_N("salutation"))->ToString());
		SetValue(_N("title"), clientEntity->Get(_N("title"))->ToString());
		SetValue(_N("firstname"), clientEntity->Get(_N("firstname"))->ToString());
		SetValue(_N("name1"), clientEntity->Get(_N("name1"))->ToString());
		SetValue(_N("name2"), clientEntity->Get(_N("name2"))->ToString());
		SetValue(_N("name3"), clientEntity->Get(_N("name3"))->ToString());
		SetValue(_N("name4"), clientEntity->Get(_N("name4"))->ToString());
		SetValue(_N("address"), clientEntity->Get(_N("address"))->ToString());
		SetValue(_N("number"), clientEntity->Get(_N("number"))->ToString());
		SetValue(_N("zipcode"), clientEntity->Get(_N("zipcode"))->ToString());
		SetValue(_N("city"), clientEntity->Get(_N("city"))->ToString());
		SetValue(_N("federalState"), clientEntity->Get(_N("federalState"))->ToString());
		SetValue(_N("land"), clientEntity->Get(_N("land"))->ToString());
	}
}
