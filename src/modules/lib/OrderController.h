#ifndef ORDER_CONTROLLER_H_
#define ORDER_CONTROLLER_H_

#include <DSListCtrl.h>
#include <DSCurrency.h>
#include "Controller.h"
#include "RuntimeInformationType.h"
#include "OrderEntity.h"
#include "StatisticsEntity.h"
#include "ReferenceInterface.h"
#include "ReferenceType.h"
#include "HtmlPrintoutLibrary.h"
#include "ReminderInterface.h"

using namespace DynSoft;

class OrderController : public Controller, public ReferenceInterface, public ReminderInterface {

	public:
		OrderController(
			RuntimeInformationType *rit
		);

		using Controller::Get;

		void Get(OrderEntity *orderEntity, const bool justGetMainEntity = false);
		bool Save(OrderEntity *orderEntity, wxArrayString *orderPositionsRemoved);
		bool Invoice(OrderEntity *orderEntity, const DSDateTime &invoiceDate);
		void Find(OrderEntity *orderEntity);
		void Change(OrderEntity *orderEntity, const bool copy = false);
		void Statistics(
			const unsigned int productId,
			const unsigned int clientId,
			const wxDateTime &from,
			const wxDateTime &to
		);
		void StatisticsGet(StatisticsEntity *statisticsEntity);
		virtual unsigned int GetReferences(
			const unsigned int kind,
			const unsigned int primaryKey,
			ReferencesType *referencesType
		);
		bool IsInvoiced(const unsigned int orderId);
		HtmlPrintoutLibrary *PrintStatistic(
			const unsigned int productId,
			const unsigned int clientId,
			const wxDateTime &from,
			const wxDateTime &to,
			DSListCtrl *listCtrl
		);
		HtmlPrintoutLibrary *Print(OrderEntity *orderEntity, const bool delivery, const bool asCopy);
		wxString PrintPositions(
			HtmlPrintoutLibrary *htmlPrintoutLibrary,
			OrderPositionEntities *orderPositionEntities,
			const bool delivery,
			DSCurrency &sumBeforeTax,
			DSCurrency &sumFullTax,
			DSCurrency &sumReducedTax,
			DSCurrency &sumAfterTax,
			DSCurrency &fullWeight,
			DSCurrency &fullAmount
		);

		wxString PrintLinkedOrders(
			HtmlPrintoutLibrary *htmlPrintoutLibrary,
			OrderEntities *linkedOrderEntities,
			const bool delivery,
			DSCurrency &sumBeforeTax,
			DSCurrency &sumFullTax,
			DSCurrency &sumReducedTax,
			DSCurrency &sumAfterTax,
			DSCurrency &fullWeight,
			DSCurrency &fullAmount
		);

		bool IncrementPrints(const unsigned int orderId);
		unsigned int GetPrints(const unsigned int orderId);
		static OrderEntity *GetDirect(RuntimeInformationType *rit, const unsigned int id);
		bool SetPayed(const unsigned int orderId, const bool payed, const DSDateTime &payedTime);
		wxString GetPayed(const unsigned int orderId);

		wxString CreateProductName(OrderPositionEntity &orderPositionEntity, const wxString &sep = _N(", "));

		bool IsLinked(const unsigned int orderId, OrderEntities *linkedOrderEntities = NULL);

		bool SetMonitionState(const unsigned int orderId, bool increment, int *newMonitionState = NULL);

		virtual wxString GetReminderDescription(const unsigned int id);
	
	private:
		wxString GetPrintFieldByKind(const unsigned int kind);
	
};

#endif /* ORDER_CONTROLLER_H_ */
