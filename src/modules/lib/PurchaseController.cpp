#include "PurchaseController.h"
#include <DSCgi.h>
#include <DSDateTime.h>
#include "PurchasePositionController.h"
#include "PurchasePositionEntities.h"
#include "PurchasePositionEntity.h"
#include "ClientEntity.h"
#include "ClientController.h"
#include "LinkedFileController.h"
#include "Tools.h"

PurchaseController::PurchaseController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("purchases"), // Table
	_N("purchaseId"),  // Primary key
	_N("purchaseId"),
	ASC
) {
	LoadResource(rit->GRP(_N("sql"), _N("Purchases.res")));
}

wxString PurchaseController::GetReminderDescription(const unsigned int id) {
	PurchaseController purchaseController(rit);
	purchaseController.Get(DSCasts::ToString(id));
	if(purchaseController.Run()) {
		PurchaseEntity purchaseEntity(rit);
		purchaseController.Get(&purchaseEntity);
		purchaseController.Reset();

		return purchaseEntity.Get(_N("distributorId"))->ToString();
	}
	return wxEmptyString;
}

void PurchaseController::Get(PurchaseEntity *purchaseEntity, const bool justGetMainEntity) {
	Controller::Get(purchaseEntity);

	if(!justGetMainEntity) {
		bool found = false;
		unsigned int count = 0;
		PurchasePositionEntity *purchasePositionEntity = NULL;
		PurchasePositionEntities *purchasePositionEntities = purchaseEntity->GetPositions();
		PurchasePositionController purchasePositionController(rit);
		purchasePositionController.Get(_N("purchaseId"), purchaseEntity->Get(_N("purchaseId"))->ToString());
		while(purchasePositionController.Run()) {
			purchasePositionEntity = new PurchasePositionEntity(rit);
			purchasePositionController.Get(purchasePositionEntity);

			found = false;
			count = purchasePositionEntities->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				if(purchasePositionEntities->Get(i)->Get(_N("purchasePositionId"))->ToUnsignedInt() == purchasePositionEntity->Get(_N("purchasePositionId"))->ToUnsignedInt()) {
					found = true;
					break;
				}
			}

			if(!found)
				purchasePositionEntities->Add(purchasePositionEntity);
			else
				delete purchasePositionEntity;
		}

		LinkedFileController linkedFileController(rit, _N("linked_files_purchases"));
		linkedFileController.Load(
			purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt(),
			*purchaseEntity->GetLinkedFileEntities()
		);

		LoadKeysValues(
			purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt(),
			_N("keys_values_purchases"),
			*purchaseEntity->GetKeysValuesType()
		);
	}
}

bool PurchaseController::Save(PurchaseEntity *purchaseEntity, wxArrayString *purchasePositionsRemoved) {
	bool ok = false;

	wxArrayString insertExcludes(*purchaseEntity->GetInsertExcludes());
	wxArrayString updateExcludes(*purchaseEntity->GetUpdateExcludes());

	ClientController clientController(rit);
	clientController.Get(purchaseEntity->Get(_N("distributorId"))->ToUnsignedInt());
	if(clientController.Run()) {
		ClientEntity clientEntity(rit);
		clientController.Get(&clientEntity, true);
		clientController.Reset();
	} else {
		return false;
	}
	
	DSPostgreSQLQuery query(ndb);
	query.Begin();

	PurchaseController purchaseController(rit);
	if(purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt() == 0) {
		ok = purchaseController.Insert(purchaseEntity, true, &insertExcludes, &query);
	} else {
		purchaseEntity->ForceChanged();
		ok = purchaseController.Update(purchaseEntity, wxEmptyString, true, &updateExcludes, &query);
	}

	if(ok) {
		PurchasePositionEntities *purchasePositionEntities = purchaseEntity->GetPositions();
		PurchasePositionEntity   *purchasePositionEntity   = NULL;
		PurchasePositionController purchasePositionController(rit);
		unsigned int count = purchasePositionEntities->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			purchasePositionEntity = purchasePositionEntities->Get(i);
			if(purchasePositionEntity) {
				purchasePositionEntity->SetValue(_N("purchaseId"), purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt());

				if(purchasePositionEntity->Get(_N("purchasePositionId"))->ToUnsignedInt() == 0)
					ok = purchasePositionController.Insert(purchasePositionEntity, true, NULL, &query);
				else
					ok = purchasePositionController.Update(purchasePositionEntity, wxEmptyString, true, NULL, &query);

				if(!ok)
					break;
			}
		}

		if(ok && purchasePositionsRemoved) {
			PurchasePositionEntity tmpPurchasePositionEntity(rit);
			const unsigned int count = purchasePositionsRemoved->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				tmpPurchasePositionEntity.SetValue(_N("purchasePositionId"), (*purchasePositionsRemoved)[i]);
				ok = purchasePositionController.Delete(&tmpPurchasePositionEntity, wxEmptyString, &query);
				if(!ok)
					break;
			}
		}
	}

	if(ok) {
		LinkedFileController linkedFileController(rit, _N("linked_files_purchases"));
		ok = linkedFileController.Save(
			query,
			purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt(),
			*purchaseEntity->GetLinkedFileEntities()
		);
	}

	if(ok) {
		ok = SaveKeysValues(
			query,
			purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt(),
			_N("keys_values_purchases"),
			*purchaseEntity->GetKeysValuesType()
		);
	}

	if(!query.Commit()) {
		query.Rollback();
		return false;
	} else {
		if(purchasePositionsRemoved)
			purchasePositionsRemoved->Clear();
		purchaseEntity->GetLinkedFileEntities()->CleanRemoved();
	}

	return true;
}

