#include "ContactPersonController.h"

ContactPersonController::ContactPersonController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("contactpersons"), // Table
	_N("contactPersonId"),  // Primary key
	_N("lastname")
) {
}
