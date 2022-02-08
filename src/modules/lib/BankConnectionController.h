#ifndef BANK_CONNECTION_CONTROLLER_H_
#define BANK_CONNECTION_CONTROLLER_H_

#include "Controller.h"
#include <DSPostgreSQLQuery.h>
#include "RuntimeInformationType.h"
#include "BankConnectionEntities.h"

using namespace DynSoft;

class BankConnectionController : public Controller {

	public:
		BankConnectionController(
			RuntimeInformationType *rit
		);

		bool Save(DSPostgreSQLQuery &query, unsigned int clientId, BankConnectionEntities &bankConnectionEntities);
		unsigned int Load(const unsigned int clientId, BankConnectionEntities &bankConnectionEntities);

};

#endif /* BANK_CONNECTION_CONTROLLER_H_ */
