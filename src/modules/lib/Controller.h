#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include <DSPostgreSQLEntityData.h>
#include <DSPostgreSQLQuery.h>
#include "Moose.h"
#include "RuntimeInformationType.h"
#include "KeysValuesType.h"
#include "HtmlPrintoutLibrary.h"

using namespace DynSoft;

class Controller : public Moose, public DSPostgreSQLEntityData {

	public:
		Controller(
			RuntimeInformationType *rit,
			const wxString &table        = wxEmptyString,
			const wxString &primaryKey   = wxEmptyString,
			const wxString &orderByField = wxEmptyString,
			const unsigned int order     = ASC
		);

		bool SaveKeysValues(
			DSPostgreSQLQuery &query,
			const unsigned int id,
			const wxString &table,
			KeysValuesType &keysValuesType
		);

		unsigned int LoadKeysValues(
			const unsigned int id,
			const wxString &table,
			KeysValuesType &keysValuesType
		);

		wxString PrintKeysValues(HtmlPrintoutLibrary &htmlPrintoutLibrary, KeysValuesType &keysValuesType);

	protected:
		wxString WildCard(const wxString &value, const bool NullIsEmpty = false);

};

#endif /* CLIENT_CONTROLLER_H_ */
