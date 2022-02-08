#include "OrderEntity.h"
#include <DSDateTime.h>
#include <DSCurrency.h>
#include "ProductController.h"
#include "ProductEntity.h"
#include "OrderController.h"
#include "OrderPositionEntity.h"
#include "CalculationLibrary.h"
#include "ConstantsLibrary.h"

using namespace DynSoft;

OrderEntity::OrderEntity(
	RuntimeInformationType *rit
) : InformationEntity(
	rit,
	_N("orders"),
	_N("orderId")
) {
	onlyUnpayed = false;

	DSCurrency price;

	DSDateTime date(DSDateTime::Now());

	Set(_N("orderId"), 0U);
	Set(_N("kind"), 0U, false, false, wxEmptyString, 0, false);
	Set(_N("clientId"));
	Set(_N("ownClientId"));
	Set(_N("basedOn"));
	Set(_N("taxRate"), 0U, false, false, wxEmptyString, 0, false);
	Set(_N("when"), date.ToDatabaseDate());
	Set(_N("billAddressSalutation"));
	Set(_N("billAddressTitle"));
	Set(_N("billAddressFirstname"));
	Set(_N("billAddressName1"));
	Set(_N("billAddressName2"));
	Set(_N("billAddressName3"));
	Set(_N("billAddressName4"));
	Set(_N("billAddressAddress"));
	Set(_N("billAddressNumber"));
	Set(_N("billAddressZipcode"));
	Set(_N("billAddressCity"));
	Set(_N("billAddressFederalState"));
	Set(_N("billAddressLand"));
	Set(_N("billAddressTranslated"));
	Set(_N("shippingAddressSalutation"));
	Set(_N("shippingAddressTitle"));
	Set(_N("shippingAddressFirstname"));
	Set(_N("shippingAddressName1"));
	Set(_N("shippingAddressName2"));
	Set(_N("shippingAddressName3"));
	Set(_N("shippingAddressName4"));
	Set(_N("shippingAddressAddress"));
	Set(_N("shippingAddressNumber"));
	Set(_N("shippingAddressZipcode"));
	Set(_N("shippingAddressCity"));
	Set(_N("shippingAddressFederalState"));
	Set(_N("shippingAddressLand"));
	Set(_N("shippingAddressTranslated"));
	Set(_N("condition"));
	Set(_N("orderNumber"));
	Set(_N("delivery"));
	Set(_N("prepandText"));
	Set(_N("appendText"));
	Set(_N("payment"), 0U, false, false, wxEmptyString, 0, false);
	Set(_N("bank"));
	Set(_N("bankCode"));
	Set(_N("accountNumber"));
	Set(_N("bic"));
	Set(_N("iban"));
	Set(_N("bankOwner"));
	Set(_N("liableToTax"), true);
	Set(_N("beforeTax"), price);
	Set(_N("fullTax"), price);
	Set(_N("reducedTax"), price);
	Set(_N("afterTax"), price);
	Set(_N("invoiceId"), 0U, false, false, wxEmptyString, 0, true);
	Set(_N("invoiceDate"), wxString(), true, true, wxEmptyString, 0, true);
	Set(_N("fullTaxPercent"), price);
	Set(_N("reducedTaxPercent"), price);
	Set(_N("prints"), 0U);
	Set(_N("payed"), wxString(), true, true, wxEmptyString, 0, true);
	Set(_N("taxNumber"));
	Set(_N("salesTaxIdentificationNumber"));
	Set(_N("linkedOrderId"), wxString(), false, false, wxEmptyString, 0, true);
	Set(_N("linkedOrderIndex"), wxString(), false, false, wxEmptyString, 0, true);
	Set(_N("monition"), true);
	Set(_N("monitionState"), 0);
	Set(_N("owner"));
}

OrderEntity::~OrderEntity() {
	GetPositions()->Clear(true);
}

OrderPositionEntities *OrderEntity::GetPositions() {
	return &positions;
}

