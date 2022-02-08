#ifndef CLIENT_ENTTITY_H_
#define CLIENT_ENTTITY_H_

#include <DSTemplateContainer.h>
#include "InformationEntity.h"
#include "RuntimeInformationType.h"
#include "AddressEntities.h"
#include "ContactPersonEntities.h"
#include "LinkedFileEntities.h"
#include "BankConnectionEntities.h"

using namespace DynSoft;

class ClientEntity : public InformationEntity {

	public:
		ClientEntity(RuntimeInformationType *rit);
		AddressEntities *GetAddresses();
		ContactPersonEntities *GetContactPersons();
		LinkedFileEntities *GetLinkedFileEntities();
		BankConnectionEntities *GetBankConnectionEntities();
		wxString GetMainData();
		wxString GetName();
	
	protected:
		AddressEntities addresses;
		ContactPersonEntities contactPersons;
		LinkedFileEntities linkedFileEntities;
		BankConnectionEntities bankConnectionEntities;

};

#endif /* CLIENT_ENTTITY_H_ */
