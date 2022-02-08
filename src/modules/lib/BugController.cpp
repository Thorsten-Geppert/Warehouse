#include "BugController.h"

BugController::BugController(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLEntityData(
	db,
	_N("bugs"),
	_N("id")
) {
}
