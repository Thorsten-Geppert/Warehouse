#ifndef INFORMATION_ENTTITY_H_
#define INFORMATION_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class InformationEntity : public Entity {

	public:
		InformationEntity(
			RuntimeInformationType *rit,
			const wxString &name,
			const wxString &primaryKey   = wxEmptyString,
			const bool getAutoSequence   = true,
			const wxString &foreignKey   = wxEmptyString,
			const wxString &foreignTable = wxEmptyString,
			const unsigned int reference = REFERENCE_NO
		);

		void ForceChanged();

		wxArrayString *GetInsertExcludes();
		wxArrayString *GetUpdateExcludes();

	protected:
		wxArrayString excludeInsert, excludeUpdate;

};

#endif /* INFORMATION_ENTTITY_H_ */
