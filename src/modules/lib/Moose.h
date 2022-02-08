#ifndef MOOSE_H_
#define MOOSE_H_

#include "RuntimeInformationType.h"

class Moose {

	public:
		Moose(RuntimeInformationType *rit);

		void SetRuntimeInformationType(RuntimeInformationType *rit);
		RuntimeInformationType *GetRuntimeInformationType() const;
	
	protected:
		RuntimeInformationType *rit;

};

#endif /* MOOSE_H_ */
