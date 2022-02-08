#include <DSDebug.h>
#include "ReminderLibrary.h"
#include "ReminderInterface.h"
#include "ClientController.h"
#include "ProductController.h"
#include "OrderController.h"
#include "VoucherController.h"
#include "PurchaseController.h"

ReminderLibrary::ReminderLibrary(RuntimeInformationType *rit) : DSType() {
	this->rit = rit;

	LoadResource(rit->GRP(_N("sql"), _N("Reminders.res")));
}

RemindersType *ReminderLibrary::Get(const wxString &username, const unsigned int referenceId, const unsigned int filterModule) {
	unsigned int module    = 0;
	unsigned int reference = 0;

	RemindersType *remindersType = new RemindersType;

	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Prepare(
		GetResourceByName(
			username.IsEmpty()
			?
			_N("Reminders.GetAll")
			:
			referenceId == 0
			?
			_N("Reminders.GetByUsername")
			:
			_N("Reminders.GetByModuleAndUsername")
		)
	);
	if(!username.IsEmpty())
		query.Bind(_N("username"), username);
	if(referenceId > 0) {
		wxString field; // TODO Get Primary Key by Entity
		switch(static_cast<int>(filterModule)) {
			case ReminderType::CLIENTS:
				field = _N("clientId");
				break;
			case ReminderType::PRODUCTS:
				field = _N("productId");
				break;
			case ReminderType::ORDERS:
				field = _N("orderId");
				break;
			case ReminderType::VOUCHERS:
				field = _N("voucherId");
				break;
			case ReminderType::PURCHASES:
				field = _N("purchaseId");
				break;
		}
		query.Bind(_N("field"), field, false);
		query.Bind(_N("id"), referenceId);
	}
	query.Exec();
	while(query.NextRecord()) {
		UnSwitch(
			&module,
			&reference,
			query.GetField(_N("clientId")).ToUnsignedInt(),
			query.GetField(_N("productId")).ToUnsignedInt(),
			query.GetField(_N("orderId")).ToUnsignedInt(),
			query.GetField(_N("voucherId")).ToUnsignedInt(),
			query.GetField(_N("purchaseId")).ToUnsignedInt()
		);

		remindersType->Add(new ReminderType(
			query.GetField(_N("reminderId")).ToUnsignedInt(),
			query.GetField(_N("username")).ToString(),
			module,
			reference,
			GetDescription(module, reference),
			query.GetDateTime(query.GetField(_N("dateTime")).ToString()),
			query.GetField(_N("notes")).ToString()
		));
	}
	query.FreeResult();

	return remindersType;
}

bool ReminderLibrary::Insert(ReminderType &reminderType) {
	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Begin();

	query.Prepare(GetResourceByName(_N("Reminders.Insert")));
	Bind(query, reminderType);
	if(query.Exec(false)) {
		query.Query(GetResourceByName(_N("Reminders.Sequence")));
		if(query.NextRecord())
			reminderType.SetId(query.GetField(_N("referenceId")).ToUnsignedInt());
		query.FreeResult();
	}

	if(!query.Commit()) {
		query.Rollback();
		return false;
	}

	return true;
}

bool ReminderLibrary::Update(ReminderType &reminderType) {
	DSPostgreSQLQuery query(rit->GetDatabase());

	query.Prepare(GetResourceByName(_N("Reminders.Update")));
	Bind(query, reminderType);
	query.Bind(_N("reminderId"), reminderType.GetId());

	return query.Exec(false);
}

bool ReminderLibrary::Delete(const unsigned int id) {
	DSPostgreSQLQuery query(rit->GetDatabase());

	query.Prepare(GetResourceByName(_N("Reminders.Delete")));
	query.Bind(_N("reminderId"), id);

	return query.Exec(false);
}

void ReminderLibrary::Switch(
	const unsigned int module,
	const unsigned int reference,
	unsigned int *clientId,
	unsigned int *productId,
	unsigned int *orderId,
	unsigned int *voucherId,
	unsigned int *purchaseId
) {
	if(clientId)   *clientId   = 0;
	if(productId)  *productId  = 0;
	if(orderId)    *orderId    = 0;
	if(voucherId)  *voucherId  = 0;
	if(purchaseId) *purchaseId = 0;

	switch(module) {
		case ReminderType::CLIENTS:
			if(clientId) *clientId = reference;
			break;
		case ReminderType::PRODUCTS:
			if(productId) *productId = reference;
			break;
		case ReminderType::ORDERS:
			if(orderId) *orderId = reference;
			break;
		case ReminderType::VOUCHERS:
			if(voucherId) *voucherId = reference;
			break;
		case ReminderType::PURCHASES:
			if(purchaseId) *purchaseId = reference;
			break;
	}
}

