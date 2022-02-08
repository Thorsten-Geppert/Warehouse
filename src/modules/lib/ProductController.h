#ifndef PRODUCT_CONTROLLER_H_
#define PRODUCT_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "ProductEntity.h"
#include "HtmlPrintoutLibrary.h"
#include "ReminderInterface.h"
#include "ReferencesType.h"

using namespace DynSoft;

class ProductController : public Controller, public ReminderInterface {

	public:
		ProductController(
			RuntimeInformationType *rit
		);

		void Find(ProductEntity *productEntity, const bool onlyAvailable = false);
		static int GetTax(RuntimeInformationType *rit, const unsigned int productId);
		HtmlPrintoutLibrary *Print(ProductEntity *productEntity);
		static ProductEntity *GetDirect(RuntimeInformationType *rit, const unsigned int id);
		void Change(ProductEntity *productEntity);

		ProductEntity *Load(const unsigned int productId);
		bool Save(ProductEntity *productEntity);

		virtual wxString GetReminderDescription(const unsigned int id);

		unsigned int GetClientReferences(
			const unsigned int primaryKey,
			ReferencesType *referencesType
		);
	
};

#endif /* PRODUCT_CONTROLLER_H_ */
