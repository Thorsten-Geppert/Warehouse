#include "UserConfigurationFileEntity.h"

UserConfigurationFileEntity::UserConfigurationFileEntity() {
	database              = new DatabaseType;
	mainWindowPreferences = new WindowPreferencesType;
}

UserConfigurationFileEntity::~UserConfigurationFileEntity() {
	delete database;
	delete mainWindowPreferences;
}

void UserConfigurationFileEntity::SetLastEnterprise(const wxString &lastEnterprise) {
	this->lastEnterprise = lastEnterprise;
}

wxString UserConfigurationFileEntity::GetLastEnterprise() const {
	return lastEnterprise;
}

void UserConfigurationFileEntity::SetLastUsername(const wxString &lastUsername) {
	this->lastUsername = lastUsername;
}

wxString UserConfigurationFileEntity::GetLastUsername() const {
	return lastUsername;
}

void UserConfigurationFileEntity::SetMainWindowPreferences(WindowPreferencesType *mainWindowPreferences) {
	delete this->mainWindowPreferences;
	this->mainWindowPreferences = mainWindowPreferences;
}

WindowPreferencesType *UserConfigurationFileEntity::GetMainWindowPreferences() const {
	return mainWindowPreferences;
}

void UserConfigurationFileEntity::SetDatabase(DatabaseType *database) {
	delete this->database;
	this->database = database;
}

DatabaseType *UserConfigurationFileEntity::GetDatabase() const {
	return database;
}

void UserConfigurationFileEntity::SetLicenceNumber(const unsigned int licenceNumber) {
	this->licenceNumber = licenceNumber;
}

unsigned int UserConfigurationFileEntity::GetLicenceNumber() const {
	return licenceNumber;
}
