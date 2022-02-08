#include "InformationEntity.h"

InformationEntity::InformationEntity(
	RuntimeInformationType *rit,
	const wxString &name,
	const wxString &primaryKey,
	const bool getAutoSequence,
	const wxString &foreignKey,
	const wxString &foreignTable,
	const unsigned int reference
) : Entity(
	rit,
	name,
	primaryKey,
	getAutoSequence,
	foreignKey,
	foreignTable,
	reference
) {
	//excludeInsert.Add(_N("createdAt"));
	excludeInsert.Add(_N("changedAt"));

	excludeUpdate.Add(_N("createdAt"));
	excludeUpdate.Add(_N("createdBy"));

	Set(_N("createdBy"), rit->GetUserConfigurationFileEntity()->GetLastUsername());
	Set(_N("createdAt"), _N("NOW()"), false);
	Set(_N("changedBy"), rit->GetUserConfigurationFileEntity()->GetLastUsername());
	Set(_N("changedAt"), _N("NOW()"), false);
}

void InformationEntity::ForceChanged() {
	Set(_N("changedBy"), rit->GetUserConfigurationFileEntity()->GetLastUsername());
	Set(_N("changedAt"), _N("NOW()"), false); 
}

wxArrayString *InformationEntity::GetInsertExcludes() {
	return &excludeInsert;
}

wxArrayString *InformationEntity::GetUpdateExcludes() {
	return &excludeUpdate;
}
