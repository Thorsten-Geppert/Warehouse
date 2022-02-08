#include "MonitionTextEntity.h"

MonitionTextEntity::MonitionTextEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("monition_texts"),
	_N("number"),
	false
) {
	Set(_N("number"), 0U);
	Set(_N("prepend"));
	Set(_N("append"));
}