wxString OrderEntity::GetBankString() {
	wxString bank(Get(_N("bank"))->ToString());
	wxString bankCode(Get(_N("bankCode"))->ToString());
	wxString accountNumber(Get(_N("accountNumber"))->ToString());
	wxString bic(Get(_N("bic"))->ToString());
	wxString iban(Get(_N("iban"))->ToString());
	wxString bankOwner(Get(_N("bankOwner"))->ToString());

	wxString tmp;

	if(!bank.IsEmpty())
		tmp += _("Bank: ") + bank;

	if(!bankCode.IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += _(", ");
		tmp += _("Bank code: ") + bankCode;
	}

	if(!accountNumber.IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += _(", ");
		tmp += _("Account number: ") + accountNumber;
	}

	if(!bic.IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += _(", ");
		tmp += _("BIC: ") + bic;
	}

	if(!iban.IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += _(", ");
		tmp += _("IBAN: ") + iban;
	}

	if(!bankOwner.IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += _(", ");
		tmp += _("Bank owner: ") + bankOwner;
	}

	return tmp;
}

void OrderEntity::SetOnlyUnpayed(const bool onlyUnpayed) {
	this->onlyUnpayed = onlyUnpayed;
}

bool OrderEntity::GetOnlyUnpayed() const {
	return onlyUnpayed;
}

OrderEntities *OrderEntity::GetLinkedOrders() {
	return &linkedOrders;
}

bool OrderEntity::IsValid(wxArrayString *products, wxArrayString *linkedOrders, wxArrayString *allreadyLinkedOrders, bool *isValidKind) {
	bool valid = true;

	wxString productId;
	ProductEntity productEntity(rit);
	const wxString productEntityPrimaryKey(productEntity.GetPrimaryKey());
	ProductController productController(rit);
	OrderPositionEntity *orderPositionEntity = NULL;
	OrderPositionEntities *orderPositionEntities = GetPositions();
	unsigned int count = orderPositionEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		orderPositionEntity = orderPositionEntities->Get(i);
		if(orderPositionEntity) {
			productId = orderPositionEntity->Get(productEntityPrimaryKey)->ToString();
			if(!productController.Exists(productEntityPrimaryKey, productId)) {
				valid = false;
				if(products)
					products->Add(productId);
			}
		} else {
			valid = false;
		}
	}

	wxString orderId;
	unsigned int id;
	const wxString orderEntityPrimaryKey(GetPrimaryKey());
	OrderController orderController(rit);
	OrderEntity *linkedOrderEntity = NULL;
	OrderEntities *linkedOrderEntities = GetLinkedOrders();
	count = linkedOrderEntities->GetCount();

	if(count > 0 && Get(_N("kind"))->ToUnsignedInt() != ConstantsLibrary::ORDER_KIND_ORDER) {
		valid = false;
		if(isValidKind)
			*isValidKind = false;
	} else {
		if(isValidKind)
			*isValidKind = true;
	}

	for(unsigned int i = 0; i < count; i++) {
		linkedOrderEntity = linkedOrderEntities->Get(i);
		if(linkedOrderEntity) {
			orderId = linkedOrderEntity->Get(orderEntityPrimaryKey)->ToString();
			id      = linkedOrderEntity->Get(orderEntityPrimaryKey)->ToUnsignedInt();
			if(!orderController.Exists(orderEntityPrimaryKey, orderId)) {
				if(linkedOrders)
					linkedOrders->Add(orderId);
			}

			if(orderController.IsLinked(id)) {
				if(allreadyLinkedOrders)
					allreadyLinkedOrders->Add(orderId);
			}
		} else {
			valid = false;
		}
	}

	return valid;
}

