#include "PhoneType.h"

PhoneType::PhoneType() {
}

PhoneType::PhoneType(
	const wxString &country,
	const wxString &prefix,
	const wxString &number
) {
	Set(country, prefix, number);
}

void PhoneType::Set(
	const wxString &country,
	const wxString &prefix,
	const wxString &number
) {
	SetCountry(country);
	SetPrefix(prefix);
	SetNumber(number);
}

void PhoneType::SetCountry(const wxString &country){
	this->country = country;
}

void PhoneType::SetPrefix(const wxString &prefix){
	this->prefix = prefix;
}

void PhoneType::SetNumber(const wxString &number){
	this->number = number;
}

void PhoneType::Get(
	wxString *country,
	wxString *prefix,
	wxString *number
) const {
	if(country)
		*country = GetCountry();
	if(prefix)
		*prefix = GetPrefix();
	if(number)
		*number = GetNumber();
}

wxString PhoneType::GetCountry() const{
	return country;
}

wxString PhoneType::GetPrefix() const{
	return prefix;
}

wxString PhoneType::GetNumber() const{
	return number;
}

wxString PhoneType::ToString() const {
	wxString phone;

	const wxString country(GetCountry());
	if(!country.IsEmpty())
		if(country.Length() > 0 && country[0] != '+')
			phone += _N("+");
		phone += country;
	
	const wxString prefix(GetPrefix());
	if(!prefix.IsEmpty()) {
		if(!country.IsEmpty())
			phone += _N(" (0) ");
		phone += prefix;
	}

	const wxString number(GetNumber());
	if(!number.IsEmpty()) {
		if(!prefix.IsEmpty())
			phone += _N(" / ");
		phone += number;
	}

	return phone;
}
