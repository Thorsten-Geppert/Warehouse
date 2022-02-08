#include "ReferencesType.h"

ReferencesType::ReferencesType(const bool cleanUp) : DSContainer(cleanUp) {
}

void ReferencesType::Append(
	const unsigned int kind,
	const unsigned int id,
	const wxString &message
) {
	ReferenceType *newReference = new ReferenceType(kind, id, message);
	ReferenceType *oldReference = static_cast<ReferenceType *>(Add(newReference, true));
	if(newReference != oldReference)
		delete oldReference;
}
