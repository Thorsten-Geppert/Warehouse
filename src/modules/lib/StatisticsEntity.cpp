#include "StatisticsEntity.h"
#include <DSCurrency.h>

StatisticsEntity::StatisticsEntity() {
	DSCurrency price;

	Set(_N("invoiceId"), 0U);
	Set(_N("invoiceDate"));
	Set(_N("clientId"));
	Set(_N("productId"));
	Set(_N("orderId"), 0U);
	Set(_N("taxLevel"), 0U);
	Set(_N("beforeTax"), price);
	Set(_N("reducedTax"), price);
	Set(_N("fullTax"), price);
	Set(_N("afterTax"), price);
}
