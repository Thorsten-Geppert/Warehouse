#include "EnterprisePreferencesController.h"

EnterprisePreferencesController::EnterprisePreferencesController(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLEntityData(
	db,
	_N("enterprise"),
	_N("id")
) {
}

bool EnterprisePreferencesController::CheckEnterpriseProfile(RuntimeInformationType *rit) {
	EnterprisePreferencesController enterpricePreferencesController(rit->GetDatabase());
	enterpricePreferencesController.Get();
	if(enterpricePreferencesController.Run()) {
		EnterprisePreferencesEntity enterprisePreferencesEntity(rit->GetDatabase());
		enterpricePreferencesController.Get(&enterprisePreferencesEntity);
		enterpricePreferencesController.Reset();

		return enterprisePreferencesEntity.IsComplete();
	}

	return false;
}
