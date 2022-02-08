#ifndef MONITION_CONTROLLER_H_
#define MONITION_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "MonitionEntity.h"
#include "HtmlPrintoutLibrary.h"

using namespace DynSoft;

class MonitionController : public Controller {

	public:
		MonitionController(
			RuntimeInformationType *rit
		);

		void GetUnpayedOrders(
			wxString orderId,
			wxString invoiceId,
			wxString clientId,
			const wxString &invoiceDateFrom,
			const wxString &invoiceDateTo,
			wxArrayString &monitionStates
		);
		MonitionEntity *GetUnpayedOrder();

		wxString Print(
			HtmlPrintoutLibrary *htmlPrintoutLibrary,
			MonitionEntity *monitionEntity,
			DSCurrency monitionPrice
		);

};

#endif /* MONITION_CONTROLLER_H_ */
