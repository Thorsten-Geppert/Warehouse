#ifndef PURCHASE_ENTTITY_H_
#define PURCHASE_ENTTITY_H_

#include "InformationEntity.h"
#include "RuntimeInformationType.h"
#include "LinkedFileEntities.h"
#include "PurchasePositionEntities.h"

using namespace DynSoft;

class PurchaseEntity : public InformationEntity {

	public:
		PurchaseEntity(RuntimeInformationType *rit);
		~PurchaseEntity();

		PurchasePositionEntities *GetPositions();
		LinkedFileEntities *GetLinkedFileEntities();

		void Recalc();
		bool IsValid(wxArrayString *products);

		void SetSearchYear(const unsigned int searchYear);
		unsigned int GetSearchYear() const;
	
	private:
		PurchasePositionEntities positions;
		LinkedFileEntities linkedFileEntities;
		unsigned int searchYear;

};

#endif /* PURCHASE_ENTTITY_H_ */
