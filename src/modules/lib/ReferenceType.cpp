#include "ReferenceType.h"
#include <DSCasts.h>

ReferenceType::ReferenceType(
	const unsigned int kind,
	const unsigned int id,
	const wxString &message
) : DSContainerType(
	DSCasts::ToString(kind) + _N("_") + DSCasts::ToString(id)
) {
	SetKind(kind);
	SetId(id);
	SetMessage(message);
}

void ReferenceType::SetKind(const unsigned int kind) {
	this->kind = kind;
}

unsigned int ReferenceType::GetKind() const {
	return kind;
}

void ReferenceType::SetId(const unsigned int id) {
	this->id = id;
}

unsigned int ReferenceType::GetId() const {
	return id;
}

void ReferenceType::SetMessage(const wxString &message) {
	this->message = message;
}

wxString ReferenceType::GetMessage() const {
	return message;
}
