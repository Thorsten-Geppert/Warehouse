#include "StockBookEntity.h"
#include <DSCurrency.h>

StockBookEntity::StockBookEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("stock_books"),
	_N("stockBookId")
) {
	DSCurrency currency;

	Set(_N("stockBookId"), 0U);
	Set(_N("stock"));
	Set(_N("product"), 0U);
	Set(_N("quantity"), currency);
	Set(_N("comment"));
	Set(_N("username"));
	Set(_N("created"), _N("NOW()"), false);
}
