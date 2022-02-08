#include "BankConnectionEntities.h"

BankConnectionEntities::BankConnectionEntities() : DSTemplateContainer<BankConnectionEntity *>() {
}

wxArrayString *BankConnectionEntities::GetRemoved() {
	return &removedBankConnections;
}
