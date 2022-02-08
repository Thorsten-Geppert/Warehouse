#include "StockEntity.h"
#include <DSCurrency.h>

StockEntity::StockEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("stocks"),
	_N("name"),
	true
) {
	Set(_N("name"));
	Set(_N("address"));
	Set(_N("number"));
	Set(_N("zipcode"));
	Set(_N("city"));
	Set(_N("comment"));
	Set(_N("active"), false);
}
