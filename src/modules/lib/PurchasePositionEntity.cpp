#include "PurchasePositionEntity.h"

PurchasePositionEntity::PurchasePositionEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("purchases_positions"),
	_N("purchasePositionId")
) {
	DSCurrency price;

	Set(_N("purchasePositionId"), 0U);
	Set(_N("purchaseId"), 0U);
	Set(_N("amount"), 0.0);
	Set(_N("productId"), 0U);
	Set(_N("articleNumber"));
	Set(_N("text"));
	Set(_N("price"), price);
	Set(_N("fullPrice"), price);
	Set(_N("group"));
	Set(_N("package"));
	Set(_N("available"), true);
	Set(_N("distributorId"), 0U);
	Set(_N("manufacturer"));
	Set(_N("publisher"));
	Set(_N("ean"));
	Set(_N("shortName"));
	Set(_N("name1"));
	Set(_N("name2"));
	Set(_N("name3"));
	Set(_N("name4"));
	Set(_N("description"));
	Set(_N("note"));
	Set(_N("isbn10"));
	Set(_N("isbn13"));
	Set(_N("weight"), 0.0);
	Set(_N("priceWithoutTax"), price);
	Set(_N("priceWithTax"), price);
	Set(_N("purchasePriceWithoutTax"), price);
	Set(_N("purchasePriceWithTax"), price);
	Set(_N("stock"));
	Set(_N("bookedOut"), false);
}

void PurchasePositionEntity::Recalc() {
	DSCurrency fullPrice = Get(_N("amount"))->ToCurrency() * Get(_N("price"))->ToCurrency();
	SetValue(_N("fullPrice"), fullPrice);
}
