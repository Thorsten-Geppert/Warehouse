#include "ClientController.h"
#include <DSPostgreSQLQuery.h>
#include <DSCasts.h>
#include <DSTemplate.h>
#include "PhoneType.h"
#include "NameType.h"
#include "ConstantsLibrary.h"
#include "AddressEntity.h"
#include "ContactPersonEntity.h"
#include "AddressEntities.h"
#include "ContactPersonEntities.h"
#include "AddressController.h"
#include "ContactPersonController.h"
#include "LinkedFileController.h"
#include "BankConnectionController.h"
#include "LinkedFileController.h"

ClientController::ClientController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("clients"), // Table
	_N("clientId"),
	_N("clientId")
) {
	LoadResource(rit->GRP(_N("sql"), _N("Clients.res")));
}

void ClientController::Find(ClientEntity *clientEntity, const bool onlyActive) {
	const wxString clientId(WildCard(clientEntity->Get(_N("clientId"))->ToString()));
	const wxString salutation(WildCard(clientEntity->Get(_N("salutation"))->ToString()));
	const wxString title(WildCard(clientEntity->Get(_N("title"))->ToString()));
	const wxString firstname(WildCard(clientEntity->Get(_N("firstname"))->ToString()));
	const wxString address(WildCard(clientEntity->Get(_N("address"))->ToString()));
	const wxString zipcode(WildCard(clientEntity->Get(_N("zipcode"))->ToString()));
	const wxString city(WildCard(clientEntity->Get(_N("city"))->ToString()));
	const wxString land(WildCard(clientEntity->Get(_N("land"))->ToString()));
	const wxString email1(WildCard(clientEntity->Get(_N("email1"))->ToString()));
	const wxString email2(WildCard(clientEntity->Get(_N("email2"))->ToString()));
	const wxString name1(WildCard(clientEntity->Get(_N("name1"))->ToString()));
	const wxString name2(WildCard(clientEntity->Get(_N("name2"))->ToString()));
	const wxString name3(WildCard(clientEntity->Get(_N("name3"))->ToString()));
	const wxString name4(WildCard(clientEntity->Get(_N("name4"))->ToString()));
	
	DSQueryParser parser;
	parser.Prepare(GetResourceByName(onlyActive ? _N("Clients.FindActive") : _N("Clients.Find")));
	parser.Bind(_N("table"), GetTable(), false);
	parser.Bind(_N("clientId"), clientId);
	parser.Bind(_N("ownClientId"), clientId);
	parser.Bind(_N("salutation"), salutation);
	parser.Bind(_N("title"), title);
	parser.Bind(_N("firstname"), firstname);
	parser.Bind(_N("address"), address);
	parser.Bind(_N("zipcode"), zipcode);
	parser.Bind(_N("city"), city);
	parser.Bind(_N("land"), land);
	parser.Bind(_N("email1"), email1);
	parser.Bind(_N("email2"), email2);
	parser.Bind(_N("name1"), name1);
	parser.Bind(_N("name2"), name2);
	parser.Bind(_N("name3"), name3);
	parser.Bind(_N("name4"), name4);
	Create(parser.GetParsedQuery());
}

