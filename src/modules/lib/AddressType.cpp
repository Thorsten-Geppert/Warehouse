#include "AddressType.h"

AddressType::AddressType() {
}

AddressType::AddressType(
	const wxString &address,
	const wxString &number,
	const wxString &zipcode,
	const wxString &city,
	const wxString &federalState,
	const wxString &land
) {
	Set(address, number, zipcode, city, federalState, land);
}

void AddressType::Set(
	const wxString &address,
	const wxString &number,
	const wxString &zipcode,
	const wxString &city,
	const wxString &federalState,
	const wxString &land
) {
	SetAddress(address);
	SetNumber(number);
	SetZipcode(zipcode);
	SetCity(city);
	SetFederalState(federalState);
	SetLand(land);
}

void AddressType::SetAddress(const wxString &address) {
	this->address = address;
}

void AddressType::SetNumber(const wxString &number) {
	this->number = number;
}

void AddressType::SetZipcode(const wxString &zipcode) {
	this->zipcode = zipcode;
}

void AddressType::SetCity(const wxString &city) {
	this->city = city;
}

void AddressType::SetFederalState(const wxString &federalState) {
	this->federalState = federalState;
}

void AddressType::SetLand(const wxString &land) {
	this->land = land;
}

void AddressType::Get(
	wxString *address,
	wxString *number,
	wxString *zipcode,
	wxString *city,
	wxString *federalState,
	wxString *land
) {
	if(address)
		*address = GetAddress();
	if(number)
		*number = GetNumber();
	if(zipcode)
		*zipcode = GetZipcode();
	if(city)
		*city = GetCity();
	if(federalState)
		*federalState = GetFederalState();
	if(land)
		*land = GetLand();
}

wxString AddressType::GetAddress() const {
	return address;
}

wxString AddressType::GetNumber() const {
	return number;
}

wxString AddressType::GetZipcode() const {
	return zipcode;
}

wxString AddressType::GetCity() const {
	return city;
}

wxString AddressType::GetFederalState() const {
	return federalState;
}

wxString AddressType::GetLand() const {
	return land;
}

wxString AddressType::GetAddressAndNumber(const wxString &address, const wxString &number) {
	if(!address.IsEmpty() && !number.IsEmpty())
		return address + _N(" ") + number;
	else if(address.IsEmpty() && !number.IsEmpty())
		return number;
	else if(!address.IsEmpty() && number.IsEmpty())
		return address;
	
	return wxEmptyString;
}

wxString AddressType::GetAddressAndNumber() const {
	return GetAddressAndNumber(GetAddress(), GetNumber());
}

wxString AddressType::GetConcatedAddress(const wxString &separator, const bool withFederalState, const bool withLand) const {
	wxString tmp(GetAddressAndNumber());

	if(!GetZipcode().IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += separator;
		tmp += GetZipcode();
	}

	if(!GetCity().IsEmpty()) {
		if(!tmp.IsEmpty() && GetZipcode().IsEmpty())
			tmp += separator;
		if(!GetZipcode().IsEmpty())
			tmp += _N(" ");
		tmp += GetCity();
	}

	if(withFederalState && !GetFederalState().IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += separator;
		tmp += GetFederalState();
	}

	if(withLand && !GetLand().IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += separator;
		tmp += GetLand();
	}

	return tmp;
}
