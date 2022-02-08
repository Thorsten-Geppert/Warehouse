#include "VoucherController.h"
#include <DSPostgreSQLQuery.h>
#include <DSCgi.h>
#include "ConstantsLibrary.h"
#include "LinkedFileController.h"
#include "Tools.h"
#include "VoucherPositionController.h"
#include "VoucherPositionEntity.h"
#include "VoucherPositionEntities.h"

VoucherController::VoucherController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("vouchers"), // Table
	_N("voucherId"),  // Primary key
	_N("invoiceDate"),
	DESC
) {
	LoadResource(rit->GRP(_N("sql"), _N("Vouchers.res")));
}

void VoucherController::Find(VoucherEntity *voucherEntity) {
	const wxString voucherId(WildCard(voucherEntity->Get(_N("voucherId"))->ToString()));
	const wxString description(WildCard(voucherEntity->Get(_N("description"))->ToString()));
	const wxString invoiceId(WildCard(voucherEntity->Get(_N("invoiceId"))->ToString()));
	const wxString company(WildCard(voucherEntity->Get(_N("company"))->ToString()));
	const wxString notes(WildCard(voucherEntity->Get(_N("notes"))->ToString()));
	const wxString year(WildCard(DSCasts::ToString(voucherEntity->GetSearchYear()), true));
	
	DSQueryParser parser;
	parser.Prepare(GetResourceByName(_N("Vouchers.Find")));
	parser.Bind(_N("table"), GetTable(), false);
	parser.Bind(_N("voucherId"), voucherId);
	parser.Bind(_N("description"), description);
	parser.Bind(_N("invoiceId"), invoiceId);
	parser.Bind(_N("company"), company);
	parser.Bind(_N("notes"), notes);
	parser.Bind(_N("year"), year);
	Create(parser.GetParsedQuery());
}

HtmlPrintoutLibrary *VoucherController::Print(VoucherEntity *voucherEntity) {
	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Voucher"), rit);

	DSTemplate voucherTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("Voucher"));
	voucherTemplate.SetEntity(voucherEntity);

	DSTemplate voucherPositionTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("VoucherPosition"));

	wxString description;
	VoucherPositionEntities *voucherPositionEntities = voucherEntity->GetVoucherPositions();
	const unsigned int count = voucherPositionEntities->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		voucherPositionTemplate.SetEntity(voucherPositionEntities->Get(i));
		voucherPositionTemplate.SetVariable(_N("description"), nl2br(voucherPositionEntities->Get(i)->Get(_N("description"))->ToString()));
		voucherPositionTemplate.SetVariable(_N("tax"), ConstantsLibrary::TaxToString(voucherPositionEntities->Get(i)->Get(_N("tax"))->ToUnsignedInt()));
		voucherPositionTemplate.SetVariable(_N("taxPrice"), voucherPositionEntities->Get(i)->Get(_N("taxPrice"))->ToCurrency().ToString());
		voucherPositionTemplate.SetVariable(_N("price"), voucherPositionEntities->Get(i)->Get(_N("price"))->ToCurrency().ToString());
		voucherPositionTemplate.Parse();
	}
	voucherTemplate.SetDynamicVariable(_N("positions"), voucherPositionTemplate.Get());

	// Linked files
	LinkedFileController linkedFileController(rit, _N("linked_files_vouchers"));
	voucherTemplate.SetDynamicVariable(_N("linkedFiles"), voucherEntity->GetLinkedFileEntities()->GetCount() == 0 ? wxString(_("No linked files exist")) : linkedFileController.Print(*htmlPrintoutLibrary, *voucherEntity->GetLinkedFileEntities()));

	// More data
	voucherTemplate.SetDynamicVariable(_N("moreData"), voucherEntity->GetKeysValuesType()->GetCount() == 0 ? wxString(_("No more data exist")) : PrintKeysValues(*htmlPrintoutLibrary, *voucherEntity->GetKeysValuesType()));

	voucherTemplate.Parse();
	htmlPrintoutLibrary->SetWithContainer(voucherTemplate.Get());

	// Header
	DSTemplate voucherHeaderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("VoucherHeader"));
	voucherHeaderTemplate.SetEntity(voucherEntity);
	voucherHeaderTemplate.SetVariable(_N("company"), nl2br(HtmlSpecialChars(voucherEntity->Get(_N("company"))->ToString())));
	voucherHeaderTemplate.SetVariable(_N("notes"), nl2br(HtmlSpecialChars(voucherEntity->Get(_N("notes"))->ToString())));
	voucherHeaderTemplate.Parse();
	htmlPrintoutLibrary->SetHeader(voucherHeaderTemplate.Get());

	// Footer
	DSTemplate voucherFooterTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("VoucherFooter"));
	voucherFooterTemplate.Parse();
	htmlPrintoutLibrary->SetFooter(voucherFooterTemplate.Get());

	return htmlPrintoutLibrary;
}

