#ifndef BANK_CONNECTION_ENTTITY_H_
#define BANK_CONNECTION_ENTTITY_H_

#include "Entity.h"
#include "RuntimeInformationType.h"
#include <DSDateTime.h>

using namespace DynSoft;

class BankConnectionEntity : public Entity {

	public:
		BankConnectionEntity(RuntimeInformationType *rit);

		void SetReferenceDate(DSDateTime dt);
		void SetReferenceDate(wxDateTime dt);
		DSDateTime GetReferenceDate();
		wxString GetReferenceStateString();
		wxString GetReferenceAccountString();
		wxString GetNotesInOneLine(const wxString &replace = _N(" - "));

};

#endif /* BANK_CONNECTION_ENTTITY_H_ */
