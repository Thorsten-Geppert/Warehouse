#include "Entity.h"

Entity::Entity(
	RuntimeInformationType *rit,
	const wxString &name,
	const wxString &primaryKey,
	const bool getAutoSequence,
	const wxString &foreignKey,
	const wxString &foreignTable,
	const unsigned int reference
) : Moose(
	rit
), DSPostgreSQLDatabaseEntity(
	rit->GetDatabase(),
	name,
	primaryKey,
	getAutoSequence,
	foreignKey,
	foreignTable,
	reference
) {
	SetRuntimeInformationType(rit);
}

KeysValuesType *Entity::GetKeysValuesType() {
	return &keysValuesType;
}
