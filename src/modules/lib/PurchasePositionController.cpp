#include "PurchasePositionController.h"

PurchasePositionController::PurchasePositionController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("purchases_positions"), // Table
	_N("purchasePositionId")  // Primary key
) {
}