HtmlPrintoutLibrary *ClientController::Print(ClientEntity *clientEntity) {
	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Client"), rit);

	DSTemplate clientTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("Client"));
	clientTemplate.SetEntity(clientEntity);

	clientTemplate.SetVariable(_N("active"), clientEntity->Get(_N("active"))->ToBool() ? _("yes") : _("no"));

	clientTemplate.SetVariable(
		_N("telephone1"),
		PhoneType(
			clientEntity->Get(_N("telephone1Country"))->ToString(),
			clientEntity->Get(_N("telephone1Prefix"))->ToString(),
			clientEntity->Get(_N("telephone1Number"))->ToString()
		).ToString(),
		true
	);
	clientTemplate.SetVariable(
		_N("telephone2"),
		PhoneType(
			clientEntity->Get(_N("telephone2Country"))->ToString(),
			clientEntity->Get(_N("telephone2Prefix"))->ToString(),
			clientEntity->Get(_N("telephone2Number"))->ToString()
		).ToString(),
		true
	);
	clientTemplate.SetVariable(
		_N("telephone3"),
		PhoneType(
			clientEntity->Get(_N("telephone3Country"))->ToString(),
			clientEntity->Get(_N("telephone3Prefix"))->ToString(),
			clientEntity->Get(_N("telephone3Number"))->ToString()
		).ToString(),
		true
	);
	clientTemplate.SetVariable(
		_N("mobile1"),
		PhoneType(
			clientEntity->Get(_N("mobile1Country"))->ToString(),
			clientEntity->Get(_N("mobile1Prefix"))->ToString(),
			clientEntity->Get(_N("mobile1Number"))->ToString()
		).ToString(),
		true
	);
	clientTemplate.SetVariable(
		_N("mobile2"),
		PhoneType(
			clientEntity->Get(_N("mobile2Country"))->ToString(),
			clientEntity->Get(_N("mobile2Prefix"))->ToString(),
			clientEntity->Get(_N("mobile2Number"))->ToString()
		).ToString(),
		true
	);
	clientTemplate.SetVariable(
		_N("fax1"),
		PhoneType(
			clientEntity->Get(_N("fax1Country"))->ToString(),
			clientEntity->Get(_N("fax1Prefix"))->ToString(),
			clientEntity->Get(_N("fax1Number"))->ToString()
		).ToString(),
		true
	);
	clientTemplate.SetVariable(
		_N("fax2"),
		PhoneType(
			clientEntity->Get(_N("fax2Country"))->ToString(),
			clientEntity->Get(_N("fax2Prefix"))->ToString(),
			clientEntity->Get(_N("fax2Number"))->ToString()
		).ToString(),
		true
	);
	clientTemplate.SetVariable(
		_N("fax3"),
		PhoneType(
			clientEntity->Get(_N("fax3Country"))->ToString(),
			clientEntity->Get(_N("fax3Prefix"))->ToString(),
			clientEntity->Get(_N("fax3Number"))->ToString()
		).ToString(),
		true
	);
	clientTemplate.SetVariable(_N("informations"), nl2br(HtmlSpecialChars(clientEntity->Get(_N("informations"))->ToString())));
	clientTemplate.SetVariable(_N("liableToTax"), clientEntity->Get(_N("liableToTax"))->ToBool() ? _("yes") : _("no"), true);
	clientTemplate.SetVariable(_N("formOfOrganization"), ConstantsLibrary::FormOfOrganizationToString(clientEntity->Get(_N("liableToTax"))->ToUnsignedInt()), true);

	// Addresses
	DSTemplate clientAddressTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("ClientAddress"));
	unsigned int count = clientEntity->GetAddresses()->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		clientAddressTemplate.SetEntity(clientEntity->GetAddresses()->Get(i));
		clientAddressTemplate.Parse();
	}
	clientTemplate.SetDynamicVariable(_N("addresses"), count == 0 ? wxString(_("No addresses exist")) : clientAddressTemplate.Get());

	// Contact persons
	ContactPersonEntity *contactPersonEntity = NULL;
	DSTemplate clientContactPersonTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("ClientContactPerson"));
	count = clientEntity->GetContactPersons()->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		contactPersonEntity = clientEntity->GetContactPersons()->Get(i);
		clientContactPersonTemplate.SetEntity(contactPersonEntity);

		clientContactPersonTemplate.SetVariable(
			_N("telephone"),
			PhoneType(
				contactPersonEntity->Get(_N("telephoneCountry"))->ToString(),
				contactPersonEntity->Get(_N("telephonePrefix"))->ToString(),
				contactPersonEntity->Get(_N("telephoneNumber"))->ToString()
			).ToString(),
			true
		);
		clientContactPersonTemplate.SetVariable(
			_N("mobile"),
			PhoneType(
				contactPersonEntity->Get(_N("mobileCountry"))->ToString(),
				contactPersonEntity->Get(_N("mobilePrefix"))->ToString(),
				contactPersonEntity->Get(_N("mobileNumber"))->ToString()
			).ToString(),
			true
		);
		clientContactPersonTemplate.SetVariable(
			_N("fax"),
			PhoneType(
				contactPersonEntity->Get(_N("faxCountry"))->ToString(),
				contactPersonEntity->Get(_N("faxPrefix"))->ToString(),
				contactPersonEntity->Get(_N("faxNumber"))->ToString()
			).ToString(),
			true
		);

		clientContactPersonTemplate.Parse();
	}
	clientTemplate.SetDynamicVariable(_N("contactPersons"), count == 0 ? wxString(_("No contact persons exist")) : clientContactPersonTemplate.Get());

	BankConnectionEntity *bankConnectionEntity = NULL;
	DSTemplate bankConnectionTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("BankConnection"));
	DSTemplate bankConnectionRowTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("BankConnectionRow"));
	count = clientEntity->GetBankConnectionEntities()->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		bankConnectionEntity = clientEntity->GetBankConnectionEntities()->Get(i);
		if(bankConnectionEntity) {
			bankConnectionRowTemplate.SetVariable(_N("description"), bankConnectionEntity->Get(_N("description"))->ToString(), true);
			bankConnectionRowTemplate.SetVariable(_N("bank"), bankConnectionEntity->Get(_N("bank"))->ToString(), true);
			bankConnectionRowTemplate.SetVariable(_N("bankCode"), bankConnectionEntity->Get(_N("bankCode"))->ToString(), true);
			bankConnectionRowTemplate.SetVariable(_N("accountNumber"), bankConnectionEntity->Get(_N("accountNumber"))->ToString(), true);
			bankConnectionRowTemplate.SetVariable(_N("bankOwner"), bankConnectionEntity->Get(_N("bankOwner"))->ToString(), true);
			bankConnectionRowTemplate.SetVariable(_N("bic"), bankConnectionEntity->Get(_N("bic"))->ToString(), true);
			bankConnectionRowTemplate.SetVariable(_N("iban"), bankConnectionEntity->Get(_N("iban"))->ToString(), true);
			bankConnectionRowTemplate.Parse();
		}
	}

	bankConnectionTemplate.SetDynamicVariable(_N("rows"), bankConnectionRowTemplate.Get());
	bankConnectionTemplate.Parse();
	clientTemplate.SetDynamicVariable(_N("bankConnections"), count == 0 ? wxString(_("No bank connections exist")) : bankConnectionTemplate.Get());

	// Linked files
	LinkedFileController linkedFileController(rit, _N("linked_files_clients"));
	clientTemplate.SetDynamicVariable(_N("linkedFiles"), clientEntity->GetLinkedFileEntities()->GetCount() == 0 ? wxString(_("No linked files exist")) : linkedFileController.Print(*htmlPrintoutLibrary, *clientEntity->GetLinkedFileEntities()));

	// More data
	clientTemplate.SetDynamicVariable(_N("moreData"), clientEntity->GetKeysValuesType()->GetCount() == 0 ? wxString(_("No more data exist")) : PrintKeysValues(*htmlPrintoutLibrary, *clientEntity->GetKeysValuesType()));

	clientTemplate.Parse();
	htmlPrintoutLibrary->SetWithContainer(clientTemplate.Get());

	// Header
	DSTemplate clientHeaderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("ClientHeader"));
	clientHeaderTemplate.SetVariable(
		_N("name"),
		NameType(
			clientEntity->Get(_N("salutation"))->ToString(),
			clientEntity->Get(_N("title"))->ToString(),
			clientEntity->Get(_N("firstname"))->ToString(),
			clientEntity->Get(_N("name1"))->ToString(),
			clientEntity->Get(_N("name2"))->ToString(),
			clientEntity->Get(_N("name3"))->ToString(),
			clientEntity->Get(_N("name4"))->ToString()
		).GetConcatedFullName(_N(" ")),
		true
	);
	clientHeaderTemplate.Parse();
	htmlPrintoutLibrary->SetHeader(clientHeaderTemplate.Get());

	// Footer
	DSTemplate clientFooterTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("ClientFooter"));
	clientFooterTemplate.Parse();
	htmlPrintoutLibrary->SetFooter(clientFooterTemplate.Get());

	return htmlPrintoutLibrary;
}

