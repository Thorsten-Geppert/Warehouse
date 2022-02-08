#include "SqlUpdateType.h"

SqlUpdateType::SqlUpdateType() {
}

SqlUpdateType::SqlUpdateType(
	wxArrayString &versions,
	const wxString &description,
	const wxString &sql
) {
	SetVersions(versions);
	SetDescription(description);
	SetSql(sql);
}

SqlUpdateType::SqlUpdateType(
	const wxString &version,
	const wxString &description,
	const wxString &sql
) {
	versions.Add(version);
	SetDescription(description);
	SetSql(sql);
}

bool SqlUpdateType::ExistsVersion(const wxString &version) {
	return versions.Index(version) != wxNOT_FOUND || versions.Index(_N("all")) != wxNOT_FOUND;
}

unsigned int SqlUpdateType::GetVersionCount() const {
	return versions.GetCount();
}

wxString SqlUpdateType::GetVersions(const wxString &delimiter) const {
	wxString versions;
	const unsigned int count = GetVersionCount();
	for(unsigned int i = 0; i < count; i++) {
		versions += GetVersion(i);
		if(i + 1 < count)
			versions += delimiter;
	}
	return versions;
}

void SqlUpdateType::SetVersions(const wxArrayString &versions) {
	this->versions = versions;
}

void SqlUpdateType::ClearVersions() {
	versions.Clear();
}

void SqlUpdateType::AddVersion(const wxString &version) {
	versions.Add(version);
}

wxString SqlUpdateType::GetVersion(const unsigned int number) const {
	if(number < GetVersionCount())
		return versions[number];
	return wxEmptyString;
}

void SqlUpdateType::SetDescription(const wxString &description) {
	this->description = description;
}

wxString SqlUpdateType::GetDescription() const {
	return description;
}

void SqlUpdateType::SetSql(const wxString &sql) {
	this->sql = sql;
}

wxString SqlUpdateType::GetSql() const {
	return sql;
}

wxString SqlUpdateType::ToString() {
	wxString version;
	const unsigned int count = GetVersionCount();
	for(unsigned int i = 0; i < count; i++) {
		version += GetVersion(i);
		if(i + 1 < count)
			version += _N(", ");
	}

	return
		_N("versions {\n\t") + version + _N("\n}\n") +
		_N("description {\n\t") + GetDescription() + _N("\n}\n") +
		_N("sql {\n\t") + GetSql() + _N("\n}\n");
}
