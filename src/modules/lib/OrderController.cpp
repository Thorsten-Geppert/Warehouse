#include "OrderController.h"
#include <DSPostgreSQLQuery.h>
#include <DSDateTime.h>
#include <DSCgi.h>
#include "OrderPositionController.h"
#include "OrderPositionEntity.h"
#include "OrderPositionEntities.h"
#include "ClientEntity.h"
#include "ClientController.h"
#include "ConstantsLibrary.h"
#include "NameType.h"
#include "AddressType.h"
#include "EnterprisePreferencesController.h"
#include "EnterprisePreferencesEntity.h"
#include "CommonPreferencesController.h"
#include "Tools.h"
#include "LinkedFileController.h"
#include "StockBookEntity.h"
#include "StockBookController.h"

OrderController::OrderController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("orders"), // Table
	_N("orderId"),  // Primary key
	_N("orderId"),
	ASC
) {
	LoadResource(rit->GRP(_N("sql"), _N("Orders.res")));
}

void OrderController::Get(OrderEntity *orderEntity, const bool justGetMainEntity) {
	Controller::Get(orderEntity);

	if(!justGetMainEntity) {
		bool found = false;
		unsigned int count = 0;
		OrderPositionEntity *orderPositionEntity = NULL;
		OrderPositionEntities *orderPositionEntities = orderEntity->GetPositions();
		OrderPositionController orderPositionController(rit);
		orderPositionController.Get(_N("orderId"), orderEntity->Get(_N("orderId"))->ToString());
		while(orderPositionController.Run()) {
			orderPositionEntity = new OrderPositionEntity(rit);
			orderPositionController.Get(orderPositionEntity);

			found = false;
			count = orderPositionEntities->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				if(orderPositionEntities->Get(i)->Get(_N("orderPositionId"))->ToUnsignedInt() == orderPositionEntity->Get(_N("orderPositionId"))->ToUnsignedInt()) {
					found = true;
					break;
				}
			}

			if(!found)
				orderPositionEntities->Add(orderPositionEntity);
			else
				delete orderPositionEntity;
		}

		OrderEntities *linkedOrderEntities = orderEntity->GetLinkedOrders();
		OrderEntity *linkedOrderEntity     = NULL;
		OrderController orderController(rit);
		orderController.SetOrderBy(_N("linkedOrderIndex"), OrderController::ASC);
		orderController.Get(_N("linkedOrderId"), orderEntity->Get(_N("orderId"))->ToString());
		while(orderController.Run()) {
			linkedOrderEntity = new OrderEntity(rit);
			orderController.Get(linkedOrderEntity);

			found = false;
			count = linkedOrderEntities->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				if(linkedOrderEntities->Get(i)->Get(_N("orderId"))->ToUnsignedInt() == linkedOrderEntity->Get(_N("orderId"))->ToUnsignedInt()) {
					found = true;
					break;
				}
			}

			if(!found)
				linkedOrderEntities->Add(linkedOrderEntity);
			else
				delete linkedOrderEntity;
		}

		LinkedFileController linkedFileController(rit, _N("linked_files_orders"));
		linkedFileController.Load(
			orderEntity->Get(_N("orderId"))->ToUnsignedInt(),
			*orderEntity->GetLinkedFileEntities()
		);

		LoadKeysValues(
			orderEntity->Get(_N("orderId"))->ToUnsignedInt(),
			_N("keys_values_orders"),
			*orderEntity->GetKeysValuesType()
		);
	}
}

