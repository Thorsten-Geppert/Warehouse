#include "VoucherEntity.h"
#include <DSCurrency.h>
#include <DSDateTime.h>

VoucherEntity::VoucherEntity(
	RuntimeInformationType *rit
) : InformationEntity(
	rit,
	_N("vouchers"),
	_N("voucherId")
) {
	DSCurrency price;
	DSDateTime date;
	date.SetWxDateTime(wxDateTime::Now());

	Set(_N("voucherId"), 0U);
	Set(_N("description"));
	Set(_N("invoiceId"));
	Set(_N("invoiceDate"), date.ToDatabaseDate());
	Set(_N("company"));
	Set(_N("notes"));
	Set(_N("priceWithoutTax"), price);
	Set(_N("reducedTax"), price);
	Set(_N("fullTax"), price);
	Set(_N("priceWithTax"), price);
}

LinkedFileEntities *VoucherEntity::GetLinkedFileEntities() {
	return &linkedFileEntities;
}

VoucherPositionEntities *VoucherEntity::GetVoucherPositions() {
	return &voucherPositionEntities;
}

void VoucherEntity::SetSearchYear(const unsigned int searchYear) {
	this->searchYear = searchYear;
}

unsigned int VoucherEntity::GetSearchYear() const {
	return searchYear;
}
