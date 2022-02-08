#include "PurchaseEntity.h"
#include <DSDateTime.h>
#include <DSCurrency.h>
#include "ProductController.h"
#include "ProductEntity.h"

using namespace DynSoft;

PurchaseEntity::PurchaseEntity(
	RuntimeInformationType *rit
) : InformationEntity(
	rit,
	_N("purchases"),
	_N("purchaseId")
) {
	DSCurrency price;

	DSDateTime date;
	date.SetWxDateTime(wxDateTime::Now());

	Set(_N("purchaseId"), 0U);
	Set(_N("distributorId"), 0U);
	Set(_N("distributorAddress"));
	Set(_N("clientId"));
	Set(_N("orderId"));
	Set(_N("orderDate"), date.ToDatabaseDate());
	Set(_N("purchaser"));
	Set(_N("informations"));
	Set(_N("billingAddress"));
	Set(_N("deliveryAddress"));
	Set(_N("textHead"));
	Set(_N("textFoot"));
	Set(_N("sum"), price);
	Set(_N("done"), wxString(), true, true, wxEmptyString, 0, true);
}

PurchaseEntity::~PurchaseEntity() {
	GetPositions()->Clear(true);
}

PurchasePositionEntities *PurchaseEntity::GetPositions() {
	return &positions;
}

LinkedFileEntities *PurchaseEntity::GetLinkedFileEntities() {
	return &linkedFileEntities;
}

void PurchaseEntity::Recalc() {
	DSCurrency sum;

	PurchasePositionEntity *purchasePositionEntity = NULL;
	PurchasePositionEntities *purchasePositionEntities = GetPositions();
	unsigned int count = purchasePositionEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		purchasePositionEntity = purchasePositionEntities->Get(i);
		if(purchasePositionEntity) {
			purchasePositionEntity->Recalc();
			sum += purchasePositionEntity->Get(_N("fullPrice"))->ToCurrency();
		}
	}

	SetValue(_N("sum"), sum);
}

bool PurchaseEntity::IsValid(wxArrayString *products) {
	bool valid = true;

	wxString productId;
	ProductEntity productEntity(rit);
	const wxString productEntityPrimaryKey(productEntity.GetPrimaryKey());
	ProductController productController(rit);
	PurchasePositionEntity *purchasePositionEntity = NULL;
	PurchasePositionEntities *purchasePositionEntities = GetPositions();
	unsigned int count = purchasePositionEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		purchasePositionEntity = purchasePositionEntities->Get(i);
		if(purchasePositionEntity) {
			productId = purchasePositionEntity->Get(productEntityPrimaryKey)->ToString();
			if(!productController.Exists(productEntityPrimaryKey, productId)) {
				valid = false;
				if(products)
					products->Add(productId);
			}
		} else {
			valid = false;
		}
	}

	return valid;
}

void PurchaseEntity::SetSearchYear(const unsigned int searchYear) {
	this->searchYear = searchYear;
}

unsigned int PurchaseEntity::GetSearchYear() const {
	return searchYear;
}
