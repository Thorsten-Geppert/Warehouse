#include "VoucherPositionEntity.h"

VoucherPositionEntity::VoucherPositionEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("vouchers_positions"),
	_N("voucherPositionId")
) {
	DSCurrency price;

	Set(_N("voucherPositionId"), 0U);
	Set(_N("voucherId"), 0U);
	Set(_N("quantity"), 1);
	Set(_N("description"));
	Set(_N("articleNumber"));
	Set(_N("article"));
	Set(_N("tax"), 0U);
	Set(_N("taxPrice"), price);
	Set(_N("price"), price);
}
