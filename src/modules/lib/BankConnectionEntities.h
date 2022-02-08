#ifndef BANK_CONNECTION_ENTITIES_H_
#define BANK_CONNECTION_ENTITIES_H_

#include <DSTemplateContainer.h>
#include "BankConnectionEntity.h"

using namespace DynSoft;

class BankConnectionEntities : public DSTemplateContainer<BankConnectionEntity *> {

	public:
		BankConnectionEntities();
		wxArrayString *GetRemoved();
	
	protected:
		wxArrayString removedBankConnections;

};

#endif /* BANK_CONNECTION_ENTITIES_H_ */
