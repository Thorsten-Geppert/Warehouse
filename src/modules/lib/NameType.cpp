#include "NameType.h"

NameType::NameType() {
}

NameType::NameType(
	const wxString &salutation,
	const wxString &title,
	const wxString &firstname,
	const wxString &name1,
	const wxString &name2,
	const wxString &name3,
	const wxString &name4
) {
	Set(salutation, title, firstname, name1, name2, name3, name4);
}

void NameType::Set(
	const wxString &salutation,
	const wxString &title,
	const wxString &firstname,
	const wxString &name1,
	const wxString &name2,
	const wxString &name3,
	const wxString &name4
) {
	SetSalutation(salutation);
	SetTitle(title);
	SetFirstname(firstname);
	SetName1(name1);
	SetName2(name2);
	SetName3(name3);
	SetName4(name4);
}

void NameType::SetSalutation(const wxString &salutation) {
	this->salutation = salutation;
}

void NameType::SetTitle(const wxString &title) {
	this->title = title;
}

void NameType::SetFirstname(const wxString &firstname) {
	this->firstname = firstname;
}

void NameType::SetName1(const wxString &name1) {
	this->name1 = name1;
}

void NameType::SetName2(const wxString &name2) {
	this->name2 = name2;
}

void NameType::SetName3(const wxString &name3) {
	this->name3 = name3;
}

void NameType::SetName4(const wxString &name4) {
	this->name4 = name4;
}

void NameType::Get(
	wxString *salutation,
	wxString *title,
	wxString *firstname,
	wxString *name1,
	wxString *name2,
	wxString *name3,
	wxString *name4
) {
	if(salutation)
		*salutation = GetSalutation();
	if(title)
		*title = GetTitle();
	if(firstname)
		*firstname = GetFirstname();
	if(name1)
		*name1 = GetName1();
	if(name2)
		*name2 = GetName2();
	if(name3)
		*name3 = GetName3();
	if(name4)
		*name4 = GetName4();

}

wxString NameType::GetSalutation() const {
	return salutation;
}

wxString NameType::GetTitle() const {
	return title;
}

wxString NameType::GetFirstname() const {
	return firstname;
}

wxString NameType::GetName1() const {
	return name1;
}

wxString NameType::GetName2() const {
	return name2;
}

wxString NameType::GetName3() const {
	return name3;
}

wxString NameType::GetName4() const {
	return name4;
}

wxString NameType::GetConcatedName(const wxString &separator) const {
	wxString tmp;

	if(!GetName1().IsEmpty())
		tmp += GetName1() + separator;
	if(!GetName2().IsEmpty())
		tmp += GetName2() + separator;
	if(!GetName3().IsEmpty())
		tmp += GetName3() + separator;
	if(!GetName4().IsEmpty())
		tmp += GetName4() + separator;
	
	const unsigned int length = tmp.Length();
	const unsigned int separatorLength = separator.Length();
	return length > separatorLength ? tmp.Mid(0, length - separatorLength) : tmp;
}

wxString NameType::GetConcatedFullName(const wxString &separator) const {
	wxString tmp(GetSalutation());
	
	if(!GetTitle().IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += _N(" ");
		tmp += GetTitle();
	}

	if(!GetFirstname().IsEmpty()) {
		if(!tmp.IsEmpty())
			tmp += _N(" ");
		tmp += GetFirstname();
	}
	const wxString names(GetConcatedName(separator));
	if(!tmp.IsEmpty() && !names.IsEmpty())
		tmp += _N(" ");
	tmp += names;

	return tmp;
}
