#include "OrderPositionController.h"

OrderPositionController::OrderPositionController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("orders_positions"), // Table
	_N("orderPositionId"),  // Primary key
	_N("rank")  // Sorting
) {
}
