#ifndef ENTTITY_H_
#define ENTTITY_H_

#include <DSPostgreSQLDatabaseEntity.h>
#include "Moose.h"
#include "RuntimeInformationType.h"
#include "KeysValuesType.h"

using namespace DynSoft;

class Entity : public Moose, public DSPostgreSQLDatabaseEntity {

	public:
		Entity(
			RuntimeInformationType *rit,
			const wxString &name,
			const wxString &primaryKey   = wxEmptyString,
			const bool getAutoSequence   = true,
			const wxString &foreignKey   = wxEmptyString,
			const wxString &foreignTable = wxEmptyString,
			const unsigned int reference = REFERENCE_NO
		);

		KeysValuesType *GetKeysValuesType();
	
	protected:	
		KeysValuesType keysValuesType;

};

#endif /* INFORMATION_ENTTITY_H_ */
