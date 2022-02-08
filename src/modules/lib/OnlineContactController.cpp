#include "OnlineContactController.h"
#include <DSCgi.h>

OnlineContactController::OnlineContactController(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLEntityData(
	db,
	_N("online_contact_messages"),
	_N("id")
) {
}

bool OnlineContactController::Mail(const wxString &to, OnlineContactEntity *contact) {
	wxString message;

	message.Append(_("Anrede: ") + contact->Get(_N("salutation"))->ToString() + _N("\n"));
	message.Append(_("Titel: ") + contact->Get(_N("title"))->ToString() + _N("\n"));
	message.Append(_("Vorname: ") + contact->Get(_N("firstname"))->ToString() + _N("\n"));
	message.Append(_("Nachname: ") + contact->Get(_N("lastname"))->ToString() + _N("\n"));
	message.Append(_("Firma: ") + contact->Get(_N("company"))->ToString() + _N("\n"));
	message.Append(_("Strasse: ") + contact->Get(_N("street"))->ToString() + _N("\n"));
	message.Append(_("Plz: ") + contact->Get(_N("zipcode"))->ToString() + _N("\n"));
	message.Append(_("Ort: ") + contact->Get(_N("city"))->ToString() + _N("\n"));
	message.Append(_("Telefon: ") + contact->Get(_N("telephone"))->ToString() + _N("\n"));
	message.Append(_("Fax: ") + contact->Get(_N("fax"))->ToString() + _N("\n"));
	message.Append(_("E-Mail: ") + contact->Get(_N("email"))->ToString() + _N("\n"));
	message.Append(_("Internet: ") + contact->Get(_N("internet"))->ToString() + _N("\n"));
	message.Append(_("Betreff: ") + contact->Get(_N("subject"))->ToString() + _N("\n"));
	message.Append(_("Nachricht:\n\n") + contact->Get(_N("message"))->ToString());

	return DSCgi::Mail(
		to,
		_N("DynSoft.com Kontakt-E-Mail"),
		message,
		_N("From: info@dynsoft.com")
	) == 0;
}
