#ifndef LISTENTRY_CONTROLLER_H_
#define LISTENTRY_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class wxComboBox;
class ListEntryController : public Controller {

	public:
		static const unsigned int FILL_NEW    = 0;
		static const unsigned int FILL_APPEND = 1;
		static const unsigned int FILL_FILL   = 2;

	public:
		ListEntryController(
			RuntimeInformationType *rit,
			const wxString &table,
			const wxString &field
		);

		wxArrayString ToArrayString();
		static unsigned int FillComboBox(
			RuntimeInformationType *rit,
			wxComboBox *comboBox,
			const wxString &table,
			const wxString &field,
			const unsigned int fillMethod = FILL_NEW,
			const bool prependEmpty       = false
		);
	
	protected:
		wxString pk;

};

#endif /* LISTENTRY_CONTROLLER_H_ */
