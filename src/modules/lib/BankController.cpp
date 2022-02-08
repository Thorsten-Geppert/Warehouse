#include "BankController.h"
#include <DSCasts.h>
#include <DSPostgreSQLQuery.h>

BankController::BankController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("banks"), // Table
	_N("bankId"),
	_N("bank")
), ImportLibrary(
) {
	LoadResource(rit->GRP(_N("sql"), _N("Banks.res")));
}

void BankController::Find(BankEntity *bankEntity) {
	const wxString bank(WildCard(bankEntity->Get(_N("bank"))->ToString()));
	const wxString bankNumber(WildCard(bankEntity->Get(_N("bankCode"))->ToString()));
	const wxString bic(WildCard(bankEntity->Get(_N("bic"))->ToString()));
	
	DSQueryParser parser;
	parser.Prepare(GetResourceByName(_N("Banks.Find")));
	parser.Bind(_N("table"), GetTable(), false);
	parser.Bind(_N("bank"), bank, false);
	parser.Bind(_N("bankCode"), bankNumber, false);
	parser.Bind(_N("bic"), bic, false);
	Create(parser.GetParsedQuery());
}

BankEntity *BankController::GetDirect(RuntimeInformationType *rit, const unsigned int id) {
	BankEntity *bankEntity = NULL;
	BankController bankController(rit);
	bankController.Get(DSCasts::ToString(id));
	if(bankController.Run()) {
		bankEntity = new BankEntity(rit);
		bankController.Get(bankEntity);
		bankController.Reset();
	}
	return bankEntity;
}

bool BankController::ImportRun(const wxString &line) {
	if(line.IsEmpty())
		return true;
	
	const wxString bankCode(line.Mid(0, 8).Trim().Trim(false)); // Bankleitzahl
	//const wxString attribute(line.Mid(8, 1).Trim().Trim(false)); // Merkmal
	const wxString bank(line.Mid(9, 58).Trim().Trim(false)); // Bezeichnung
	//const wxString zipcode(line.Mid(67, 5).Trim().Trim(false)); // Postleitzahl
	//const wxString city(line.Mid(72, 35).Trim().Trim(false)); // Ort
	//const wxString description(line.Mid(107, 27).Trim().Trim(false)); // Kurzbezeichnung
	//const wxString pan(line.Mid(134, 5).Trim().Trim(false)); // PAN
	const wxString bic(line.Mid(139, 11).Trim().Trim(false)); // BIC
	//const wxString checkMethod(line.Mid(150, 2).Trim().Trim(false)); // Pruefzifferberechnungsmethode
	//const wxString dataNumber(line.Mid(152, 6).Trim().Trim(false)); // Datensatznummer
	//const wxString change(line.Mid(158, 1).Trim().Trim(false)); // Aenderungskennzeichen
	//const wxString deleted(line.Mid(159, 1).Trim().Trim(false)); // Bankleitzahlloeschung
	//const wxString next(line.Mid(160, 8).Trim().Trim(false)); // Nachfolge-Bankleitzahl

	if(ExistsWithBankAndBankCode(bank, bankCode, bic))
		return true;

	BankEntity bankEntity(rit);
	bankEntity.SetValue(_N("bank"), bank);
	bankEntity.SetValue(_N("bankCode"), bankCode);
	bankEntity.SetValue(_N("bic"), bic);

	return Insert(&bankEntity);
}

bool BankController::DeleteAll() {
	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Banks.DeleteAll")));
	query.Bind(_N("table"), GetTable(), false);
	return query.Exec(false);
}

bool BankController::ExistsWithBankAndBankCode(const wxString &bank, const wxString &bankCode, const wxString &bic) {
	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Banks.Exists")));
	query.Bind(_N("table"), GetTable(), false);
	query.Bind(_N("bank"), bank);
	query.Bind(_N("bankCode"), bankCode);
	query.Bind(_N("bic"), bic);
	query.Exec();
	query.NextRecord();
	const bool exists = query.GetField(_N("counter")).ToUnsignedInt() > 0;
	query.FreeResult();
	return exists;
}
