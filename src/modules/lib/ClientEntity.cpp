#include "ClientEntity.h"
#include <DSCurrency.h>
#include "NameType.h"
#include "AddressType.h"

ClientEntity::ClientEntity(
	RuntimeInformationType *rit
) : InformationEntity(
	rit,
	_N("clients"),
	_N("clientId")
) {
	DSCurrency discount;

	Set(_N("clientId"), 0U);
	Set(_N("formOfOrganization"), 0U, false, false, wxEmptyString, 0, false);
	Set(_N("circularLetter"));
	Set(_N("salutation"));
	Set(_N("title"));
	Set(_N("firstname"));
	Set(_N("name1"));
	Set(_N("name2"));
	Set(_N("name3"));
	Set(_N("name4"));
	Set(_N("shortName"));
	Set(_N("address"));
	Set(_N("number"));
	Set(_N("zipcode"));
	Set(_N("city"));
	Set(_N("federalState"));
	Set(_N("land"));
	Set(_N("telephone1Country"));
	Set(_N("telephone1Prefix"));
	Set(_N("telephone1Number"));
	Set(_N("telephone2Country"));
	Set(_N("telephone2Prefix"));
	Set(_N("telephone2Number"));
	Set(_N("telephone3Country"));
	Set(_N("telephone3Prefix"));
	Set(_N("telephone3Number"));
	Set(_N("fax1Country"));
	Set(_N("fax1Prefix"));
	Set(_N("fax1Number"));
	Set(_N("fax2Country"));
	Set(_N("fax2Prefix"));
	Set(_N("fax2Number"));
	Set(_N("fax3Country"));
	Set(_N("fax3Prefix"));
	Set(_N("fax3Number"));
	Set(_N("mobile1Country"));
	Set(_N("mobile1Prefix"));
	Set(_N("mobile1Number"));
	Set(_N("mobile2Country"));
	Set(_N("mobile2Prefix"));
	Set(_N("mobile2Number"));
	Set(_N("email1"));
	Set(_N("email2"));
	Set(_N("internet1"));
	Set(_N("internet2"));
	Set(_N("informations"));
	Set(_N("discount"), discount);
	Set(_N("liableToTax"), true);
	Set(_N("taxNumber"));
	Set(_N("salesTaxIdentificationNumber"));
	Set(_N("hiddenInformation"));
	Set(_N("ownClientId"));
	Set(_N("kind"));
	Set(_N("group"));
	Set(_N("sector"));
	Set(_N("birthdayDay"));
	Set(_N("birthdayMonth"));
	Set(_N("birthdayYear"));
	Set(_N("active"));
	Set(_N("condition"));
	Set(_N("monitionable"));
}

AddressEntities *ClientEntity::GetAddresses() {
	return &addresses;
}

ContactPersonEntities *ClientEntity::GetContactPersons() {
	return &contactPersons;
}

LinkedFileEntities *ClientEntity::GetLinkedFileEntities() {
	return &linkedFileEntities;
}

BankConnectionEntities *ClientEntity::GetBankConnectionEntities() {
	return &bankConnectionEntities;
}

wxString ClientEntity::GetMainData() {
	return NameType(
		Get(_N("salutation"))->ToString(),
		Get(_N("title"))->ToString(),
		Get(_N("firstname"))->ToString(),
		Get(_N("name1"))->ToString(),
		Get(_N("name2"))->ToString(),
		Get(_N("name3"))->ToString(),
		Get(_N("name4"))->ToString()
	).GetConcatedFullName(_N("\n")) +
	_N("\n\n") +
	AddressType(
		Get(_N("address"))->ToString(),
		Get(_N("number"))->ToString(),
		Get(_N("zipcode"))->ToString(),
		Get(_N("city"))->ToString(),
		Get(_N("federalState"))->ToString(),
		Get(_N("land"))->ToString()
	).GetConcatedAddress(_N("\n"));
}

wxString ClientEntity::GetName() {
	return NameType(
		Get(_N("salutation"))->ToString(),
		Get(_N("title"))->ToString(),
		Get(_N("firstname"))->ToString(),
		Get(_N("name1"))->ToString(),
		Get(_N("name2"))->ToString(),
		Get(_N("name3"))->ToString(),
		Get(_N("name4"))->ToString()
	).GetConcatedFullName(_N(", "));
}
