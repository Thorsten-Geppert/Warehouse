#include "ReminderType.h"
#include <DSCasts.h>

ReminderType::ReminderType() {
}

ReminderType::ReminderType(
	const unsigned int id,
	const wxString &username,
	const unsigned int module,
	const unsigned int reference,
	const wxString &description,
	const DSDateTime &dateTime,
	const wxString &notes
) {
	SetId(id);
	SetUsername(username);
	SetModule(module);
	SetReference(reference);
	SetDescription(description);
	SetDateTime(dateTime);
	SetNotes(notes);
}

void ReminderType::SetId(const unsigned int id) {
	this->id = id;
}

unsigned int ReminderType::GetId() const {
	return id;
}

void ReminderType::SetUsername(const wxString &username) {
	this->username = username;
}

wxString ReminderType::GetUsername() const {
	return username;
}

void ReminderType::SetModule(const unsigned int module) {
	this->module = module;
}

unsigned int ReminderType::GetModule() const {
	return module;
}

void ReminderType::SetReference(const unsigned int reference) {
	this->reference = reference;
}

unsigned int ReminderType::GetReference() const {
	return reference;
}

void ReminderType::SetDescription(const wxString &description) {
	this->description = description;
}

wxString ReminderType::GetDescription() const {
	return description;
}

void ReminderType::SetDateTime(const DSDateTime &dateTime) {
	this->dateTime = dateTime;
}

DSDateTime ReminderType::GetDateTime() const {
	return dateTime;
}

void ReminderType::SetNotes(const wxString &notes) {
	this->notes = notes;
}

wxString ReminderType::GetNotes() const {
	return notes;
}

wxString ReminderType::ToString() {
	return
		_N("         ID: ") + DSCasts::ToString(GetId()) + _N("\n") +
		_N("     Module: ") + DSCasts::ToString(GetModule()) + _N("\n") +
		_N("  Reference: ") + DSCasts::ToString(GetReference()) + _N("\n") +
		_N("Description: ") + GetDescription() + _N("\n") +
		_N("   DateTime: ") + GetDateTime().ToDatabaseDateTime() + _N("\n") +
		_N("      Notes: ") + GetNotes()
	;
}

wxString ReminderType::GetModuleString() {
	return GetModuleString(GetModule());
}

wxString ReminderType::GetModuleString(const unsigned int module) {
	wxString name;

	switch(module) {
		case CLIENTS:   name = _("Clients");   break;
		case PRODUCTS:  name = _("Products");  break;
		case ORDERS:    name = _("Orders");    break;
		case VOUCHERS:  name = _("Vouchers");  break;
		case PURCHASES: name = _("Purchases"); break;
		default:        name = _("n/a");       break;
	}

	return name;
}
