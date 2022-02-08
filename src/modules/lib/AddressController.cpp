#include "AddressController.h"

AddressController::AddressController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("addresses"), // Table
	_N("addressId"),  // Primary key
	_N("name1")
) {
}
