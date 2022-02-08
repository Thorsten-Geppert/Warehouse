#include "EnterprisePreferencesEntity.h"

EnterprisePreferencesEntity::EnterprisePreferencesEntity(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLDatabaseEntity(
	db,
	_N("enterprise"),
	_N("id")
) {
	Set(_N("id"), 0);
	Set(_N("company"));
	Set(_N("executive"));
	Set(_N("address1"));
	Set(_N("address2"));
	Set(_N("zipcode"));
	Set(_N("city"));
	Set(_N("salesTaxIdentificationNumber"));
	Set(_N("taxNumber"));
	Set(_N("salesTaxIndicator"));
	Set(_N("telephone"));
	Set(_N("fax"));
	Set(_N("email"));
	Set(_N("web"));
	Set(_N("bank"));
	Set(_N("bankCode"));
	Set(_N("holder"));
	Set(_N("accountNumber"));
	Set(_N("bic"));
	Set(_N("iban"));
	Set(_N("mandatoryProductsArticleNumber"));
	Set(_N("mandatoryClientsOwnClientId"));
}

bool EnterprisePreferencesEntity::IsComplete() {
	return
		!Get(_N("company"))->ToString().IsEmpty() &&
		!Get(_N("executive"))->ToString().IsEmpty() &&
		!Get(_N("address1"))->ToString().IsEmpty() &&
		!Get(_N("zipcode"))->ToString().IsEmpty() &&
		!Get(_N("city"))->ToString().IsEmpty() &&
		!Get(_N("telephone"))->ToString().IsEmpty() &&
		!Get(_N("fax"))->ToString().IsEmpty() &&
		!Get(_N("email"))->ToString().IsEmpty() &&
		!Get(_N("web"))->ToString().IsEmpty() &&
		!Get(_N("bank"))->ToString().IsEmpty() &&
		!Get(_N("holder"))->ToString().IsEmpty() &&
		!Get(_N("bic"))->ToString().IsEmpty() &&
		!Get(_N("iban"))->ToString().IsEmpty()
	;
}

wxString EnterprisePreferencesEntity::AsPostAddress() {
	wxArrayString addressArray;

	addressArray.Add(Get(_N("company"))->ToString());
	if(!Get(_N("executive"))->ToString().IsEmpty())
		addressArray.Add(Get(_N("executive"))->ToString());
	addressArray.Add(Get(_N("address1"))->ToString());
	addressArray.Add(Get(_N("address2"))->ToString());
	addressArray.Add(Get(_N("zipcode"))->ToString() + _N(" ") + Get(_N("city"))->ToString());

	wxString address;
	const unsigned int count = addressArray.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		if(!addressArray[i].IsEmpty()) {
			address += addressArray[i];
			if(i + 1 < count)
				address += _N("\n");
		}
	}
	
	return address;
}
