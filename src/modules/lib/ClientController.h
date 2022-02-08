#ifndef CLIENT_CONTROLLER_H_
#define CLIENT_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "ClientEntity.h"
#include "HtmlPrintoutLibrary.h"
#include "AddressEntities.h"
#include "ReminderInterface.h"

using namespace DynSoft;

class ClientController : public Controller, public ReminderInterface {

	public:
		ClientController(
			RuntimeInformationType *rit
		);

		using Controller::Get;

		void Find(ClientEntity *clientEntity, const bool onlyActive = false);
		HtmlPrintoutLibrary *Print(ClientEntity *clientEntity);
		static ClientEntity *GetDirect(RuntimeInformationType *rit, const unsigned int clientId);
		bool Save(ClientEntity *clientEntity, wxArrayString *addressesRemoved, wxArrayString *contactPersonsRemoved);
		void Get(ClientEntity *clientEntity, const bool justGetMainEntity = false);
		AddressEntities *GetAddresses(const unsigned int clientId);
		bool ExistsOwnClientId(const wxString &ownClientId, const wxString &oldOwnClientId);
		bool IsActive(const unsigned int clientId);
		bool IsDistributor(const unsigned int clientId, bool *active = NULL);

		virtual wxString GetReminderDescription(const unsigned int id);
	
};

#endif /* CLIENT_CONTROLLER_H_ */
