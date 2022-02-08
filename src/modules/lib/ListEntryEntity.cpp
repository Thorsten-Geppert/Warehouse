#include "ListEntryEntity.h"

ListEntryEntity::ListEntryEntity(
	RuntimeInformationType *rit,
	const wxString &table,
	const wxString &field
) : Entity(
	rit,
	table,
	field,
	false
) {
	pk = field;
	Set(pk);
}
