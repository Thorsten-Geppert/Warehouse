#ifndef LOG_ENTITY_H_
#define LOG_ENTITY_H_

#include <DSEntity.h>
#include <DSLogType.h>

using namespace DynSoft;

class LogEntity : public DSEntity {

	public:
		LogEntity();
		static LogEntity *ConvertLogType(DSLogType *logType);

};

#endif /* LOG_ENTITY_H_ */
