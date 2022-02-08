#ifndef CALCULATION_LIBRARY_H_
#define CALCULATION_LIBRARY_H_

#include <DSCurrency.h>

using namespace DynSoft;

class CalculationLibrary {

	public:
		static DSCurrency Reduction(DSCurrency price, DSCurrency reduction);
		static DSCurrency GetAddTax(DSCurrency price, DSCurrency tax);
		static DSCurrency GetSubTax(DSCurrency price, DSCurrency tax);
		static DSCurrency AddTax(DSCurrency price, DSCurrency tax);
		static DSCurrency SubTax(DSCurrency price, DSCurrency tax);

};

#endif /* CALCULATION_LIBRARY_H_ */