ClientEntity *ClientController::GetDirect(RuntimeInformationType *rit, const unsigned int clientId) {
	ClientEntity *clientEntity = NULL;
	ClientController clientController(rit);
	clientController.Get(DSCasts::ToString(clientId));
	if(clientController.Run()) {
		clientEntity = new ClientEntity(rit);
		clientController.Get(clientEntity);
		clientController.Reset();

		const wxString clientIdString(DSCasts::ToString(clientId));

		AddressEntities *addressEntities = clientEntity->GetAddresses();
		AddressEntity *addressEntity = NULL;
		AddressController addressController(rit);
		addressController.Get(_N("clientId"), clientIdString);
		while(addressController.Run()) {
			addressEntity = new AddressEntity(rit);
			addressController.Get(addressEntity);
			addressEntities->Add(addressEntity);
		}

		ContactPersonEntities *contactPersonEntities = clientEntity->GetContactPersons();
		ContactPersonEntity *contactPersonEntity = NULL;
		ContactPersonController contactPersonController(rit);
		contactPersonController.Get(_N("clientId"), clientIdString);
		while(contactPersonController.Run()) {
			contactPersonEntity = new ContactPersonEntity(rit);
			contactPersonController.Get(contactPersonEntity);
			contactPersonEntities->Add(contactPersonEntity);
		}

		LinkedFileController linkedFileController(rit, _N("linked_files_clients"));
		linkedFileController.Load(
			clientId,
			*clientEntity->GetLinkedFileEntities()
		);

		clientController.LoadKeysValues(
			clientId,
			_N("keys_values_clients"),
			*clientEntity->GetKeysValuesType()
		);

		BankConnectionController bankConnectionController(rit);
		bankConnectionController.Load(
			clientId,
			*clientEntity->GetBankConnectionEntities()
		);
	}
	return clientEntity;
}

