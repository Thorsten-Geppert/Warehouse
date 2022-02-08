#include "PgHbaHostType.h"
#include <DSLib.h>

PgHbaHostType::PgHbaHostType() {
}

PgHbaHostType::PgHbaHostType(
	const wxString &type,
	const wxString &database,
	const wxString &user,
	const wxString &address,
	const wxString &subnet,
	const wxString &method
) {
	SetType(type);
	SetDatabase(database);
	SetUser(user);
	SetAddress(address);
	SetSubnet(subnet);
	SetMethod(method);
}

void PgHbaHostType::SetType(const wxString &type) {
	this->type = type;
}

wxString PgHbaHostType::GetType() const {
	return type;
}

void PgHbaHostType::SetDatabase(const wxString &database) {
	this->database = database;
}

wxString PgHbaHostType::GetDatabase() const {
	return database;
}

void PgHbaHostType::SetUser(const wxString &user) {
	this->user = user;
}

wxString PgHbaHostType::GetUser() const {
	return user;
}

void PgHbaHostType::SetAddress(const wxString &address) {
	this->address = address;
}

wxString PgHbaHostType::GetAddress() const {
	return address;
}

void PgHbaHostType::SetSubnet(const wxString &subnet) {
	this->subnet = subnet;
}

wxString PgHbaHostType::GetSubnet() const {
	return subnet;
}

void PgHbaHostType::SetMethod(const wxString &method) {
	this->method = method;
}

wxString PgHbaHostType::GetMethod() const {
	return method;
}

wxString PgHbaHostType::ToLine() {
	wxString tmp;
	if(GetSubnet().Find(_N(".")) == wxNOT_FOUND)
		tmp = GetAddress() + _N("/") + GetSubnet();
	else
		tmp = GetAddress() + _N(" ") + GetSubnet();
	
	return
		GetType() + _N(" ") +
		GetDatabase() + _N(" ") +
		GetUser() + _N(" ") +
		tmp + _N(" ") +
		GetMethod();
}
