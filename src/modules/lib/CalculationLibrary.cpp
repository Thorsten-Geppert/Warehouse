#include "CalculationLibrary.h"
#include "CommonPreferencesController.h"
#include "CommonPreferencesEntity.h"

DSCurrency CalculationLibrary::Reduction(DSCurrency price, DSCurrency reduction) {
	return DSCurrency(price / 100 * reduction).Round();
}

DSCurrency CalculationLibrary::GetAddTax(DSCurrency price, DSCurrency tax) {
	return DSCurrency(price / 100 * tax).Round();
}

DSCurrency CalculationLibrary::GetSubTax(DSCurrency price, DSCurrency tax) {
	return DSCurrency(price - (price / (tax + 100) * 100)).Round();
}

DSCurrency CalculationLibrary::AddTax(DSCurrency price, DSCurrency tax) {
	return DSCurrency(price + GetAddTax(price, tax)).Round();
}

DSCurrency CalculationLibrary::SubTax(DSCurrency price, DSCurrency tax) {
	return DSCurrency(price - GetSubTax(price, tax)).Round();
}
