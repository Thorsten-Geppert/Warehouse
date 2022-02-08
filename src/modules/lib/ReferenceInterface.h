#ifndef REFERENCE_INTERFACE_H_
#define REFERENCE_INTERFACE_H_

#include "ReferencesType.h"
#include "RuntimeInformationType.h"

class ReferenceInterface {

	public:
		virtual unsigned int GetReferences(
			const unsigned int kind,
			const unsigned int primaryKey,
			ReferencesType *referencesType
		) = 0;

};

#endif /* REFERENCE_INTERFACE_H_ */