VoucherEntity *VoucherController::GetDirect(RuntimeInformationType *rit, const unsigned int id) {
	VoucherEntity *voucherEntity = NULL;
	VoucherController voucherController(rit);
	voucherController.Get(DSCasts::ToString(id));
	if(voucherController.Run()) {
		voucherEntity = new VoucherEntity(rit);
		voucherController.Get(voucherEntity);
		voucherController.Reset();

		VoucherPositionEntities *voucherPositionEntities = voucherEntity->GetVoucherPositions();
		VoucherPositionEntity *voucherPositionEntity = NULL;
		VoucherPositionController voucherPositionController(rit);
		voucherPositionController.Get(_N("voucherId"), DSCasts::ToString(id));
		while(voucherPositionController.Run()) {
			voucherPositionEntity = new VoucherPositionEntity(rit);
			voucherPositionController.Get(voucherPositionEntity);
			voucherPositionEntities->Add(voucherPositionEntity);
		}

		LinkedFileController linkedFileController(rit, _N("linked_files_vouchers"));
		linkedFileController.Load(
			voucherEntity->Get(_N("voucherId"))->ToUnsignedInt(),
			*voucherEntity->GetLinkedFileEntities()
		);
		
		voucherController.LoadKeysValues(
			voucherEntity->Get(_N("voucherId"))->ToUnsignedInt(),
			_N("keys_values_vouchers"),
			*voucherEntity->GetKeysValuesType()
		);
	}
	return voucherEntity;
}

void VoucherController::Change(VoucherEntity *voucherEntity) {
	if(voucherEntity) {
		voucherEntity->SetValue(_N("voucherId"), 0);
		voucherEntity->SetValue(_N("createdAt"), _N("NOW()"));
		voucherEntity->SetValue(_N("changeAt"), _N("NOW()"));
		voucherEntity->GetLinkedFileEntities()->ResetId();

		VoucherPositionEntity *voucherPositionEntity = NULL;
		VoucherPositionEntities *voucherPositionEntities = voucherEntity->GetVoucherPositions();
		if(voucherPositionEntities) {
			const unsigned int count = voucherPositionEntities->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				voucherPositionEntity = voucherPositionEntities->Get(i);
				if(voucherPositionEntity)
					voucherPositionEntity->SetValue(_N("voucherPositionId"), 0U);
			}
		}
	}
}

VoucherEntity *VoucherController::Load(const unsigned int voucherId) {
	VoucherEntity *voucherEntity = NULL;

	if(voucherId == 0)
		return voucherEntity;
	
	VoucherController voucherController(rit);
	voucherController.Get(DSCasts::ToString(voucherId));
	if(voucherController.Run()) {
		voucherEntity = new VoucherEntity(rit);
		voucherController.Get(voucherEntity);
		voucherController.Reset();

		VoucherPositionEntities *voucherPositionEntities = voucherEntity->GetVoucherPositions();
		VoucherPositionEntity *voucherPositionEntity = NULL;
		VoucherPositionController voucherPositionController(rit);
		voucherPositionController.Get(_N("voucherId"), DSCasts::ToString(voucherId));
		while(voucherPositionController.Run()) {
			voucherPositionEntity = new VoucherPositionEntity(rit);
			voucherPositionController.Get(voucherPositionEntity);
			voucherPositionEntities->Add(voucherPositionEntity);
		}

		LinkedFileController linkedFileController(rit, _N("linked_files_vouchers"));
		linkedFileController.Load(
			voucherEntity->Get(_N("voucherId"))->ToUnsignedInt(),
			*voucherEntity->GetLinkedFileEntities()
		);
		
		LoadKeysValues(
			voucherEntity->Get(_N("voucherId"))->ToUnsignedInt(),
			_N("keys_values_vouchers"),
			*voucherEntity->GetKeysValuesType()
		);
	}

	return voucherEntity;
}