void PurchaseController::Find(PurchaseEntity *purchaseEntity) {
	const wxString purchaseId(WildCard(purchaseEntity->Get(_N("purchaseId"))->ToString()));
	const wxString distributorId(WildCard(purchaseEntity->Get(_N("distributorId"))->ToString()));
	const wxString distributorAddress(WildCard(purchaseEntity->Get(_N("distributorAddress"))->ToString()));
	const wxString billingAddress(WildCard(purchaseEntity->Get(_N("billingAddress"))->ToString()));
	const wxString deliveryAddress(WildCard(purchaseEntity->Get(_N("deliveryAddress"))->ToString()));
	const wxString orderId(WildCard(purchaseEntity->Get(_N("orderId"))->ToString()));
	const wxString purchaser(WildCard(purchaseEntity->Get(_N("purchaser"))->ToString()));
	const wxString informations(WildCard(purchaseEntity->Get(_N("informations"))->ToString()));
	const wxString year(WildCard(DSCasts::ToString(purchaseEntity->GetSearchYear()), true));
	
	DSQueryParser parser;
	parser.Prepare(GetResourceByName(_N("Purchases.Find")));
	parser.Bind(_N("table"), GetTable(), false);
	parser.Bind(_N("purchaseId"), purchaseId);
	parser.Bind(_N("distributorId"), distributorId);
	parser.Bind(_N("distributorAddress"), distributorAddress);
	parser.Bind(_N("billingAddress"), billingAddress);
	parser.Bind(_N("deliveryAddress"), deliveryAddress);
	parser.Bind(_N("orderId"), orderId);
	parser.Bind(_N("purchaser"), purchaser);
	parser.Bind(_N("informations"), informations);
	parser.Bind(_N("year"), year);

	Create(parser.GetParsedQuery());
}

void PurchaseController::Change(PurchaseEntity *purchaseEntity) {
	DSDateTime dt;
	dt.SetWxDateTime(wxDateTime::Now());

	purchaseEntity->SetValue(_N("purchaseId"), 0U);
	purchaseEntity->SetValue(_N("orderId"), wxString());
	purchaseEntity->SetValue(_N("orderDate"), dt.ToDatabaseDate());
	purchaseEntity->SetValue(_N("createdAt"), _N("NOW()"));
	purchaseEntity->SetValue(_N("changeAt"), _N("NOW()"));

	purchaseEntity->Recalc();

	purchaseEntity->GetLinkedFileEntities()->ResetId();
}

PurchaseEntity *PurchaseController::GetDirect(RuntimeInformationType *rit, const unsigned int id) {
	PurchaseEntity *purchaseEntity = NULL;
	PurchaseController purchaseController(rit);
	purchaseController.Get(DSCasts::ToString(id));
	if(purchaseController.Run()) {
		purchaseEntity = new PurchaseEntity(rit);
		purchaseController.Get(purchaseEntity);
		purchaseController.Reset();

		LinkedFileController linkedFileController(rit, _N("linked_files_purchases"));
		linkedFileController.Load(
			purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt(),
			*purchaseEntity->GetLinkedFileEntities()
		);

		purchaseController.LoadKeysValues(
			purchaseEntity->Get(_N("purchaseId"))->ToUnsignedInt(),
			_N("keys_values_purchases"),
			*purchaseEntity->GetKeysValuesType()
		);
	}
	return purchaseEntity;
}

bool PurchaseController::SetDone(const unsigned int purchaseId, const bool done, const DSDateTime &doneTime) {
	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(done ? _N("Purchases.Done") : _N("Purchases.NotDone")));
	query.Bind(_N("purchaseId"), purchaseId);
	if(done)
		query.Bind(_N("doneTime"), doneTime.ToDatabaseDate());
	return query.Exec(false);
}

