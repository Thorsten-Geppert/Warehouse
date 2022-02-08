#include "VoucherPositionController.h"

VoucherPositionController::VoucherPositionController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("vouchers_positions"), // Table
	_N("voucherPositionId")  // Primary key
) {
}
