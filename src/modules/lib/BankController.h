#ifndef BANK_CONTROLLER_H_
#define BANK_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "BankEntity.h"
#include "ImportLibrary.h"

using namespace DynSoft;

class BankController : public Controller, public ImportLibrary {

	public:
		BankController(
			RuntimeInformationType *rit
		);

		void Find(BankEntity *bankEntity);
		static BankEntity *GetDirect(RuntimeInformationType *rit, const unsigned int id);
		virtual bool ImportRun(const wxString &line);
		bool DeleteAll();
		bool ExistsWithBankAndBankCode(const wxString &bank, const wxString &bankCode, const wxString &bic);
	
};

#endif /* BANK_CONTROLLER_H_ */
