#ifndef NOTE_CONTROLLER_H_
#define NOTE_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "NoteEntity.h"
#include "HtmlPrintoutLibrary.h"

using namespace DynSoft;

class NoteController : public Controller {

	public:
		NoteController(
			RuntimeInformationType *rit
		);
		HtmlPrintoutLibrary *Print(NoteEntity *noteEntity);

};

#endif /* NOTE_CONTROLLER_H_ */
