#include "LinkedFileEntity.h"

LinkedFileEntity::LinkedFileEntity(
	RuntimeInformationType *rit,
	const wxString &table
) : Entity(
	rit,
	table,
	_N("linkedFileId")
) {
	Set(_N("linkedFileId"), 0U);
	Set(_N("fileId"), 0U);
	Set(_N("id"), 0U);
	Set(_N("description"));
}
