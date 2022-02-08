#ifndef REMINDER_LIBRARY_H_
#define REMINDER_LIBRARY_H_

#include <DSType.h>
#include <DSDateTime.h>
#include <DSPostgreSQLQuery.h>
#include "RuntimeInformationType.h"
#include "ReminderType.h"
#include "RemindersType.h"

class ReminderLibrary : DSType {

	public:
		ReminderLibrary(RuntimeInformationType *rit);
		
		RemindersType *Get(const wxString &username = wxEmptyString, const unsigned int referenceId = 0, const unsigned int filterModule = 0);
		bool Insert(ReminderType &reminderType);
		bool Update(ReminderType &reminderType);
		bool Delete(const unsigned int id);

		wxString GetDescription(
			const unsigned int module,
			const unsigned int id
		);

		static void FillTimeComboBoxes(wxComboBox *hours, wxComboBox *minutes);

		DSDateTime GetServerTime();

	protected:
		RuntimeInformationType *rit;

		void Switch(
			const unsigned int module,
			const unsigned int reference,
			unsigned int *clientId,
			unsigned int *productId,
			unsigned int *orderId,
			unsigned int *voucherId,
			unsigned int *purchaseId
		);
		void UnSwitch(
			unsigned int *module,
			unsigned int *reference,
			const unsigned int clientId,
			const unsigned int productId,
			const unsigned int orderId,
			const unsigned int voucherId,
			const unsigned int purchaseId
		);

		void Bind(DSPostgreSQLQuery &query, ReminderType &reminderType);

};

#endif /* REMINDER_LIBRARY_H_ */
