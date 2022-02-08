#ifndef PURCHASE_CONTROLLER_H_
#define PURCHASE_CONTROLLER_H_

#include <DSListCtrl.h>
#include <DSCurrency.h>
#include "Controller.h"
#include "RuntimeInformationType.h"
#include "ReminderInterface.h"
#include "PurchaseEntity.h"

using namespace DynSoft;

class PurchaseController : public Controller, public ReminderInterface {

	public:
		PurchaseController(
			RuntimeInformationType *rit
		);

		using Controller::Get;

		virtual wxString GetReminderDescription(const unsigned int id);
		void Get(PurchaseEntity *purchaseEntity, const bool justGetMainEntity = false);
		bool Save(PurchaseEntity *purchaseEntity, wxArrayString *purchasePositionsRemoved);
		void Find(PurchaseEntity *purchaseEnity);
		void Change(PurchaseEntity *purchaseEntity);
		static PurchaseEntity *GetDirect(RuntimeInformationType *rit, const unsigned int id);
		bool SetDone(const unsigned int purchaseId, const bool done, const DSDateTime &doneTime);
		wxString GetDone(const unsigned int purchaseId);
		HtmlPrintoutLibrary *Print(PurchaseEntity *purchaseEntity);
		wxString CreateProductName(PurchasePositionEntity &purchasePositionEntity, const wxString &sep = _N(", "));
	
};

#endif /* PURCHASE_CONTROLLER_H_ */