bool ClientController::Save(ClientEntity *clientEntity, wxArrayString *addressesRemoved, wxArrayString *contactPersonsRemoved) {
	DSPostgreSQLQuery query(ndb);
	query.Begin();

	ClientController clientController(rit);
	bool ok = false;
	if(clientEntity->Get(_N("clientId"))->ToUnsignedInt() == 0) {
		ok = clientController.Insert(clientEntity, true, clientEntity->GetInsertExcludes(), &query);
	} else {
		clientEntity->ForceChanged();
		ok = clientController.Update(clientEntity, wxEmptyString, true, clientEntity->GetUpdateExcludes(), &query);
	}

	const unsigned int clientId = clientEntity->Get(_N("clientId"))->ToUnsignedInt();
	unsigned int count          = 0;
	
	if(ok) {
		AddressController addressController(rit);
		AddressEntity *addressEntity     = NULL;
		AddressEntities *addressEntities = clientEntity->GetAddresses();
		count = addressEntities->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			addressEntity = addressEntities->Get(i);
			if(!addressEntity)
				continue;

			addressEntity->SetValue(_N("clientId"), clientId);

			if(addressEntity->Get(_N("addressId"))->ToUnsignedInt() == 0)
				ok = addressController.Insert(addressEntity, true, NULL, &query);
			else
				ok = addressController.Update(addressEntity, wxEmptyString, true, NULL, &query);
			
			if(!ok)
				break;
		}

		if(ok && addressesRemoved) {
			AddressEntity addressEntity(rit);
			count = addressesRemoved->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				addressEntity.SetValue(_N("addressId"), DSCasts::ToUnsignedInt(addressesRemoved->Item(i)));
				ok = addressController.Delete(&addressEntity, wxEmptyString, &query);
				if(!ok)
					break;
			}
		}
	}

	if(ok) {
		ContactPersonController contactPersonController(rit);
		ContactPersonEntity *contactPersonEntity     = NULL;
		ContactPersonEntities *contactPersonEntities = clientEntity->GetContactPersons();
		count = contactPersonEntities->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			contactPersonEntity = contactPersonEntities->Get(i);
			if(!contactPersonEntity)
				continue;

			contactPersonEntity->SetValue(_N("clientId"), clientId);

			if(contactPersonEntity->Get(_N("contactPersonId"))->ToUnsignedInt() == 0)
				ok = contactPersonController.Insert(contactPersonEntity, true, NULL, &query);
			else
				ok = contactPersonController.Update(contactPersonEntity, wxEmptyString, true, NULL, &query);
			
			if(!ok)
				break;
		}

		if(ok && contactPersonsRemoved) {
			AddressEntity contactPersonEntity(rit);
			count = contactPersonsRemoved->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				contactPersonEntity.SetValue(_N("contactPersonId"), DSCasts::ToUnsignedInt(contactPersonsRemoved->Item(i)));
				ok = contactPersonController.Delete(&contactPersonEntity, wxEmptyString, &query);
				if(!ok)
					break;
			}
		}
	}

	if(ok) {
		LinkedFileController linkedFileController(rit, _N("linked_files_clients"));
		ok = linkedFileController.Save(
			query,
			clientEntity->Get(_N("clientId"))->ToUnsignedInt(),
			*clientEntity->GetLinkedFileEntities()
		);
	}

	if(ok) {
		ok = SaveKeysValues(
			query,
			clientEntity->Get(_N("clientId"))->ToUnsignedInt(),
			_N("keys_values_clients"),
			*clientEntity->GetKeysValuesType()
		);
	}
	
	if(ok) {
		BankConnectionController bankConnectionController(rit);
		ok = bankConnectionController.Save(
			query,
			clientEntity->Get(_N("clientId"))->ToUnsignedInt(),
			*clientEntity->GetBankConnectionEntities()
		);
	}

	if(!ok || !query.Commit()) {
		query.Rollback();
		return false;
	} else {
		clientEntity->GetLinkedFileEntities()->CleanRemoved();
	}

	return true;
}

