#include "DatabaseType.h"
#include <DSLib.h>

void DatabaseType::SetHostname(const wxString &hostname) {
	this->hostname = hostname;
}

wxString DatabaseType::GetHostname() const {
	return hostname;
}

void DatabaseType::SetDatabase(const wxString &database) {
	this->database = database;
}

wxString DatabaseType::GetDatabase() const {
	return database;
}

void DatabaseType::SetUsername(const wxString &username) {
	this->username = username;
}

wxString DatabaseType::GetUsername() const {
	return username;
}

void DatabaseType::SetPassword(const wxString &password) {
	this->password = password;
}

wxString DatabaseType::GetPassword() const {
	return password;
}

void DatabaseType::SetPort(const wxString &port) {
	this->port = port;
}

wxString DatabaseType::GetPort() const {
	return port;
}

wxString DatabaseType::ToString() {
	return
		GetUsername() +
		_N(":") +
		GetPassword() +
		_N("@") +
		GetHostname() +
		_N(":") +
		GetPort() +
		_N(":/") +
		GetDatabase()
	;
}
