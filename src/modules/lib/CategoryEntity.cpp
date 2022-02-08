#include "CategoryEntity.h"

CategoryEntity::CategoryEntity(
	RuntimeInformationType *rit,
	const wxString &table
) : Entity(
	rit,
	table,
	_N("categoryId")
) {
	Set(_N("categoryId"), 0U);
	Set(_N("parentId"), 0U);
	Set(_N("name"));
}
