#include "EnterpriseController.h"

EnterpriseController::EnterpriseController(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLEntityData(
	db,
	_N("enterprises"),
	_N("id"),
	_N("company")
) {
}