void ClientController::Get(ClientEntity *clientEntity, const bool justGetMainEntity) {
	Controller::Get(clientEntity);

	if(!justGetMainEntity) {
		const wxString clientId(clientEntity->Get(_N("clientId"))->ToString());
		
		AddressEntities *addressEntities = clientEntity->GetAddresses();
		AddressEntity *addressEntity = NULL;
		AddressController addressController(rit);
		addressController.Get(_N("clientId"), clientId);
		while(addressController.Run()) {
			addressEntity = new AddressEntity(rit);
			addressController.Get(addressEntity);
			addressEntities->Add(addressEntity);
		}

		ContactPersonEntities *contactPersonEntities = clientEntity->GetContactPersons();
		ContactPersonEntity *contactPersonEntity = NULL;
		ContactPersonController contactPersonController(rit);
		contactPersonController.Get(_N("clientId"), clientId);
		while(contactPersonController.Run()) {
			contactPersonEntity = new ContactPersonEntity(rit);
			contactPersonController.Get(contactPersonEntity);
			contactPersonEntities->Add(contactPersonEntity);
		}

		LinkedFileController linkedFileController(rit, _N("linked_files_clients"));
		linkedFileController.Load(
			clientEntity->Get(_N("clientId"))->ToUnsignedInt(),
			*clientEntity->GetLinkedFileEntities()
		);

		LoadKeysValues(
			clientEntity->Get(_N("clientId"))->ToUnsignedInt(),
			_N("keys_values_clients"),
			*clientEntity->GetKeysValuesType()
		);

		BankConnectionController bankConnectionController(rit);
		bankConnectionController.Load(
			clientEntity->Get(_N("clientId"))->ToUnsignedInt(),
			*clientEntity->GetBankConnectionEntities()
		);
	}
}

AddressEntities *ClientController::GetAddresses(const unsigned int clientId) {
	ClientController clientController(rit);
	clientController.Get(DSCasts::ToString(clientId));
	if(clientController.Run()) {
		ClientEntity clientEntity(rit);
		clientController.Get(&clientEntity);
		clientController.Reset();

		AddressEntities *addressEntities = new AddressEntities;

		AddressEntity *addressEntity = new AddressEntity(rit, &clientEntity);
		addressEntities->Add(addressEntity);

		AddressController addressController(rit);
		addressController.Get(_N("clientId"), DSCasts::ToString(clientId));
		while(addressController.Run()) {
			addressEntity = new AddressEntity(rit);
			addressController.Get(addressEntity);
			addressEntities->Add(addressEntity);
		}

		return addressEntities;
	}

	return NULL;
}

wxString ClientController::GetReminderDescription(const unsigned int id) {
	ClientController clientController(rit);
	clientController.Get(DSCasts::ToString(id));
	if(clientController.Run()) {
		ClientEntity clientEntity(rit);
		clientController.Get(&clientEntity);
		clientController.Reset();

		return clientEntity.GetName();
	}
	return wxEmptyString;
}

bool ClientController::ExistsOwnClientId(const wxString &ownClientId, const wxString &oldOwnClientId) {
	if(ownClientId == oldOwnClientId)
		return false;
	
	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Clients.ExistsOwnClientId")));
	query.Bind(_N("table"), GetTable(), false);
	query.Bind(_N("ownClientId"), ownClientId);
	query.Exec();
	query.NextRecord();
	const bool exists = query.GetField(_N("counter")).ToUnsignedInt() > 0;
	query.FreeResult();
	return exists;
}

bool ClientController::IsActive(const unsigned int clientId) {
	ClientEntity *clientEntity = GetDirect(rit, clientId);
	const bool ok = clientEntity && clientEntity->Get(_N("active"))->ToBool();
	delete clientEntity;
	return ok;
}

bool ClientController::IsDistributor(const unsigned int clientId, bool *active) {
	if(clientId == 0)
		return false;

	ClientEntity *clientEntity = GetDirect(rit, clientId);
	if(!clientEntity)
		return false;

	const wxString kind(clientEntity->Get(_N("kind"))->ToString());
	const bool ok = clientEntity && (kind == _N("Lieferant") || kind == _N("Kunde/Lieferant"));
	if(active)
		*active = clientEntity->Get(_N("active"))->ToBool();
	delete clientEntity;
	return ok;
}