bool VoucherController::Save(VoucherEntity *voucherEntity) {
	bool ok = true;

	DSPostgreSQLQuery query(ndb);
	query.Begin();

	VoucherController voucherController(rit);
	if(voucherEntity->Get(_N("voucherId"))->ToUnsignedInt()) {
		voucherEntity->ForceChanged();
		ok = voucherController.Update(voucherEntity, wxEmptyString, true, voucherEntity->GetUpdateExcludes(), &query);
	} else {
		ok = voucherController.Insert(voucherEntity, true, voucherEntity->GetInsertExcludes(), &query);
	}

	const unsigned int voucherId = voucherEntity->Get(_N("voucherId"))->ToUnsignedInt();

	if(ok) {
		LinkedFileController linkedFileController(rit, _N("linked_files_vouchers"));
		ok = linkedFileController.Save(
			query,
			voucherEntity->Get(_N("voucherId"))->ToUnsignedInt(),
			*voucherEntity->GetLinkedFileEntities()
		);
	}

	if(ok) {
		ok = SaveKeysValues(
			query,
			voucherEntity->Get(_N("voucherId"))->ToUnsignedInt(),
			_N("keys_values_vouchers"),
			*voucherEntity->GetKeysValuesType()
		);
	}

	if(ok) {
		VoucherPositionController voucherPositionController(rit);
		VoucherPositionEntity *voucherPositionEntity     = NULL;
		VoucherPositionEntities *voucherPositionEntities = voucherEntity->GetVoucherPositions();
		const unsigned int count = voucherPositionEntities->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			voucherPositionEntity = voucherPositionEntities->Get(i);
			if(!voucherPositionEntity)
				continue;

			voucherPositionEntity->SetValue(_N("voucherId"), voucherId);

			if(voucherPositionEntity->Get(_N("voucherPositionId"))->ToUnsignedInt() == 0)
				ok = voucherPositionController.Insert(voucherPositionEntity, true, NULL, &query);
			else
				ok = voucherPositionController.Update(voucherPositionEntity, wxEmptyString, true, NULL, &query);
			
			if(!ok)
				break;
		}
	}

	if(!ok || !query.Commit()) {
		query.Rollback();
		return false;
	} else {
		voucherEntity->GetLinkedFileEntities()->CleanRemoved();
	}

	return true;
}

void VoucherController::Statistics(
	const wxDateTime &from,
	const wxDateTime &to
) {
	DSDateTime fromDateTime;
	fromDateTime.SetWxDateTime(from);
	DSDateTime toDateTime;
	toDateTime.SetWxDateTime(to);

	DSQueryParser parser;
	parser.Prepare(GetResourceByName(_N("Vouchers.Statistics")));
	parser.Bind(_N("from"), fromDateTime.ToDatabaseDate());
	parser.Bind(_N("to"), toDateTime.ToDatabaseDate());

	Create(parser.GetParsedQuery());
}

void VoucherController::StatisticsGet(StatisticsEntity *statisticsEntity) {
	if(statisticsEntity) {
		DSCurrency beforeTax(query->GetField(_N("priceWithoutTax")).ToCurrency() * -1);
		DSCurrency fullTax(query->GetField(_N("fullTax")).ToCurrency() * -1);
		DSCurrency reducedTax(query->GetField(_N("reducedTax")).ToCurrency() * -1);
		DSCurrency afterTax(query->GetField(_N("priceWithTax")).ToCurrency() * -1);

		statisticsEntity->SetValue(_N("invoiceId"), query->GetField(_N("invoiceId")).ToUnsignedInt());
		statisticsEntity->SetValue(_N("invoiceDate"), query->GetField(_N("invoiceDate")).ToString());
		statisticsEntity->SetValue(_N("clientId"), wxString(_("n/a")));
		statisticsEntity->SetValue(_N("productId"), wxString(_("n/a")));
		statisticsEntity->SetValue(_N("orderId"), query->GetField(_N("voucherId")).ToUnsignedInt());
		statisticsEntity->SetValue(_N("taxLevel"), 0);
		statisticsEntity->SetValue(_N("beforeTax"), beforeTax);
		statisticsEntity->SetValue(_N("fullTax"), fullTax);
		statisticsEntity->SetValue(_N("reducedTax"), reducedTax);
		statisticsEntity->SetValue(_N("afterTax"), afterTax);
	}
}

wxString VoucherController::GetReminderDescription(const unsigned int id) {
	VoucherController voucherController(rit);
	voucherController.Get(DSCasts::ToString(id));
	if(voucherController.Run()) {
		VoucherEntity voucherEntity(rit);
		voucherController.Get(&voucherEntity);
		voucherController.Reset();

		return voucherEntity.Get(_N("description"))->ToString();
	}
	return wxEmptyString;
}
