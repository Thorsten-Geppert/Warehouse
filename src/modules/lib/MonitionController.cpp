#include "MonitionController.h"
#include <DSQueryParser.h>
#include <DSCasts.h>
#include <DSTemplate.h>
#include <DSCgi.h>
#include <DSCurrency.h>
#include "EnterprisePreferencesController.h"
#include "EnterprisePreferencesEntity.h"
#include "OrderEntity.h"
#include "OrderController.h"
#include "MonitionTextEntity.h"
#include "MonitionTextController.h"
#include "NameType.h"
#include "AddressType.h"
#include "Tools.h"

MonitionController::MonitionController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("monitions"), // Table
	_N("monitionId")
) {
	LoadResource(rit->GRP(_N("sql"), _N("Monitions.res")));
}

void MonitionController::GetUnpayedOrders(
	wxString orderId,
	wxString invoiceId,
	wxString clientId,
	const wxString &invoiceDateFrom,
	const wxString &invoiceDateTo,
	wxArrayString &monitionStates
) {
	orderId   = WildCard(orderId);
	invoiceId = WildCard(invoiceId);
	clientId  = WildCard(clientId);

	wxString query(GetResourceByName(_N("Monitions.GetUnpayedOrdersState")));
	wxString states, tmp;
	const unsigned int count = monitionStates.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		tmp = query;
		tmp.Replace(_N("{s:monitionState}"), _N(":monitionState") + DSCasts::ToString(i) + _N(":"));
		states += tmp;
		if(i < count - 1)
			states += _N(" OR ");
	}

	DSQueryParser parser;
	parser.Prepare(GetResourceByName(_N("Monitions.GetUnpayedOrdersStart")) + states + GetResourceByName(_N("Monitions.GetUnpayedOrdersStop")));
	parser.Bind(_N("orderId"), orderId);
	parser.Bind(_N("invoiceId"), invoiceId);
	parser.Bind(_N("clientId"), clientId);
	parser.Bind(_N("invoiceDateFrom"), invoiceDateFrom);
	parser.Bind(_N("invoiceDateTo"), invoiceDateTo);
	for(unsigned int i = 0; i < count; i++)
		parser.Bind(_N("monitionState") + DSCasts::ToString(i), DSCasts::ToUnsignedInt(monitionStates[i]));

	Create(parser.GetParsedQuery());
}

MonitionEntity *MonitionController::GetUnpayedOrder() {
	if(!query->NextRecord())
		return NULL;

	MonitionEntity *monitionEntity = new MonitionEntity(rit);

	DSCurrency price(query->GetField(_N("price")).ToCurrency());
	monitionEntity->SetValue(_N("invoiceDate"), query->GetField(_N("invoiceDate")).ToString());
	monitionEntity->SetValue(_N("clientId"), query->GetField(_N("clientId")).ToUnsignedInt());
	monitionEntity->SetValue(_N("orderId"), query->GetField(_N("orderId")).ToUnsignedInt());
	monitionEntity->SetValue(_N("invoiceId"), query->GetField(_N("invoiceId")).ToUnsignedInt());
	monitionEntity->SetValue(_N("price"), price);
	monitionEntity->SetValue(_N("monitionState"), query->GetField(_N("monitionState")).ToUnsignedInt());

	return monitionEntity;
}

wxString MonitionController::Print(
	HtmlPrintoutLibrary *htmlPrintoutLibrary,
	MonitionEntity *monitionEntity,
	DSCurrency monitionPrice
) {
	if(!htmlPrintoutLibrary || !monitionEntity)
		return wxEmptyString;

	EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());
	EnterprisePreferencesController enterprisePreferencesController(rit->GetDatabase());
	enterprisePreferencesController.Get();
	if(enterprisePreferencesController.Run()) {
		enterprisePreferencesController.Get(&enterprisePreferencesEntity);
		enterprisePreferencesController.Reset();
	}

	OrderEntity orderEntity(rit);
	OrderController orderController(rit);
	orderController.Get(monitionEntity->Get(_N("orderId"))->ToString());
	if(orderController.Run()) {
		orderController.Get(&orderEntity);
		orderController.Reset();
	}

	MonitionTextController monitionTextController(rit);
	MonitionTextEntity *monitionTextEntity = monitionTextController.GetDirect(
		monitionEntity->Get(_N("monitionState"))->ToInt() + 1,
		true
	);

	DSTemplate monitionTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("Monition"));
	monitionTemplate.SetEntity(&orderEntity);
	monitionTemplate.SetEntity(monitionEntity);
	monitionTemplate.SetEntity(&enterprisePreferencesEntity);

	monitionTemplate.SetVariable(_N("monitionState"), DSCasts::ToString(monitionEntity->Get(_N("monitionState"))->ToInt() + 1));
	monitionTemplate.SetVariable(_N("monitionPrice"), monitionPrice.ToString(), true);
	monitionTemplate.SetVariable(_N("prependText"), nl2br(HtmlSpecialChars(monitionTextEntity->Get(_N("prepend"))->ToString())));
	monitionTemplate.SetVariable(_N("appendText"), nl2br(HtmlSpecialChars(monitionTextEntity->Get(_N("append"))->ToString())));
	monitionTemplate.SetVariable(
		_N("translatedBillAddress"),
		nl2br(DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressTranslated"))->ToString()))
	);
	monitionTemplate.SetVariable(
		_N("billAddress"),
		NameType(
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressSalutation"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressTitle"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressFirstname"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressName1"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressName2"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressName3"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressName4"))->ToString())
		).GetConcatedFullName(_N("<br>")) +
		_N("<br>") +
		AddressType(
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressAddress"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressNumber"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressZipcode"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressCity"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressFederalState"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity.Get(_N("billAddressLand"))->ToString())
		).GetConcatedAddress(_N("<br>"))
	);
	monitionTemplate.SetVariable(_N("orderSalesTaxIdentificationNumber"), Tools::Description(orderEntity.Get(_N("salesTaxIdentificationNumber"))->ToString(), _("n/a")), true);
	monitionTemplate.SetVariable(_N("orderTaxNumber"), Tools::Description(orderEntity.Get(_N("taxNumber"))->ToString(), _("n/a")), true);
	monitionTemplate.SetVariable(_N("invoiceDate"), Tools::Description(orderEntity.Get(_N("invoiceDate"))->ToString(), _("n/a")), true);
	monitionTemplate.SetVariable(_N("delivery"), Tools::Description(orderEntity.Get(_N("delivery"))->ToString(), _("n/a")), true);
	monitionTemplate.SetVariable(_N("orderNumber"), Tools::Description(orderEntity.Get(_N("orderNumber"))->ToString(), _("n/a")), true);
	monitionTemplate.SetVariable(
		_N("sum"),
		DSCurrency(monitionEntity->Get(_N("price"))->ToCurrency() + monitionPrice).ToString(),
		true
	);

	delete monitionTextEntity;

	monitionTemplate.Parse();
	return monitionTemplate.Get();
}
