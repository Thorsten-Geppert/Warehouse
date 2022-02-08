#ifndef REPORT_FILLED_TYPE_H_
#define REPORT_FILLED_TYPE_H_

#include "ReportTypes.h"

class ReportFilledType : public ReportType {

	public:
		ReportFilledType();
		~ReportFilledType();

		bool LoadFromFile(const wxString &filename);

};

#endif /* REPORT_FILLED_TYPE_H_ */
