#ifndef CONSTANTS_LIBRARY_H_
#define CONSTANTS_LIBRARY_H_

#include <DSLib.h>

class ConstantsLibrary {

	public:
		static const unsigned int ORDER_KIND_BID               = 0;
		static const unsigned int ORDER_KIND_ORDER             = 1;
		static const unsigned int ORDER_KIND_CREDIT            = 2;
		static const unsigned int ORDER_KIND_DELIVERY          = 3;

		static const unsigned int CLIENT_KIND                  = 0;
		static const unsigned int PRODUCT_KIND                 = 1;
		static const unsigned int ORDER_KIND                   = 2;

		static const unsigned int PAYMENT_CASH                 = 0; // Bar
		static const unsigned int PAYMENT_BILL                 = 1; // Per Rechnung
		static const unsigned int PAYMENT_PREPAYMENT           = 2; // Per Vorauskasse
		static const unsigned int PAYMENT_CASH_ON_DELIVERY     = 3; // Per Nachnahme
		static const unsigned int PAYMENT_DEBIT                = 4; // Per Lastschrift

		static const unsigned int FORM_OF_ORGANIZATION_NATURAL = 0;
		static const unsigned int FORM_OF_ORGANIZATION_LEGAL   = 1;

		static const unsigned int SEPA_STATE_ONCE              = 0;
		static const unsigned int SEPA_STATE_MULTIPLE          = 1;

		static const unsigned int SEPA_ACCOUNT_B2B             = 0;
		static const unsigned int SEPA_ACCOUNT_B2C             = 1;

		static wxString OrderKindToString(const unsigned kind);
		static wxArrayString OrderKindToArrayString();
		static wxString KindToString(const unsigned kind);
		static wxArrayString KindToArrayString();
		static wxString PaymentToString(const unsigned payment);
		static wxArrayString PaymentToArrayString();
		static wxString TaxRateToString(const unsigned taxRate);
		static wxArrayString TaxRateToArrayString();
		static wxString FormOfOrganizationToString(const unsigned form);
		static wxArrayString FormOfOrganizationToArrayString();
		static wxString TaxToString(const unsigned tax);
		static wxArrayString TaxToArrayString();
		static wxString SepaStateToString(const unsigned int state);
		static wxArrayString SepaStateToArrayString();
		static wxString SepaAccountToString(const unsigned int account);
		static wxArrayString SepaAccountToArrayString();
		static wxString TaxKindToString(const unsigned int taxKind);
		static wxArrayString TaxKindToArrayString();

};

#endif /* CONSTANTS_LIBRARY_H_ */
