#ifndef ORDER_ENTTITY_H_
#define ORDER_ENTTITY_H_

#include "InformationEntity.h"
#include "RuntimeInformationType.h"
#include "OrderPositionEntities.h"
#include "OrderEntities.h"
#include "LinkedFileEntities.h"

using namespace DynSoft;

class OrderEntity : public InformationEntity {

	public:
		OrderEntity(RuntimeInformationType *rit);
		~OrderEntity();

		OrderPositionEntities *GetPositions();
		OrderEntities *GetLinkedOrders();

		wxString GetBankString();

		// Just for search
		void SetOnlyUnpayed(const bool onlyUnpayed);
		bool GetOnlyUnpayed() const;
		void SetSearchYear(const unsigned int searchYear);
		unsigned int GetSearchYear() const;

		bool IsValid(wxArrayString *products = NULL, wxArrayString *linkedOrders = NULL, wxArrayString *allreadyLinkedOrders = NULL, bool *isValidKind = NULL);
		void Recalc();

		LinkedFileEntities *GetLinkedFileEntities();
	
	private:
		OrderPositionEntities positions;
		bool onlyUnpayed;
		OrderEntities linkedOrders;
		LinkedFileEntities linkedFileEntities;
		unsigned int searchYear;

};

#endif /* ORDER_ENTTITY_H_ */
