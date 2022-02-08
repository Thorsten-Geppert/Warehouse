#ifndef VOUCHER_ENTTITY_H_
#define VOUCHER_ENTTITY_H_

#include "InformationEntity.h"
#include "RuntimeInformationType.h"
#include "LinkedFileEntities.h"
#include "VoucherPositionEntities.h"

using namespace DynSoft;

class VoucherEntity : public InformationEntity {

	public:
		VoucherEntity(RuntimeInformationType *rit);
		LinkedFileEntities *GetLinkedFileEntities();
		VoucherPositionEntities *GetVoucherPositions();

		void SetSearchYear(const unsigned int searchYear);
		unsigned int GetSearchYear() const;

	protected:
		LinkedFileEntities linkedFileEntities;
		VoucherPositionEntities voucherPositionEntities;
		unsigned int searchYear;

};

#endif /* VOUCHER_ENTTITY_H_ */
