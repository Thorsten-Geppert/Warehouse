#include "CommonPreferencesController.h"

CommonPreferencesController::CommonPreferencesController(
	DSPostgreSQLDatabase *db
) : DSPostgreSQLEntityData(
	db,
	_N("common"),
	_N("id")
) {
}

void CommonPreferencesController::GetTax(RuntimeInformationType *rit, DSCurrency *taxFull, DSCurrency *taxReduced) {
	if(taxFull)
		*taxFull = 0.0;
	if(taxReduced)
		*taxReduced = 0.0;

	CommonPreferencesController commonPreferencesController(rit->GetDatabase());
	commonPreferencesController.Get();
	if(commonPreferencesController.Run()) {
		CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());
		commonPreferencesController.Get(&commonPreferencesEntity);
		commonPreferencesController.Reset();

		if(taxFull)
			*taxFull = commonPreferencesEntity.Get(_N("majorValueAddedTax"))->ToCurrency();
		if(taxReduced)
			*taxReduced = commonPreferencesEntity.Get(_N("minorValueAddedTax"))->ToCurrency();
	}
}

bool CommonPreferencesController::CheckTaxProfile(RuntimeInformationType *rit) {
	DSCurrency taxReduced, taxFull;
	GetTax(rit, &taxReduced, &taxFull);
	return taxReduced > 0 && taxFull > 0;
}

int CommonPreferencesController::GetMonitionStates(RuntimeInformationType *rit, const int defaultStates) {
	int states = defaultStates;

	CommonPreferencesController commonPreferencesController(rit->GetDatabase());
	commonPreferencesController.Get();
	if(commonPreferencesController.Run()) {
		CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());
		commonPreferencesController.Get(&commonPreferencesEntity);
		commonPreferencesController.Reset();

		states = commonPreferencesEntity.Get(_N("monitionStates"))->ToInt();
	}

	return states;
}

DSCurrency CommonPreferencesController::GetMonitionPrice(RuntimeInformationType *rit) {
	DSCurrency price;

	CommonPreferencesController commonPreferencesController(rit->GetDatabase());
	commonPreferencesController.Get();
	if(commonPreferencesController.Run()) {
		CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());
		commonPreferencesController.Get(&commonPreferencesEntity);
		commonPreferencesController.Reset();

		price = commonPreferencesEntity.Get(_N("monitionPrice"))->ToCurrency();
	}

	return price;
}
