#include "UpdateType.h"

UpdateType::UpdateType() {
}

UpdateType::UpdateType(
	const wxString &version,
	const wxString &information
) {
	SetVersion(version);
	SetInformation(information);
}

void UpdateType::SetVersion(const wxString &version) {
	this->version = version;
}

wxString UpdateType::GetVersion() const {
	return version;
}

void UpdateType::SetInformation(const wxString &information) {
	this->information = information;
}

wxString UpdateType::GetInformation() const {
	return information;
}
