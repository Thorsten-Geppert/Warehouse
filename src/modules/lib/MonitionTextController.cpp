#include "MonitionTextController.h"

MonitionTextController::MonitionTextController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("monition_texts"), // Table
	_N("number"),
	_N("number")
) {
//	LoadResource(rit->GRP(_N("sql"), _N("MonitionTexts.res")));
}

MonitionTextEntity *MonitionTextController::GetDirect(const int number, const bool createNewInstanceIfNotFound) {
	MonitionTextEntity *monitionTextEntity = NULL;

	MonitionTextController monitionController(rit);
	monitionController.Get(DSCasts::ToString(number));
	if(monitionController.Run()) {
		monitionTextEntity = new MonitionTextEntity(rit);
		monitionController.Get(monitionTextEntity);
		monitionController.Reset();
	}

	if(!monitionTextEntity && createNewInstanceIfNotFound) {
		monitionTextEntity = new MonitionTextEntity(rit);
		monitionTextEntity->SetValue(_N("number"), number);
	}

	return monitionTextEntity;
}
