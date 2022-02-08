#ifndef SEND_CONTROLLER_H_
#define SEND_CONTROLLER_H_

#include <wx/wx.h>
#include <DSEntity.h>

using namespace DynSoft;

class SendController {

	public:
		static const unsigned int SEND               = 0;
		static const unsigned int ENTITY_IS_NULL     = 1;
		static const unsigned int UNKNOWN_ERROR      = 2;
		static const unsigned int NO_CONNECTION      = 3;
		static const unsigned int INCORRECT_RESPONSE = 4;
		static const unsigned int SERVER_ERROR       = 5;

		unsigned int Send(
			const wxString &contactUrl,
			const wxString &contactFile,
			DSEntity *entity,
			wxString *reply = NULL
		);

};

#endif /* SEND_CONTROLLER_H_ */