wxString PurchaseController::GetDone(const unsigned int purchaseId) {
	wxString done;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Purchases.GetDone")));
	query.Bind(_N("purchaseId"), purchaseId);
	query.Exec();
	if(query.NextRecord() && !query.IsNull(_N("done"))) {
		done = query.GetField(_N("done")).ToString();
		done = done.Mid(0, done.Find(_N(".")));
	}
	query.FreeResult();

	return done;
}

HtmlPrintoutLibrary *PurchaseController::Print(PurchaseEntity *purchaseEntity) {
	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Purchase"), rit);

	wxString generatedId(purchaseEntity->Get(_N("orderId"))->ToString());
	if(generatedId.IsEmpty())
		generatedId = purchaseEntity->Get(_N("purchaseId"))->ToString();
	
	DSDateTime orderDate;
	orderDate.SetAmericanDate(purchaseEntity->Get(_N("orderDate"))->ToString());

	const wxString distributorAddress(nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("distributorAddress"))->ToString())));
	const wxString billingAddress(nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("billingAddress"))->ToString())));
	wxString deliveryAddress(nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("deliveryAddress"))->ToString())));
	deliveryAddress.Trim().Trim(false);
	if(deliveryAddress.IsEmpty())
		deliveryAddress = billingAddress;

	// Body
	DSTemplate purchaseTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("Purchase"));
	purchaseTemplate.SetEntity(purchaseEntity);
	purchaseTemplate.SetVariable(_N("distributorAddress"), distributorAddress);
	purchaseTemplate.SetVariable(_N("billingAddress"), billingAddress);
	purchaseTemplate.SetVariable(_N("deliveryAddress"), deliveryAddress);
	purchaseTemplate.SetVariable(_N("informations"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("informations"))->ToString())));
	purchaseTemplate.SetVariable(_N("textHead"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("textHead"))->ToString())));
	purchaseTemplate.SetVariable(_N("textFoot"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("textFoot"))->ToString())));
	purchaseTemplate.SetVariable(_N("generatedId"), generatedId);
	purchaseTemplate.SetVariable(_N("orderDate"), orderDate.ToGermanDate());
	purchaseTemplate.SetVariable(_N("sum"), purchaseEntity->Get(_N("sum"))->ToCurrency().ToString());

	DSCurrency fullAmount;
	DSCurrency fullWeight;
	DSCurrency amount;
	wxString generatedArticleNumber;
	DSTemplate purchaseRowTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("PurchaseRow"));
	unsigned int position = 1;
	PurchasePositionEntity *purchasePositionEntity = NULL;
	PurchasePositionEntities *purchasePositionEntities = purchaseEntity->GetPositions();
	const unsigned int count = purchasePositionEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		purchasePositionEntity = purchasePositionEntities->Get(i);
		if(!purchasePositionEntity)
			continue;

		generatedArticleNumber = purchasePositionEntity->Get(_N("articleNumber"))->ToString();
		if(generatedArticleNumber.IsEmpty())
			generatedArticleNumber = purchasePositionEntity->Get(_N("productId"))->ToString();

		amount = purchasePositionEntity->Get(_N("amount"))->ToCurrency();

		purchaseRowTemplate.SetEntity(purchasePositionEntity);
		purchaseRowTemplate.SetVariable(_N("position"),               DSCasts::ToString(position++));
		purchaseRowTemplate.SetVariable(_N("generatedArticleNumber"), generatedArticleNumber, true);
		purchaseRowTemplate.SetVariable(_N("name1"),                  purchasePositionEntity->Get(_N("name1"))->ToString(), true);
		purchaseRowTemplate.SetVariable(_N("name2"),                  purchasePositionEntity->Get(_N("name2"))->ToString(), true);
		purchaseRowTemplate.SetVariable(_N("name3"),                  purchasePositionEntity->Get(_N("name3"))->ToString(), true);
		purchaseRowTemplate.SetVariable(_N("name4"),                  purchasePositionEntity->Get(_N("name4"))->ToString(), true);
		purchaseRowTemplate.SetVariable(_N("name"),                   nl2br(HtmlSpecialChars(CreateProductName(*purchasePositionEntity))));
		purchaseRowTemplate.SetVariable(_N("shortName"),              purchasePositionEntity->Get(_N("shortName"))->ToString(), true);
		purchaseRowTemplate.SetVariable(_N("description"),            nl2br(HtmlSpecialChars(purchasePositionEntity->Get(_N("description"))->ToString())));
		purchaseRowTemplate.SetVariable(_N("amount"),                 amount.ToString(), true);
		purchaseRowTemplate.SetVariable(_N("package"),                purchasePositionEntity->Get(_N("package"))->ToString(), true);
		purchaseRowTemplate.SetVariable(_N("fullWeight"),             Tools::RemoveLastZeros(DSCasts::ToString(amount * purchasePositionEntity->Get(_N("weight"))->ToDouble())), true);
		purchaseRowTemplate.SetVariable(_N("stock"),                  nl2br(HtmlSpecialChars(purchasePositionEntity->Get(_N("stock"))->ToString())));
		purchaseRowTemplate.SetVariable(_N("price"),                  purchasePositionEntity->Get(_N("price"))->ToCurrency().ToString());
		purchaseRowTemplate.SetVariable(_N("fullPrice"),              purchasePositionEntity->Get(_N("fullPrice"))->ToCurrency().ToString());
		purchaseRowTemplate.SetVariable(_N("text"),                   nl2br(DSCgi::HtmlSpecialChars(purchasePositionEntity->Get(_N("text"))->ToString())));
		purchaseRowTemplate.Parse();

		fullAmount += amount;
		fullWeight += amount * purchasePositionEntity->Get(_N("weight"))->ToDouble();
	}

	purchaseTemplate.SetDynamicVariable(_N("positions"), purchaseRowTemplate.Get());
	purchaseTemplate.SetVariable(_N("fullWeight"), fullWeight.ToString());
	purchaseTemplate.SetVariable(_N("fullAmount"), fullAmount.ToString());

	purchaseTemplate.Parse();
	htmlPrintoutLibrary->SetWithContainer(purchaseTemplate.Get());

	// Header
	DSTemplate purchaseHeaderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("PurchaseHeader"));
	purchaseHeaderTemplate.SetEntity(purchaseEntity);
	purchaseHeaderTemplate.SetVariable(_N("distributorAddress"), distributorAddress);
	purchaseHeaderTemplate.SetVariable(_N("billingAddress"), billingAddress);
	purchaseHeaderTemplate.SetVariable(_N("deliveryAddress"), deliveryAddress);
	purchaseHeaderTemplate.SetVariable(_N("informations"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("informations"))->ToString())));
	purchaseHeaderTemplate.SetVariable(_N("textHead"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("textHead"))->ToString())));
	purchaseHeaderTemplate.SetVariable(_N("textFoot"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("textFoot"))->ToString())));
	purchaseHeaderTemplate.SetVariable(_N("generatedId"), generatedId);
	purchaseHeaderTemplate.SetVariable(_N("orderDate"), orderDate.ToGermanDate());
	purchaseHeaderTemplate.SetVariable(_N("sum"), purchaseEntity->Get(_N("sum"))->ToCurrency().ToString());
	purchaseHeaderTemplate.Parse();
	htmlPrintoutLibrary->SetHeader(purchaseHeaderTemplate.Get());

	// Footer
	DSTemplate purchaseFooterTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("PurchaseFooter"));
	purchaseHeaderTemplate.SetEntity(purchaseEntity);
	purchaseFooterTemplate.SetVariable(_N("distributorAddress"), distributorAddress);
	purchaseFooterTemplate.SetVariable(_N("billingAddress"), billingAddress);
	purchaseFooterTemplate.SetVariable(_N("deliveryAddress"), deliveryAddress);
	purchaseFooterTemplate.SetVariable(_N("informations"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("informations"))->ToString())));
	purchaseFooterTemplate.SetVariable(_N("textHead"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("textHead"))->ToString())));
	purchaseFooterTemplate.SetVariable(_N("textFoot"), nl2br(DSCgi::HtmlSpecialChars(purchaseEntity->Get(_N("textFoot"))->ToString())));
	purchaseFooterTemplate.SetVariable(_N("generatedId"), generatedId);
	purchaseFooterTemplate.SetVariable(_N("orderDate"), orderDate.ToGermanDate());
	purchaseFooterTemplate.SetVariable(_N("sum"), purchaseEntity->Get(_N("sum"))->ToCurrency().ToString());
	purchaseFooterTemplate.Parse();
	htmlPrintoutLibrary->SetFooter(purchaseFooterTemplate.Get());

	return htmlPrintoutLibrary;
}

wxString PurchaseController::CreateProductName(PurchasePositionEntity &purchasePositionEntity, const wxString &sep) {
	const unsigned int count = 4;

	wxString tmp;
	wxString name;
	for(unsigned int i = 1; i <= count; i++) {
		tmp = purchasePositionEntity.Get(_N("name") + DSCasts::ToString(i))->ToString();
		if(!tmp.IsEmpty())
			name += tmp + sep;
	}

	if(name.Length() >= sep.Length())
		name = name.Mid(0, name.Length() - sep.Length());

	return name;
}
