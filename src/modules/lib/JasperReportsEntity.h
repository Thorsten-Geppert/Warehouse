#ifndef JASPER_REPORTS_ENTITY_H_
#define JASPER_REPORTS_ENTITY_H_

#include "Entity.h"

using namespace DynSoft;

class JasperReportsEntity : public Entity {

	public:
		JasperReportsEntity(RuntimeInformationType *rit);
	
	protected:
		RuntimeInformationType *rit;

};


#endif /* JASPER_REPORTS_ENTITY_H_ */
