#include "BankConnectionEntity.h"
#include "ConstantsLibrary.h"

BankConnectionEntity::BankConnectionEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("bank_connections"),
	_N("bankConnectionId")
) {
	Set(_N("bankConnectionId"), 0U);
	Set(_N("clientId"));
	Set(_N("description"));
	Set(_N("bank"));
	Set(_N("bankCode"));
	Set(_N("accountNumber"));
	Set(_N("bic"));
	Set(_N("iban"));
	Set(_N("bankOwner"));
	Set(_N("referenceNumber"));
	Set(_N("referenceDate"));
	Set(_N("referenceState"), 0U); // 0 = once, 1 = mutliple
	Set(_N("referenceAccount"), 0U); // 0 = B2B, 1 = B2C
	Set(_N("notes"));
	Set(_N("active"), true);
}

void BankConnectionEntity::SetReferenceDate(DSDateTime dt) {
	SetValue(_N("referenceDate"), dt.ToDatabaseDate());
}

void BankConnectionEntity::SetReferenceDate(wxDateTime dt) {
	DSDateTime tmp;
	tmp.SetWxDateTime(dt);
	SetValue(_N("referenceDate"), tmp.ToDatabaseDate());
}

DSDateTime BankConnectionEntity::GetReferenceDate() {
	DSDateTime tmp;
	tmp.SetAmericanDate(Get(_N("referenceDate"))->ToString());
	return tmp;
}

wxString BankConnectionEntity::GetReferenceStateString() {
	const unsigned int position = Get(_N("referenceState"))->ToUnsignedInt();
	wxArrayString tmp(ConstantsLibrary::SepaStateToArrayString());
	if(position < tmp.GetCount())
		return tmp[position];
	return _("unknown");
}

wxString BankConnectionEntity::GetReferenceAccountString() {
	const unsigned int position = Get(_N("referenceAccount"))->ToUnsignedInt();
	wxArrayString tmp(ConstantsLibrary::SepaAccountToArrayString());
	if(position < tmp.GetCount())
		return tmp[position];
	return _("unknown");
}

wxString BankConnectionEntity::GetNotesInOneLine(const wxString &replace) {
	wxString notes(Get(_N("notes"))->ToString());
	notes.Replace(_N("\r"), wxEmptyString);
	notes.Replace(_N("\n"), replace);
	return notes;
}
