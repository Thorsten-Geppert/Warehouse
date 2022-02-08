#include "ContactController.h"

ContactController::ContactController(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLEntityData(
	db,
	_N("contacts"),
	_N("id")
) {
}
