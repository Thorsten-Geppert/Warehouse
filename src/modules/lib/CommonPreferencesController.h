#ifndef COMMON_PREFERENCES_CONTROLLER_H_
#define COMMON_PREFERENCES_CONTROLLER_H_

#include <DSPostgreSQLDatabase.h>
#include <DSPostgreSQLEntityData.h>
#include "CommonPreferencesEntity.h"
#include "RuntimeInformationType.h"

using namespace DynSoft;

class CommonPreferencesController : public DSPostgreSQLEntityData {

	public:
		CommonPreferencesController(
			DSPostgreSQLDatabase *db
		);

		static void GetTax(RuntimeInformationType *rit, DSCurrency *taxReduced, DSCurrency *taxFull);
		static bool CheckTaxProfile(RuntimeInformationType *rit);
		static int GetMonitionStates(RuntimeInformationType *rit, const int defaultStates);
		static DSCurrency GetMonitionPrice(RuntimeInformationType *rit);

};

#endif /* COMMON_PREFERENCES_CONTROLLER_H_ */