void OrderEntity::Recalc() {
	DSCurrency tmpBeforeTax;
	DSCurrency tmpFullTax;
	DSCurrency tmpReducedTax;
	DSCurrency tmpAfterTax;

	DSCurrency amount;
	DSCurrency price;
	DSCurrency fullPrice;
	DSCurrency reduction;
	DSCurrency sum;
	DSCurrency tax;
	DSCurrency absolutePrice;
	DSCurrency taxPercent;
	unsigned int taxLevel = 0;
	const unsigned int taxRate = Get(_N("taxRate"))->ToUnsignedInt();
	DSCurrency reducted;
	DSCurrency beforeTax;
	DSCurrency afterTax;
	const bool liableToTax = Get(_N("liableToTax"))->ToBool();

	OrderPositionEntity *orderPositionEntity = NULL;
	OrderPositionEntities *orderPositionEntities = GetPositions();
	unsigned int count = orderPositionEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		orderPositionEntity = orderPositionEntities->Get(i);
		if(orderPositionEntity) {
			taxLevel   = orderPositionEntity->Get(_N("taxLevel"))->ToUnsignedInt();
			if(taxLevel > 2)
				continue;

			amount     = orderPositionEntity->Get(_N("amount"))->ToCurrency();
			price      = orderPositionEntity->Get(_N("price"))->ToCurrency();
			fullPrice  = amount * price;
			reduction  = orderPositionEntity->Get(_N("reduction"))->ToCurrency();
			reducted   = CalculationLibrary::Reduction(fullPrice, reduction);
			sum        = fullPrice - reducted;
			taxPercent = liableToTax ? orderPositionEntity->Get(_N("taxPercent"))->ToCurrency() : 0.0;

			if(taxRate == 0) {
				tax           = CalculationLibrary::GetAddTax(sum, taxPercent);
				absolutePrice = sum + tax;
				beforeTax     = sum;
				afterTax      = absolutePrice;
			} else {
				tax           = CalculationLibrary::GetSubTax(sum, taxPercent);
				absolutePrice = sum;
				beforeTax     = absolutePrice - tax;
				afterTax      = absolutePrice;
			}
			
			orderPositionEntity->SetValue(_N("fullPrice"), fullPrice);
			orderPositionEntity->SetValue(_N("reducted"), reducted);
			orderPositionEntity->SetValue(_N("sum"), sum);
			orderPositionEntity->SetValue(_N("tax"), tax);
			orderPositionEntity->SetValue(_N("absolutePrice"), absolutePrice);
			orderPositionEntity->SetValue(_N("beforeTax"), beforeTax);
			orderPositionEntity->SetValue(_N("afterTax"), afterTax);

			tmpBeforeTax  += orderPositionEntity->Get(_N("beforeTax"))->ToCurrency();
			tmpAfterTax   += orderPositionEntity->Get(_N("afterTax"))->ToCurrency();
			if(orderPositionEntity->Get(_N("taxLevel"))->ToUnsignedInt() == 0)
				tmpFullTax    += orderPositionEntity->Get(_N("tax"))->ToCurrency();
			else
				tmpReducedTax += orderPositionEntity->Get(_N("tax"))->ToCurrency();
		}
	}

	OrderEntities *linkedOrderEntities = GetLinkedOrders();
	OrderEntity *linkedOrderEntity = NULL;
	count = linkedOrderEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		linkedOrderEntity = linkedOrderEntities->Get(i);
		if(linkedOrderEntity) {
			tmpBeforeTax  += linkedOrderEntity->Get(_N("beforeTax"))->ToCurrency();
			tmpAfterTax   += linkedOrderEntity->Get(_N("afterTax"))->ToCurrency();
			tmpFullTax    += linkedOrderEntity->Get(_N("fullTax"))->ToCurrency();
			tmpReducedTax += linkedOrderEntity->Get(_N("reducedTax"))->ToCurrency();
		}
	}

	SetValue(_N("beforeTax"),  tmpBeforeTax);
	SetValue(_N("fullTax"),    tmpFullTax);
	SetValue(_N("reducedTax"), tmpReducedTax);
	SetValue(_N("afterTax"),   tmpAfterTax);
}

LinkedFileEntities *OrderEntity::GetLinkedFileEntities() {
	return &linkedFileEntities;
}

void OrderEntity::SetSearchYear(const unsigned int searchYear) {
	this->searchYear = searchYear;
}

unsigned int OrderEntity::GetSearchYear() const {
	return searchYear;
}
