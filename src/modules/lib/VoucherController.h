#ifndef VOUCHER_CONTROLLER_H_
#define VOUCHER_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "VoucherEntity.h"
#include "HtmlPrintoutLibrary.h"
#include "StatisticsEntity.h"
#include "ReminderInterface.h"

using namespace DynSoft;

class VoucherController : public Controller, public ReminderInterface {

	public:
		VoucherController(
			RuntimeInformationType *rit
		);

		void Find(VoucherEntity *voucherEntity);
		HtmlPrintoutLibrary *Print(VoucherEntity *voucherEntity);
		static VoucherEntity *GetDirect(RuntimeInformationType *rit, const unsigned int id);
		void Change(VoucherEntity *voucherEntity);

		VoucherEntity *Load(const unsigned int voucherId);
		bool Save(VoucherEntity *voucherEntity);

		void Statistics(
			const wxDateTime &from,
			const wxDateTime &to
		);
		void StatisticsGet(StatisticsEntity *statisticsEntity);

		virtual wxString GetReminderDescription(const unsigned int id);
	
};

#endif /* VOUCHER_CONTROLLER_H_ */
