#include "NoteEntity.h"

NoteEntity::NoteEntity(
	RuntimeInformationType *rit
) : InformationEntity(
	rit,
	_N("notes"), // Table
	_N("noteId")  // Primary
) {
	Set(_N("noteId"), 0U);
	Set(_N("title"));
	Set(_N("note"));
}
