#include "BankConnectionController.h"
#include "BankConnectionEntity.h"

BankConnectionController::BankConnectionController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("bank_connections"), // Table
	_N("bankConnectionId"),
	_N("bank")
) {
}

bool BankConnectionController::Save(DSPostgreSQLQuery &query, unsigned int clientId, BankConnectionEntities &bankConnectionEntities) {
	bool ok = true;

	BankConnectionEntity *bankConnectionEntity = NULL;
	BankConnectionController bankConnectionController(rit);
	unsigned int count = bankConnectionEntities.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		bankConnectionEntity = bankConnectionEntities.Get(i);
		if(bankConnectionEntity) {
			bankConnectionEntity->SetValue(_N("clientId"), clientId);
			if(bankConnectionEntity->Get(_N("bankConnectionId"))->ToUnsignedInt() == 0)
				ok = bankConnectionController.Insert(bankConnectionEntity, true, NULL, &query);
			else
				ok = bankConnectionController.Update(bankConnectionEntity, wxEmptyString, true, NULL, &query);

			if(!ok)
				break;
		}
	}

	bankConnectionEntity = new BankConnectionEntity(rit);
	wxArrayString *removedBankConnections = bankConnectionEntities.GetRemoved();
	count = removedBankConnections->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		bankConnectionEntity->SetValue(_N("bankConnectionId"), DSCasts::ToUnsignedInt(removedBankConnections->Item(i)));
		ok = bankConnectionController.Delete(bankConnectionEntity, wxEmptyString, &query);
		if(!ok)
			break;
	}
	delete bankConnectionEntity;

	return ok;
}

unsigned int BankConnectionController::Load(const unsigned int clientId, BankConnectionEntities &bankConnectionEntities) {
	unsigned int count = 0;

	BankConnectionEntity *bankConnectionEntity = NULL;
	BankConnectionController bankConnectionController(rit);
	bankConnectionController.Get(_N("clientId"), DSCasts::ToString(clientId));
	while(bankConnectionController.Run()) {
		bankConnectionEntity = new BankConnectionEntity(rit);
		bankConnectionController.Get(bankConnectionEntity);
		bankConnectionEntities.Add(bankConnectionEntity);

		count++;
	}

	return count;
}
