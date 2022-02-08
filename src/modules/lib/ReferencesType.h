#ifndef REFERENCES_TYPE_H_
#define REFERENCES_TYPE_H_

#include <DSContainer.h>
#include "ReferenceType.h"

using namespace DynSoft;

class ReferencesType : public DSContainer {

	public:
		ReferencesType(const bool cleanUp = true);
		void Append(
			const unsigned int kind,
			const unsigned int id,
			const wxString &message
		);


};

#endif /* REFERENCES_TYPE_H_ */