void ReminderLibrary::UnSwitch(
	unsigned int *module,
	unsigned int *reference,
	const unsigned int clientId,
	const unsigned int productId,
	const unsigned int orderId,
	const unsigned int voucherId,
	const unsigned int purchaseId
) {
	if(!module && !reference)
		return;
	
	unsigned int tmpModule    = 0;
	unsigned int tmpReference = 0;

	if(clientId > 0) {
		tmpModule    = ReminderType::CLIENTS;
		tmpReference = clientId;
	} else if(productId > 0) {
		tmpModule    = ReminderType::PRODUCTS;
		tmpReference = productId;
	} else if(orderId > 0) {
		tmpModule    = ReminderType::ORDERS;
		tmpReference = orderId;
	} else if(voucherId > 0) {
		tmpModule    = ReminderType::VOUCHERS;
		tmpReference = voucherId;
	} else if(purchaseId > 0) {
		tmpModule    = ReminderType::PURCHASES;
		tmpReference = purchaseId;
	}

	if(module)
		*module = tmpModule;
	if(reference)
		*reference = tmpReference;
}

void ReminderLibrary::Bind(DSPostgreSQLQuery &query, ReminderType &reminderType) {
	unsigned int clientId   = 0;
	unsigned int productId  = 0;
	unsigned int orderId    = 0;
	unsigned int voucherId  = 0;
	unsigned int purchaseId = 0;

	Switch(
		reminderType.GetModule(),
		reminderType.GetReference(),
		&clientId,
		&productId,
		&orderId,
		&voucherId,
		&purchaseId
	);

	if(clientId > 0)
		query.Bind(_N("clientId"), clientId);
	else
		query.Bind(_N("clientId"), _N("NULL"), false);
	
	if(productId > 0)
		query.Bind(_N("productId"), productId);
	else
		query.Bind(_N("productId"), _N("NULL"), false);
	
	if(orderId > 0)
		query.Bind(_N("orderId"), orderId);
	else
		query.Bind(_N("orderId"), _N("NULL"), false);
	
	if(voucherId > 0)
		query.Bind(_N("voucherId"), voucherId);
	else
		query.Bind(_N("voucherId"), _N("NULL"), false);

	if(purchaseId > 0)
		query.Bind(_N("purchaseId"), purchaseId);
	else
		query.Bind(_N("purchaseId"), _N("NULL"), false);

	query.Bind(_N("username"), reminderType.GetUsername());
	query.Bind(_N("dateTime"), reminderType.GetDateTime().ToDatabaseDateTime());
	query.Bind(_N("notes"), reminderType.GetNotes());
}

wxString ReminderLibrary::GetDescription(
	const unsigned int module,
	const unsigned int id
) {
	wxString description;
	
	switch(module) {
		case ReminderType::CLIENTS:
		{
			ClientController clientController(rit);
			description = clientController.GetReminderDescription(id);
			break;
		}
		case ReminderType::PRODUCTS:
		{
			ProductController productController(rit);
			description = productController.GetReminderDescription(id);
			break;
		}
		case ReminderType::ORDERS:
		{
			OrderController orderController(rit);
			description = orderController.GetReminderDescription(id);
			break;
		}
		case ReminderType::VOUCHERS:
		{
			VoucherController voucherController(rit);
			description = voucherController.GetReminderDescription(id);
			break;
		}
		case ReminderType::PURCHASES:
		{
			PurchaseController purchaseController(rit);
			description = purchaseController.GetReminderDescription(id);
			break;
		}
	}

	return description;
}

void ReminderLibrary::FillTimeComboBoxes(wxComboBox *hours, wxComboBox *minutes) {
	if(hours) {
		hours->Clear();
		for(unsigned int i = 0; i < 24; i++)
			hours->Append(DSCasts::ToString(i));
	}

	if(minutes) {
		minutes->Clear();
		for(unsigned int i = 0; i < 60; i++)
			minutes->Append(DSCasts::ToString(i));
	}
}

DSDateTime ReminderLibrary::GetServerTime() {
	DSDateTime serverTime;
	
	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Query(GetResourceByName(_N("Reminders.ServerTime")));
	if(query.NextRecord())
		serverTime = DSPostgreSQLQuery::GetDateTime(query.GetField(_N("serverTime")).ToString());
	query.FreeResult();
	
	return serverTime;
}