bool OrderController::Save(OrderEntity *orderEntity, wxArrayString *orderPositionsRemoved) {
	bool ok = false;

	wxArrayString insertExcludes(*orderEntity->GetInsertExcludes());
	wxArrayString updateExcludes(*orderEntity->GetUpdateExcludes());
	
	insertExcludes.Add(_N("invoiceId"));      updateExcludes.Add(_N("invoiceId"));
	insertExcludes.Add(_N("invoiceDate"));    updateExcludes.Add(_N("invoiceDate"));
	insertExcludes.Add(_N("printsBid"));      updateExcludes.Add(_N("printsBid"));
	insertExcludes.Add(_N("printsOrder"));    updateExcludes.Add(_N("printsOrder"));
	insertExcludes.Add(_N("printsCredit"));   updateExcludes.Add(_N("printsCredit"));
	insertExcludes.Add(_N("printsDelivery")); updateExcludes.Add(_N("printsDelivery"));
	insertExcludes.Add(_N("monitionState"));  updateExcludes.Add(_N("monitionState"));

	DSCurrency fullTaxPercent, reducedTaxPercent;
	CommonPreferencesController::GetTax(rit, &fullTaxPercent, &reducedTaxPercent);

	orderEntity->SetValue(_N("fullTaxPercent"), fullTaxPercent);
	orderEntity->SetValue(_N("reducedTaxPercent"), reducedTaxPercent);

	ClientController clientController(rit);
	clientController.Get(orderEntity->Get(_N("clientId"))->ToUnsignedInt());
	if(clientController.Run()) {
		ClientEntity clientEntity(rit);
		clientController.Get(&clientEntity, true);
		clientController.Reset();
	} else {
		return false;
	}
	
	DSPostgreSQLQuery query(ndb);
	query.Begin();

	OrderController orderController(rit);
	if(orderEntity->Get(_N("orderId"))->ToUnsignedInt() == 0) {
		ok = orderController.Insert(orderEntity, true, &insertExcludes, &query);
	} else {
		orderEntity->ForceChanged();
		ok = orderController.Update(orderEntity, wxEmptyString, true, &updateExcludes, &query);
	}

	if(ok) {
		OrderPositionEntities *orderPositionEntities = orderEntity->GetPositions();
		OrderPositionEntity   *orderPositionEntity   = NULL;
		OrderPositionController orderPositionController(rit);
		unsigned int count = orderPositionEntities->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			orderPositionEntity = orderPositionEntities->Get(i);
			if(orderPositionEntity) {
				orderPositionEntity->SetValue(_N("orderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());

				if(orderPositionEntity->Get(_N("orderPositionId"))->ToUnsignedInt() == 0)
					ok = orderPositionController.Insert(orderPositionEntity, true, NULL, &query);
				else
					ok = orderPositionController.Update(orderPositionEntity, wxEmptyString, true, NULL, &query);

				if(!ok)
					break;
			}
		}

		if(ok && orderPositionsRemoved) {
			OrderPositionEntity tmpOrderPositionEntity(rit);
			const unsigned int count = orderPositionsRemoved->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				tmpOrderPositionEntity.SetValue(_N("orderPositionId"), (*orderPositionsRemoved)[i]);
				ok = orderPositionController.Delete(&tmpOrderPositionEntity, wxEmptyString, &query);
				if(!ok)
					break;
			}
		}

		if(ok) {
			query.Prepare(GetResourceByName(_N("Orders.Unlink")));
			query.Bind(_N("orderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
			ok = query.Exec(false);

			if(ok) {
				OrderEntity *linkedOrderEntity = NULL;
				OrderEntities *linkedOrderEntities = orderEntity->GetLinkedOrders();
				OrderController linkedOrderController(rit);
				const unsigned int count = linkedOrderEntities->GetCount();
				for(unsigned int i = 0; i < count; i++) {
					linkedOrderEntity = linkedOrderEntities->Get(i);
					if(linkedOrderEntity) {
						query.Prepare(GetResourceByName(_N("Orders.Link")));
						query.Bind(_N("linkedOrderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
						query.Bind(_N("linkedOrderIndex"), linkedOrderEntity->Get(_N("linkedOrderIndex"))->ToUnsignedInt());
						query.Bind(_N("orderId"), linkedOrderEntity->Get(_N("orderId"))->ToUnsignedInt());
						ok = query.Exec(false);
						if(!ok)
							break;

						linkedOrderEntity->SetValue(_N("linkedOrderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
					}
				}
			}
		}
	}

	if(ok) {
		LinkedFileController linkedFileController(rit, _N("linked_files_orders"));
		ok = linkedFileController.Save(
			query,
			orderEntity->Get(_N("orderId"))->ToUnsignedInt(),
			*orderEntity->GetLinkedFileEntities()
		);
	}

	if(ok) {
		ok = SaveKeysValues(
			query,
			orderEntity->Get(_N("orderId"))->ToUnsignedInt(),
			_N("keys_values_orders"),
			*orderEntity->GetKeysValuesType()
		);
	}

	if(!query.Commit()) {
		query.Rollback();
		return false;
	} else {
		if(orderPositionsRemoved)
			orderPositionsRemoved->Clear();
		orderEntity->GetLinkedFileEntities()->CleanRemoved();
	}

	return true;
}

void OrderController::Find(OrderEntity *orderEntity) {
	const wxString invoiceId(WildCard(orderEntity->Get(_N("invoiceId"))->ToUnsignedInt() == 0 ? wxString(_N("%")) : orderEntity->Get(_N("invoiceId"))->ToString()));
	const wxString orderId(WildCard(orderEntity->Get(_N("orderId"))->ToString()));
	const wxString clientId(WildCard(orderEntity->Get(_N("clientId"))->ToString()));
	const wxString billAddressSalutation(WildCard(orderEntity->Get(_N("billAddressSalutation"))->ToString()));
	const wxString billAddressTitle(WildCard(orderEntity->Get(_N("billAddressTitle"))->ToString()));
	const wxString billAddressFirstname(WildCard(orderEntity->Get(_N("billAddressFirstname"))->ToString()));
	const wxString billAddressAddress(WildCard(orderEntity->Get(_N("billAddressAddress"))->ToString()));
	const wxString billAddressZipcode(WildCard(orderEntity->Get(_N("billAddressZipcode"))->ToString()));
	const wxString billAddressCity(WildCard(orderEntity->Get(_N("billAddressCity"))->ToString()));
	const wxString billAddressLand(WildCard(orderEntity->Get(_N("billAddressLand"))->ToString()));
	const wxString billAddressName1(WildCard(orderEntity->Get(_N("billAddressName1"))->ToString()));
	const wxString billAddressName2(WildCard(orderEntity->Get(_N("billAddressName2"))->ToString()));
	const wxString billAddressName3(WildCard(orderEntity->Get(_N("billAddressName3"))->ToString()));
	const wxString billAddressName4(WildCard(orderEntity->Get(_N("billAddressName4"))->ToString()));
	const wxString shippingAddressSalutation(WildCard(orderEntity->Get(_N("shippingAddressSalutation"))->ToString()));
	const wxString shippingAddressTitle(WildCard(orderEntity->Get(_N("shippingAddressTitle"))->ToString()));
	const wxString shippingAddressFirstname(WildCard(orderEntity->Get(_N("shippingAddressFirstname"))->ToString()));
	const wxString shippingAddressAddress(WildCard(orderEntity->Get(_N("shippingAddressAddress"))->ToString()));
	const wxString shippingAddressZipcode(WildCard(orderEntity->Get(_N("shippingAddressZipcode"))->ToString()));
	const wxString shippingAddressCity(WildCard(orderEntity->Get(_N("shippingAddressCity"))->ToString()));
	const wxString shippingAddressLand(WildCard(orderEntity->Get(_N("shippingAddressLand"))->ToString()));
	const wxString shippingAddressName1(WildCard(orderEntity->Get(_N("shippingAddressName1"))->ToString()));
	const wxString shippingAddressName2(WildCard(orderEntity->Get(_N("shippingAddressName2"))->ToString()));
	const wxString shippingAddressName3(WildCard(orderEntity->Get(_N("shippingAddressName3"))->ToString()));
	const wxString shippingAddressName4(WildCard(orderEntity->Get(_N("shippingAddressName4"))->ToString()));
	const wxString kind(WildCard(orderEntity->Get(_N("kind"))->ToUnsignedInt() == ConstantsLibrary::OrderKindToArrayString().GetCount() - 1 ? wxString(_N("%")) : orderEntity->Get(_N("kind"))->ToString()));
	const wxString year(WildCard(DSCasts::ToString(orderEntity->GetSearchYear()), true));
	
	DSQueryParser parser;
	parser.Prepare(GetResourceByName(_N("Orders.Find")));
	parser.Bind(_N("table"), GetTable(), false);
	parser.Bind(_N("invoiceId"), invoiceId, false);
	parser.Bind(_N("orderId"), orderId);
	parser.Bind(_N("clientId"), clientId);
	parser.Bind(_N("billAddressSalutation"), billAddressSalutation);
	parser.Bind(_N("billAddressTitle"), billAddressTitle);
	parser.Bind(_N("billAddressFirstname"), billAddressFirstname);
	parser.Bind(_N("billAddressAddress"), billAddressAddress);
	parser.Bind(_N("billAddressZipcode"), billAddressZipcode);
	parser.Bind(_N("billAddressCity"), billAddressCity);
	parser.Bind(_N("billAddressLand"), billAddressLand);
	parser.Bind(_N("billAddressName1"), billAddressName1);
	parser.Bind(_N("billAddressName2"), billAddressName2);
	parser.Bind(_N("billAddressName3"), billAddressName3);
	parser.Bind(_N("billAddressName4"), billAddressName4);
	parser.Bind(_N("shippingAddressSalutation"), shippingAddressSalutation);
	parser.Bind(_N("shippingAddressTitle"), shippingAddressTitle);
	parser.Bind(_N("shippingAddressFirstname"), shippingAddressFirstname);
	parser.Bind(_N("shippingAddressAddress"), shippingAddressAddress);
	parser.Bind(_N("shippingAddressZipcode"), shippingAddressZipcode);
	parser.Bind(_N("shippingAddressCity"), shippingAddressCity);
	parser.Bind(_N("shippingAddressLand"), shippingAddressLand);
	parser.Bind(_N("shippingAddressName1"), shippingAddressName1);
	parser.Bind(_N("shippingAddressName2"), shippingAddressName2);
	parser.Bind(_N("shippingAddressName3"), shippingAddressName3);
	parser.Bind(_N("shippingAddressName4"), shippingAddressName4);
	parser.Bind(_N("kind"), kind, false);
	parser.Bind(
		_N("onlyUnpayed"),
		(orderEntity->GetOnlyUnpayed() ? GetResourceByName(_N("Orders.OnlyUnpayed")) : wxString()),
		false
	);
	parser.Bind(_N("year"), year);

	Create(parser.GetParsedQuery());
}

bool OrderController::Invoice(OrderEntity *orderEntity, const DSDateTime &invoiceDate) {
	unsigned int invoiceId = 0;

	DSPostgreSQLQuery query(ndb);
	query.Begin();
	query.Query(GetResourceByName(
		orderEntity->Get(_N("kind"))->ToUnsignedInt() == ConstantsLibrary::ORDER_KIND_CREDIT ? _N("Orders.CreditInvoiceId") : _N("Orders.OrderInvoiceId")
	));
	if(query.NextRecord())
		invoiceId = query.GetField(_N("invoiceId")).ToUnsignedInt();
	query.FreeResult();

	if(invoiceId == 0) {
		query.Rollback();
		return false;
	}

	ClientController clientController(rit);
	clientController.Get(orderEntity->Get(_N("clientId"))->ToUnsignedInt());
	if(clientController.Run()) {
		ClientEntity clientEntity(rit);
		clientController.Get(&clientEntity, true);
		clientController.Reset();
	} else {
		query.Rollback();
		return false;
	}

	wxArrayString stocks, products, quantities;
	query.Prepare(GetResourceByName(_N("Orders.GetBookedStockAndQuantityAndProduct")));
	query.Bind(_N("linkedOrderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
	query.Bind(_N("orderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
	query.Exec();
	while(query.NextRecord()) {
		stocks.Add(query.GetField(_N("stock")).ToString());
		products.Add(query.GetField(_N("productId")).ToString());
		quantities.Add(query.GetField(_N("quantity")).ToString());
	}
	query.FreeResult();

	// NEW
	DSCurrency quantity;
	StockBookEntity stockBookEntity(rit);
	StockBookController stockBookController(rit);
	
	const unsigned int stocksCount = stocks.GetCount();
	for(unsigned int i = 0; i < stocksCount; i++) {
		quantity = DSCasts::ToDouble(quantities[i]) * -1;

		stockBookEntity.SetValue(_N("stock"), stocks[i]);
		stockBookEntity.SetValue(_N("product"), products[i]);
		stockBookEntity.SetValue(_N("quantity"), quantity);
		stockBookEntity.SetValue(_N("comment"), _("System book out, order: ") + orderEntity->Get(_N("orderId"))->ToString());

		if(!stockBookController.Insert(&stockBookEntity, true, NULL, &query)) {
			query.Rollback();
			return false;
		}
	}

	query.Prepare(GetResourceByName(_N("Orders.BookOut")));
	query.Bind(_N("linkedOrderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
	query.Bind(_N("orderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
	query.Exec(false);
	if(!query.Exec()) {
		query.Rollback();
		return false;
	}
	// NEW

	query.Prepare(GetResourceByName(_N("Orders.Invoice")));
	query.Bind(_N("table"), GetTable(), false);
	query.Bind(_N("orderId"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
	query.Bind(_N("invoiceId"), invoiceId);
	query.Bind(_N("invoiceDate"), invoiceDate.ToDatabaseDate());
	query.Exec(false);
	if(!query.Commit()) {
		query.Rollback();
		return false;
	}

	orderEntity->SetValue(_N("invoiceId"), invoiceId);
	orderEntity->SetValue(_N("invoiceDate"), invoiceDate.ToDatabaseDate());
	
	return true;
}

void OrderController::Change(OrderEntity *orderEntity, const bool copy) {
	unsigned int kind = orderEntity->Get(_N("kind"))->ToUnsignedInt();
	if(!copy)
		kind = kind == 0 ? 1 : 2;
	//const unsigned int kind = orderEntity->Get(_N("kind"))->ToUnsignedInt() == ConstantsLibrary::ORDER_KIND_BID ? ConstantsLibrary::ORDER_KIND_ORDER : ConstantsLibrary::ORDER_KIND_CREDIT;

	if(!copy)
		orderEntity->SetValue(_N("basedOn"), orderEntity->Get(_N("orderId"))->ToUnsignedInt());
	orderEntity->SetValue(_N("orderId"), 0U);
	orderEntity->SetValue(_N("kind"), kind);
	orderEntity->SetValue(_N("invoiceId"), 0U);
	orderEntity->SetValue(_N("invoiceDate"));
	orderEntity->SetValue(_N("payed"), wxString());
	orderEntity->SetValue(_N("createdAt"), _N("NOW()"));
	orderEntity->SetValue(_N("changeAt"), _N("NOW()"));

	if(copy) {
		DSDateTime when;
		when.SetWxDateTime(wxDateTime::Now());
		orderEntity->SetValue(_N("when"), when.ToDatabaseDate());
	}

	DSCurrency tmp;
	OrderPositionEntity *orderPositionEntity     = NULL;
	OrderPositionEntities *orderPositionEntities = orderEntity->GetPositions();
	unsigned int count = orderPositionEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		orderPositionEntity = orderPositionEntities->Get(i);
		if(orderPositionEntity) {
			orderPositionEntity->SetValue(_N("orderPositionId"), 0U);
			orderPositionEntity->SetValue(_N("orderId"), 0U);
			if(!copy && kind == ConstantsLibrary::ORDER_KIND_CREDIT) {
				tmp = orderPositionEntity->Get(_N("price"))->ToCurrency() * -1;
				orderPositionEntity->SetValue(_N("price"), tmp);
			}
		}
	}

	unsigned int positionCount         = 0;
	OrderEntity *linkedOrderEntity     = NULL;
	OrderEntities *linkedOrderEntities = orderEntity->GetLinkedOrders();
	count = linkedOrderEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		linkedOrderEntity = linkedOrderEntities->Get(i);
		if(!linkedOrderEntity)
			continue;
		
		orderPositionEntities = linkedOrderEntity->GetPositions();
		positionCount         = orderPositionEntities->GetCount();
		for(unsigned j = 0; j < positionCount; j++) {
			orderPositionEntity = orderPositionEntities->Get(j);
			if(!orderPositionEntity)
				continue;
			
			orderPositionEntity->SetValue(_N("orderPositionId"), 0U);
			orderPositionEntity->SetValue(_N("orderId"), 0U);
			if(!copy && kind == ConstantsLibrary::ORDER_KIND_CREDIT) {
				tmp = orderPositionEntity->Get(_N("price"))->ToCurrency() * -1;
				orderPositionEntity->SetValue(_N("price"), tmp);
			}

			orderEntity->GetPositions()->Add(orderPositionEntity);
		}
	}

	orderEntity->GetLinkedOrders()->Clear(false);
	orderEntity->Recalc();

	orderEntity->GetLinkedFileEntities()->ResetId();
}

void OrderController::Statistics(
	const unsigned int productId,
	const unsigned int clientId,
	const wxDateTime &from,
	const wxDateTime &to
) {
	DSDateTime fromDateTime;
	fromDateTime.SetWxDateTime(from);
	DSDateTime toDateTime;
	toDateTime.SetWxDateTime(to);

	wxString queryProduct;
	wxString queryClient;

	DSQueryParser parser;
	wxString query(GetResourceByName(_N("Orders.Statistics")));
	if(productId != 0) {
		parser.Prepare(GetResourceByName(_N("Orders.StatisticsProductId")));
		parser.Bind(_N("productId"), productId);
		queryProduct = parser.GetParsedQuery();
	}
	
	if(clientId != 0) {
		parser.Prepare(GetResourceByName(_N("Orders.StatisticsClientId")));
		parser.Bind(_N("clientId"), clientId);
		queryClient = parser.GetParsedQuery();
	}

	parser.Prepare(query);
	parser.Bind(_N("from"), fromDateTime.ToDatabaseDate());
	parser.Bind(_N("to"), toDateTime.ToDatabaseDate());
	parser.Bind(_N("product"), queryProduct, false);
	parser.Bind(_N("client"), queryClient, false);

	Create(parser.GetParsedQuery());
}

void OrderController::StatisticsGet(StatisticsEntity *statisticsEntity) {
	wxString tmp;

	if(statisticsEntity) {
		DSCurrency beforeTax(query->GetField(_N("beforeTax")).ToCurrency());
		DSCurrency fullTax(query->GetField(_N("taxLevel")).ToUnsignedInt() == 0 ? query->GetField(_N("tax")).ToCurrency() : DSCurrency());
		DSCurrency reducedTax(query->GetField(_N("taxLevel")).ToUnsignedInt() == 1 ? query->GetField(_N("tax")).ToCurrency() : DSCurrency());
		DSCurrency afterTax(query->GetField(_N("afterTax")).ToCurrency());

		statisticsEntity->SetValue(_N("invoiceId"), query->GetField(_N("invoiceId")).ToUnsignedInt());
		statisticsEntity->SetValue(_N("invoiceDate"), query->GetField(_N("invoiceDate")).ToString());

		tmp = query->GetField(_N("ownClientId")).ToString();
		if(tmp.IsEmpty())
			tmp = query->GetField(_N("clientId")).ToString();
		statisticsEntity->SetValue(_N("clientId"), tmp);

		tmp = query->GetField(_N("articleNumber")).ToString();
		if(tmp.IsEmpty())
			tmp = query->GetField(_N("productId")).ToString();
		statisticsEntity->SetValue(_N("productId"), tmp);

		statisticsEntity->SetValue(_N("orderId"), query->GetField(_N("orderId")).ToUnsignedInt());
		statisticsEntity->SetValue(_N("taxLevel"), query->GetField(_N("taxLevel")).ToUnsignedInt());
		statisticsEntity->SetValue(_N("beforeTax"), beforeTax);
		statisticsEntity->SetValue(_N("fullTax"), fullTax);
		statisticsEntity->SetValue(_N("reducedTax"), reducedTax);
		statisticsEntity->SetValue(_N("afterTax"), afterTax);
	}
}

unsigned int OrderController::GetReferences(
	const unsigned int kind,
	const unsigned int primaryKey,
	ReferencesType *referencesType
) {
	unsigned int counter = 0;
	if(!referencesType)
		return counter;

	if(kind == ConstantsLibrary::PRODUCT_KIND) {
		DSPostgreSQLQuery query(ndb);
		query.Prepare(GetResourceByName(_N("Orders.ReferenceByProductId")));
		query.Bind(_N("productId"), primaryKey);
		query.Exec();
		while(query.NextRecord()) {
			referencesType->Append(
				ConstantsLibrary::ORDER_KIND,
				query.GetField(_N("id")).ToUnsignedInt(),
				wxEmptyString
			);
			counter++;
		}
		query.FreeResult();
	}

	if(kind == ConstantsLibrary::CLIENT_KIND) {
		DSPostgreSQLQuery query(ndb);
		query.Prepare(GetResourceByName(_N("Orders.ReferenceByClientId")));
		query.Bind(_N("clientId"), primaryKey);
		query.Exec();
		while(query.NextRecord()) {
			referencesType->Append(
				ConstantsLibrary::ORDER_KIND,
				query.GetField(_N("id")).ToUnsignedInt(),
				wxEmptyString
			);
			counter++;
		}
		query.FreeResult();
	}

	if(kind == ConstantsLibrary::ORDER_KIND) {
		DSPostgreSQLQuery query(ndb);
		query.Prepare(GetResourceByName(_N("Orders.ReferenceByOrderId")));
		query.Bind(_N("orderId"), primaryKey);
		query.Exec();
		while(query.NextRecord()) {
			referencesType->Append(
				ConstantsLibrary::ORDER_KIND,
				query.GetField(_N("id")).ToUnsignedInt(),
				wxEmptyString
			);
			counter++;
		}
		query.FreeResult();
	}

	return counter;
}

bool OrderController::IsInvoiced(const unsigned int orderId) {
	OrderController orderController(rit);
	orderController.Get(DSCasts::ToString(orderId));
	if(orderController.Run()) {
		OrderEntity orderEntity(rit);
		orderController.Get(&orderEntity);
		orderController.Reset();
		return orderEntity.Get(_N("invoiceId"))->ToUnsignedInt() != 0;
	}

	return false;
}

HtmlPrintoutLibrary *OrderController::PrintStatistic(
	const unsigned int productId,
	const unsigned int clientId,
	const wxDateTime &from,
	const wxDateTime &to,
	DSListCtrl *listCtrl
) {
	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Statistic"), rit);

	DSTemplate statisticTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("Statistic"));
	statisticTemplate.SetVariable(_N("table"), htmlPrintoutLibrary->CreateTableByListCtrl(listCtrl));
	statisticTemplate.Parse();
	htmlPrintoutLibrary->SetWithContainer(statisticTemplate.Get());

	// Header
	DSDateTime fromDateTime;
	fromDateTime.SetWxDateTime(from);
	DSDateTime toDateTime;
	toDateTime.SetWxDateTime(to);

	DSTemplate statisticHeaderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("StatisticHeader"));
	statisticHeaderTemplate.SetVariable(_N("clientId"), DSCasts::ToString(clientId), true);
	statisticHeaderTemplate.SetVariable(_N("productId"), DSCasts::ToString(productId), true);
	statisticHeaderTemplate.SetVariable(_N("from"), fromDateTime.ToDatabaseDate(), true);
	statisticHeaderTemplate.SetVariable(_N("to"), toDateTime.ToDatabaseDate(), true);
	statisticHeaderTemplate.Parse();
	htmlPrintoutLibrary->SetHeader(statisticHeaderTemplate.Get());

	// Footer
	DSTemplate statisticFooterTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("StatisticFooter"));
	statisticFooterTemplate.Parse();
	htmlPrintoutLibrary->SetFooter(statisticFooterTemplate.Get());

	return htmlPrintoutLibrary;
}

HtmlPrintoutLibrary *OrderController::Print(OrderEntity *orderEntity, const bool delivery, const bool asCopy) {
	wxString clientId(orderEntity->Get(_N("clientId"))->ToString());
	if(!orderEntity->Get(_N("ownClientId"))->ToString().IsEmpty())
		clientId = orderEntity->Get(_N("ownClientId"))->ToString();

	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Order"), rit);

	EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());
	EnterprisePreferencesController enterprisePreferencesController(rit->GetDatabase());
	enterprisePreferencesController.Get();
	if(enterprisePreferencesController.Run()) {
		enterprisePreferencesController.Get(&enterprisePreferencesEntity);
		enterprisePreferencesController.Reset();
	}

	DSTemplate orderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(delivery ? _N("OrderDelivery") : _N("Order"));
	orderTemplate.SetEntity(orderEntity);
	orderTemplate.SetEntity(&enterprisePreferencesEntity);

	orderTemplate.SetVariable(_N("clientId"), clientId);

	unsigned int invoiceId = orderEntity->Get(_N("invoiceId"))->ToUnsignedInt();
	
	orderTemplate.SetVariable(_N("prepandText"), nl2br(HtmlSpecialChars(orderEntity->Get(_N("prepandText"))->ToString())));
	orderTemplate.SetVariable(_N("appendText"), nl2br(HtmlSpecialChars(orderEntity->Get(_N("appendText"))->ToString())));

	DSCurrency sumBeforeTax;
	DSCurrency sumFullTax;
	DSCurrency sumReducedTax;
	DSCurrency sumAfterTax;
	DSCurrency fullWeight;
	DSCurrency fullAmount;

	orderTemplate.SetDynamicVariable(_N("positions"), PrintPositions(
		htmlPrintoutLibrary,
		orderEntity->GetPositions(),
		delivery,
		sumBeforeTax,
		sumFullTax,
		sumReducedTax,
		sumAfterTax,
		fullWeight,
		fullAmount
	));

	orderTemplate.SetDynamicVariable(_N("linked"), PrintLinkedOrders(
		htmlPrintoutLibrary,
		orderEntity->GetLinkedOrders(),
		delivery,
		sumBeforeTax,
		sumFullTax,
		sumReducedTax,
		sumAfterTax,
		fullWeight,
		fullAmount
	));

	orderTemplate.SetVariable(_N("sumBeforeTax"), sumBeforeTax.ToString(), true);
	orderTemplate.SetVariable(_N("sumFullTax"), sumFullTax.ToString(), true);
	orderTemplate.SetVariable(_N("sumReducedTax"), sumReducedTax.ToString(), true);
	orderTemplate.SetVariable(_N("sumAfterTax"), sumAfterTax.ToString(), true);
	orderTemplate.SetVariable(_N("fullTaxPercent"), orderEntity->Get(_N("fullTaxPercent"))->ToCurrency().ToString(), true);
	orderTemplate.SetVariable(_N("reducedTaxPercent"), orderEntity->Get(_N("reducedTaxPercent"))->ToCurrency().ToString(), true);
	orderTemplate.SetVariable(_N("bankData"), orderEntity->Get(_N("payment"))->ToUnsignedInt() == ConstantsLibrary::PAYMENT_DEBIT ? _N("(") + orderEntity->GetBankString() + _N(")") : wxString(), true);
	orderTemplate.SetVariable(_N("payment"), ConstantsLibrary::PaymentToString(orderEntity->Get(_N("payment"))->ToUnsignedInt()), true);
	orderTemplate.SetVariable(_N("condition"), orderEntity->Get(_N("condition"))->ToString(), true);
	orderTemplate.SetVariable(_N("fullAmount"), fullAmount.ToString());
	orderTemplate.SetVariable(_N("fullWeight"), Tools::RemoveLastZeros(fullWeight.ToString()));

	orderTemplate.Parse();
	htmlPrintoutLibrary->SetWithContainer(orderTemplate.Get());

	// Header
	DSTemplate orderHeaderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(delivery ? _N("OrderDeliveryHeader") : _N("OrderHeader"));
	orderHeaderTemplate.SetEntity(orderEntity);
	orderHeaderTemplate.SetEntity(&enterprisePreferencesEntity);
	orderHeaderTemplate.SetVariable(_N("clientId"), clientId);

	orderHeaderTemplate.SetVariable(_N("salesTaxIdentificationNumber"), Tools::Description(enterprisePreferencesEntity.Get(_N("salesTaxIdentificationNumber"))->ToString(), _("n/a")), true);
	orderHeaderTemplate.SetVariable(_N("taxNumber"), Tools::Description(enterprisePreferencesEntity.Get(_N("taxNumber"))->ToString(), _("n/a")), true);

	orderHeaderTemplate.SetVariable(_N("orderSalesTaxIdentificationNumber"), Tools::Description(orderEntity->Get(_N("salesTaxIdentificationNumber"))->ToString(), _("n/a")), true);
	orderHeaderTemplate.SetVariable(_N("orderTaxNumber"), Tools::Description(orderEntity->Get(_N("taxNumber"))->ToString(), _("n/a")), true);
	orderHeaderTemplate.SetVariable(_N("invoiceDate"), Tools::Description(orderEntity->Get(_N("invoiceDate"))->ToString(), _("n/a")), true);
	wxString when(orderEntity->Get(_N("when"))->ToString());
	if(when.Find(_N(" ")) != wxNOT_FOUND)
		when = when.Mid(0, when.Find(_N(" ")));
	orderHeaderTemplate.SetVariable(_N("when"), Tools::Description(when, _("n/a")), true);
	orderHeaderTemplate.SetVariable(_N("delivery"), Tools::Description(orderEntity->Get(_N("delivery"))->ToString(), _("n/a")), true);
	orderHeaderTemplate.SetVariable(_N("orderNumber"), Tools::Description(orderEntity->Get(_N("orderNumber"))->ToString(), _("n/a")), true);

	orderHeaderTemplate.SetVariable(_N("orderId"),   invoiceId != 0 ? DSCasts::ToString(invoiceId) : orderEntity->Get(_N("orderId"))->ToString());
	orderHeaderTemplate.SetVariable(_N("invoiceId"), invoiceId != 0 ? DSCasts::ToString(invoiceId) : wxString(_("n/a")));

	wxString kind(ConstantsLibrary::OrderKindToString(orderEntity->Get(_N("kind"))->ToUnsignedInt()));
	if(delivery) {
		kind = _("Delivery");
	} else  {
		if(orderEntity->Get(_N("invoiceId"))->ToUnsignedInt() != 0) {
			switch(orderEntity->Get(_N("kind"))->ToUnsignedInt()) {
				case ConstantsLibrary::ORDER_KIND_ORDER:
					kind = _("Bill");
					break;
				case ConstantsLibrary::ORDER_KIND_CREDIT:
					kind = _("Credit");
					break;
			}
		}
	}

	orderHeaderTemplate.SetVariable(_N("kind"), kind, true);
	orderHeaderTemplate.SetVariable(_N("copy"), asCopy ? _N("(COPY)") : _N(""));

	wxString addressPrefix(_N("bill"));
	if(delivery) {
		if(
			!orderEntity->Get(_N("billAddressSalutation"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressTitle"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressFirstname"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressName1"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressName2"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressName3"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressName4"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressAddress"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressNumber"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressZipcode"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressCity"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressFederalState"))->ToString().IsEmpty() ||
			!orderEntity->Get(_N("billAddressLand"))->ToString().IsEmpty()
		) {
			addressPrefix = _N("shipping");
		}
	}

	orderHeaderTemplate.SetVariable(
		_N("shippingAddress"),
		NameType(
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressSalutation"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressTitle"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressFirstname"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressName1"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressName2"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressName3"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressName4"))->ToString())
		).GetConcatedFullName(_N("<br>")) +
		_N("<br>") +
		AddressType(
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressAddress"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressNumber"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressZipcode"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressCity"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressFederalState"))->ToString()),
			DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressLand"))->ToString())
		).GetConcatedAddress(_N("<br>"))
	);
	orderHeaderTemplate.SetVariable(
		_N("translatedShippingAddress"),
		nl2br(DSCgi::HtmlSpecialChars(orderEntity->Get(addressPrefix + _N("AddressTranslated"))->ToString()))
	);

	orderHeaderTemplate.Parse();
	htmlPrintoutLibrary->SetHeader(orderHeaderTemplate.Get());

	// Footer
	DSTemplate orderFooterTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(delivery ? _N("OrderDeliveryFooter") : _N("OrderFooter"));
	orderFooterTemplate.SetEntity(orderEntity);
	orderFooterTemplate.SetEntity(&enterprisePreferencesEntity);
	orderFooterTemplate.SetVariable(_N("clientId"), clientId);
	orderFooterTemplate.Parse();
	htmlPrintoutLibrary->SetFooter(orderFooterTemplate.Get());

	return htmlPrintoutLibrary;
}

wxString OrderController::PrintPositions(
	HtmlPrintoutLibrary *htmlPrintoutLibrary,
	OrderPositionEntities *orderPositionEntities,
	const bool delivery,
	DSCurrency &sumBeforeTax,
	DSCurrency &sumFullTax,
	DSCurrency &sumReducedTax,
	DSCurrency &sumAfterTax,
	DSCurrency &fullWeight,
	DSCurrency &fullAmount
) {
	DSTemplate orderRowTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(delivery ? _N("OrderDeliveryRow") : _N("OrderRow"));

	DSCurrency amount;
	DSCurrency price;
	DSCurrency fullPrice;
	DSCurrency reduction;
	DSCurrency beforeTax;
	DSCurrency tax;
	DSCurrency afterTax;

	unsigned int taxLevel = 0;
	wxString taxLevelString;
	wxString articleNumber;

	unsigned int position = 1;
	OrderPositionEntity *orderPositionEntity = NULL;
	const unsigned int count = orderPositionEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		orderPositionEntity = orderPositionEntities->Get(i);
		if(!orderPositionEntity)
			continue;

		articleNumber = orderPositionEntity->Get(_N("articleNumber"))->ToString();
		amount        = orderPositionEntity->Get(_N("amount"))->ToCurrency();
		price         = orderPositionEntity->Get(_N("price"))->ToCurrency();
		fullPrice     = price * amount;
		reduction     = orderPositionEntity->Get(_N("reduction"))->ToCurrency();
		beforeTax     = orderPositionEntity->Get(_N("beforeTax"))->ToCurrency();
		tax           = orderPositionEntity->Get(_N("tax"))->ToCurrency();
		afterTax      = orderPositionEntity->Get(_N("afterTax"))->ToCurrency();
		fullWeight   += orderPositionEntity->Get(_N("weight"))->ToCurrency() * amount;
		fullAmount   += amount;

		orderRowTemplate.SetVariable(_N("position"),    DSCasts::ToString(position++));
		orderRowTemplate.SetVariable(_N("productId"),   articleNumber.IsEmpty() ? orderPositionEntity->Get(_N("productId"))->ToString() : articleNumber, true);
		orderRowTemplate.SetVariable(_N("name1"),       orderPositionEntity->Get(_N("name1"))->ToString(), true);
		orderRowTemplate.SetVariable(_N("name2"),       orderPositionEntity->Get(_N("name2"))->ToString(), true);
		orderRowTemplate.SetVariable(_N("name3"),       orderPositionEntity->Get(_N("name3"))->ToString(), true);
		orderRowTemplate.SetVariable(_N("name4"),       orderPositionEntity->Get(_N("name4"))->ToString(), true);
		orderRowTemplate.SetVariable(_N("name"),        nl2br(HtmlSpecialChars(CreateProductName(*orderPositionEntity))));
		orderRowTemplate.SetVariable(_N("shortName"),   orderPositionEntity->Get(_N("shortName"))->ToString(), true);
		orderRowTemplate.SetVariable(_N("description"), nl2br(HtmlSpecialChars(orderPositionEntity->Get(_N("description"))->ToString())));
		orderRowTemplate.SetVariable(_N("amount"),      amount.ToString(), true);
		orderRowTemplate.SetVariable(_N("package"),     orderPositionEntity->Get(_N("package"))->ToString(), true);
		orderRowTemplate.SetVariable(_N("price"),       price.ToString(), true);
		orderRowTemplate.SetVariable(_N("fullPrice"),   fullPrice.ToString(), true);
		orderRowTemplate.SetVariable(_N("reduction"),   reduction.ToString(), true);
		orderRowTemplate.SetVariable(_N("beforeTax"),   beforeTax.ToString(), true);
		orderRowTemplate.SetVariable(_N("tax"),         tax.ToString(), true);
		orderRowTemplate.SetVariable(_N("afterTax"),    afterTax.ToString(), true);
		orderRowTemplate.SetVariable(_N("afterTax"),    afterTax.ToString(), true);
		orderRowTemplate.SetVariable(_N("text"),        nl2br(HtmlSpecialChars(orderPositionEntity->Get(_N("text"))->ToString())));
		orderRowTemplate.SetVariable(_N("fullWeight"),  Tools::RemoveLastZeros(DSCasts::ToString(amount * orderPositionEntity->Get(_N("weight"))->ToDouble())), true);
		orderRowTemplate.SetVariable(_N("stock"),       nl2br(HtmlSpecialChars(orderPositionEntity->Get(_N("stock"))->ToString())));

		taxLevel = orderPositionEntity->Get(_N("taxLevel"))->ToUnsignedInt();
		switch(taxLevel) {
			case 1:
				taxLevelString = _("R");
				break;
			case 2:
				taxLevelString = _("N");
				break;
			case 0:
			default:
				taxLevelString = _("F");
				break;
		}
		orderRowTemplate.SetVariable(_N("taxLevelString"), taxLevelString);

		orderRowTemplate.Parse();

		sumBeforeTax += beforeTax;
		sumAfterTax  += afterTax;
		if(taxLevel == 0)
			sumFullTax    += tax;
		else
			sumReducedTax += tax;
	}

	return orderRowTemplate.Get();
}

wxString OrderController::PrintLinkedOrders(
	HtmlPrintoutLibrary *htmlPrintoutLibrary,
	OrderEntities *linkedOrderEntities,
	const bool delivery,
	DSCurrency &sumBeforeTax,
	DSCurrency &sumFullTax,
	DSCurrency &sumReducedTax,
	DSCurrency &sumAfterTax,
	DSCurrency &fullWeight,
	DSCurrency &fullAmount
) {
	const unsigned int linkedOrdersCount = linkedOrderEntities->GetCount();
	if(linkedOrdersCount == 0)
		return wxEmptyString;

	DSTemplate linkedOrderRowTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(delivery ? _N("OrderLinkedDeliveryRow") : _N("OrderLinkedRow"));

	OrderEntity *linkedOrderEntity = NULL;
	for(unsigned int i = 0; i < linkedOrdersCount; i++) {
		linkedOrderEntity = linkedOrderEntities->Get(i);
		if(!linkedOrderEntity)
			continue;

		linkedOrderRowTemplate.SetEntity(linkedOrderEntity);
		linkedOrderRowTemplate.SetDynamicVariable(_N("positions"), PrintPositions(
			htmlPrintoutLibrary,
			linkedOrderEntity->GetPositions(),
			delivery,
			sumBeforeTax,
			sumFullTax,
			sumReducedTax,
			sumAfterTax,
			fullWeight,
			fullAmount
		));
		linkedOrderRowTemplate.Parse();
	}

	return linkedOrderRowTemplate.Get();
}

bool OrderController::IncrementPrints(const unsigned int orderId) {
	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Orders.IncrementPrints")));
	query.Bind(_N("orderId"), orderId);
	return query.Exec(false);
}

unsigned int OrderController::GetPrints(const unsigned int orderId) {
	unsigned int prints = 0;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Orders.GetPrints")));
	query.Bind(_N("orderId"), orderId);
	query.Exec();
	if(query.NextRecord())
		prints = query.GetField(_N("prints")).ToUnsignedInt();
	query.FreeResult();

	return prints;
}

OrderEntity *OrderController::GetDirect(RuntimeInformationType *rit, const unsigned int id) {
	OrderEntity *orderEntity = NULL;
	OrderController orderController(rit);
	orderController.Get(DSCasts::ToString(id));
	if(orderController.Run()) {
		orderEntity = new OrderEntity(rit);
		orderController.Get(orderEntity);
		orderController.Reset();

		LinkedFileController linkedFileController(rit, _N("linked_files_orders"));
		linkedFileController.Load(
			orderEntity->Get(_N("orderId"))->ToUnsignedInt(),
			*orderEntity->GetLinkedFileEntities()
		);

		orderController.LoadKeysValues(
			orderEntity->Get(_N("orderId"))->ToUnsignedInt(),
			_N("keys_values_orders"),
			*orderEntity->GetKeysValuesType()
		);
	}
	return orderEntity;
}

bool OrderController::SetPayed(const unsigned int orderId, const bool payed, const DSDateTime &payedTime) {
	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(payed ? _N("Orders.Payed") : _N("Orders.NotPayed")));
	query.Bind(_N("orderId"), orderId);
	if(payed)
		query.Bind(_N("payedTime"), payedTime.ToDatabaseDate());
	return query.Exec(false);
}

wxString OrderController::GetPayed(const unsigned int orderId) {
	wxString payed;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Orders.GetPayed")));
	query.Bind(_N("orderId"), orderId);
	query.Exec();
	if(query.NextRecord() && !query.IsNull(_N("payed"))) {
		payed = query.GetField(_N("payed")).ToString();
		payed = payed.Mid(0, payed.Find(_N(".")));
	}
	query.FreeResult();

	return payed;
}

wxString OrderController::CreateProductName(OrderPositionEntity &orderPositionEntity, const wxString &sep) {
	const unsigned int count = 4;

	wxString tmp;
	wxString name;
	for(unsigned int i = 1; i <= count; i++) {
		tmp = orderPositionEntity.Get(_N("name") + DSCasts::ToString(i))->ToString();
		if(!tmp.IsEmpty())
			name += tmp + sep;
	}

	if(name.Length() >= sep.Length())
		name = name.Mid(0, name.Length() - sep.Length());

	return name;
}

bool OrderController::IsLinked(const unsigned int orderId, OrderEntities *linkedOrderEntities) {
	const unsigned int count       = linkedOrderEntities ? linkedOrderEntities->GetCount() : 0U;
	unsigned int linkedOrderId     = 0;
	OrderEntity *linkedOrderEntity = NULL;
	bool found                     = false;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Orders.IsLinked")));
	query.Bind(_N("orderId"), orderId);
	query.Exec();
	while(query.NextRecord()) {
		found         = false;
		linkedOrderId = query.GetField(_N("orderId")).ToUnsignedInt() > 0;

		for(unsigned int i = 0; i < count; i++) {
			linkedOrderEntity = linkedOrderEntities->Get(i);
			if(linkedOrderId == linkedOrderEntity->Get(_N("orderId"))->ToUnsignedInt())
				found = true;
		}

		if(!found && orderId == linkedOrderId) {
			found = true;
			break;
		}
	}
	query.FreeResult();

	return found;
}

bool OrderController::SetMonitionState(const unsigned int orderId, bool increment, int *newMonitionState) {
	OrderController orderController(rit);
	orderController.Get(DSCasts::ToString(orderId));
	if(orderController.Run()) {
		OrderEntity orderEntity(rit);
		orderController.Get(&orderEntity);
		orderController.Reset();

		int monitionStates = 3;
		int monitionState  = orderEntity.Get(_N("monitionState"))->ToInt();

		if(!increment && monitionState == 0)
			return true;

		CommonPreferencesController commonPreferencesController(rit->GetDatabase());
		commonPreferencesController.Get();
		if(commonPreferencesController.Run()) {
			CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());
			commonPreferencesController.Get(&commonPreferencesEntity);
			commonPreferencesController.Reset();

			monitionStates = commonPreferencesEntity.Get(_N("monitionStates"))->ToInt();
		}

		if(increment && monitionState >= monitionStates)
			return true;

		if(increment)
			monitionState++;
		else
			monitionState--;

		if(newMonitionState)
			*newMonitionState = monitionState;

		DSPostgreSQLQuery query(rit->GetDatabase());
		query.Prepare(GetResourceByName(_N("Orders.SetMonition")));
		query.Bind(_N("monitionState"), monitionState);
		query.Bind(_N("orderId"), orderId);
		return query.Exec(false);
	}

	return false;
}

wxString OrderController::GetReminderDescription(const unsigned int id) {
	OrderController orderController(rit);
	orderController.Get(DSCasts::ToString(id));
	if(orderController.Run()) {
		OrderEntity orderEntity(rit);
		orderController.Get(&orderEntity);
		orderController.Reset();

		return ConstantsLibrary::KindToString(orderEntity.Get(_N("kind"))->ToUnsignedInt());
	}
	return wxEmptyString;
}
