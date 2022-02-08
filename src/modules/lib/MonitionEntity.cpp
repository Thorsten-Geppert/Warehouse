#include "MonitionEntity.h"
#include <DSCurrency.h>

MonitionEntity::MonitionEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("monitions"),
	_N("monitionId")
) {
	DSCurrency price;
	Set(_N("monitionId"), 0U);
	Set(_N("invoiceDate"));
	Set(_N("clientId"));
	Set(_N("orderId"));
	Set(_N("invoiceId"));
	Set(_N("price"), price);
	Set(_N("monitionState"), 0U);
	Set(_N("created"), _N("NOW()"), false);
}
