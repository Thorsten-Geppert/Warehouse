#ifndef LINKED_FILE_CONTROLLER_H_
#define LINKED_FILE_CONTROLLER_H_

#include "Controller.h"
#include <DSPostgreSQLQuery.h>
#include "RuntimeInformationType.h"
#include "LinkedFileEntities.h"
#include "HtmlPrintoutLibrary.h"

using namespace DynSoft;

class LinkedFileController : public Controller {

	public:
		LinkedFileController(RuntimeInformationType *rit, const wxString &table);
		
		bool Save(
			DSPostgreSQLQuery &query,
			const unsigned int id,
			LinkedFileEntities &linkedFileEntities
		);

		unsigned int Load(
			const unsigned int id,
			LinkedFileEntities &linkedFileEntities
		);

		wxString Print(HtmlPrintoutLibrary &htmlPrintoutLibrary, LinkedFileEntities &linkedFileEntities);

};

#endif /* LINKED_FILE_CONTROLLER_H_ */
