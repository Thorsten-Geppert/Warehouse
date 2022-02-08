#include "ProductEntity.h"
#include <DSCurrency.h>

ProductEntity::ProductEntity(
	RuntimeInformationType *rit
) : InformationEntity(
	rit,
	_N("products"),
	_N("productId")
) {
	DSCurrency price;

	Set(_N("productId"), 0U);
	Set(_N("articleNumber"));
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
	Set(_N("priceWithoutTax"), price);
	Set(_N("purchasePriceWithTax"), price);
	Set(_N("purchasePriceWithoutTax"), price);
	Set(_N("priceWithTax"), price);
	Set(_N("tax"), 0U, false, false, wxEmptyString, 0, false);
	Set(_N("weight"), 0.0);
}

LinkedFileEntities *ProductEntity::GetLinkedFileEntities() {
	return &linkedFileEntities;
}
