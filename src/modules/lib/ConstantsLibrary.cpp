#include "ConstantsLibrary.h"

wxString ConstantsLibrary::OrderKindToString(const unsigned kind) {
	return OrderKindToArrayString()[kind];
}

wxArrayString ConstantsLibrary::OrderKindToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("Bid"));
	tmp.Add(_("Order"));
	tmp.Add(_("Credit"));
	tmp.Add(_("Delivery"));
	return tmp;
}

wxString ConstantsLibrary::KindToString(const unsigned kind) {
	return KindToArrayString()[kind];
}

wxArrayString ConstantsLibrary::KindToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("Client"));
	tmp.Add(_("Product"));
	tmp.Add(_("Order"));
	return tmp;
}

wxString ConstantsLibrary::PaymentToString(const unsigned payment) {
	return PaymentToArrayString()[payment];
}

wxArrayString ConstantsLibrary::PaymentToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("Bill"));
	tmp.Add(_("Prepayment"));
	tmp.Add(_("Cash"));
	tmp.Add(_("Cash on delivery"));
	tmp.Add(_("Debit"));
	return tmp;
}

wxString ConstantsLibrary::TaxRateToString(const unsigned taxRate) {
	return TaxRateToArrayString()[taxRate];
}

wxArrayString ConstantsLibrary::TaxRateToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("Invoice pre tax"));
	tmp.Add(_("Invoice after tax"));
	tmp.Add(_("No tax"));
	return tmp;
}

wxString ConstantsLibrary::TaxToString(const unsigned taxRate) {
	return TaxToArrayString()[taxRate];
}

wxArrayString ConstantsLibrary::TaxToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("Pre tax"));
	tmp.Add(_("After tax"));
	tmp.Add(_("No tax"));
	return tmp;
}

wxString ConstantsLibrary::FormOfOrganizationToString(const unsigned form) {
	return FormOfOrganizationToArrayString()[form];
}

wxArrayString ConstantsLibrary::FormOfOrganizationToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("natural"));
	tmp.Add(_("legal"));
	return tmp;
}

wxString ConstantsLibrary::SepaStateToString(const unsigned int state) {
	return SepaStateToArrayString()[state];
}

wxArrayString ConstantsLibrary::SepaStateToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("once"));
	tmp.Add(_("multiple"));
	return tmp;
}

wxString ConstantsLibrary::SepaAccountToString(const unsigned int account) {
	return SepaAccountToArrayString()[account];
}

wxArrayString ConstantsLibrary::SepaAccountToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("B2B"));
	tmp.Add(_("B2C"));
	return tmp;
}

wxString ConstantsLibrary::TaxKindToString(const unsigned int taxKind) {
	return TaxKindToArrayString()[taxKind];
}

wxArrayString ConstantsLibrary::TaxKindToArrayString() {
	wxArrayString tmp;
	tmp.Add(_("full tax"));
	tmp.Add(_("reduced tax"));
	tmp.Add(_("without tax"));
	return tmp;
}
