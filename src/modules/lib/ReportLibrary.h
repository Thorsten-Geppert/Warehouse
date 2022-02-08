#ifndef REPORT_LIBRARY_H_
#define REPORT_LIBRARY_H_

#include "RuntimeInformationType.h"
#include "ReportTypes.h"

class ReportLibrary {

	public:
		ReportLibrary(RuntimeInformationType *rit);

		bool LoadFromFile(const wxString &filename, ReportType &reportType);
		void FillUp(ReportType &reportType);
		bool Save(ReportType &reportType);
		wxString Get(const wxString &name);
	
	protected:
		RuntimeInformationType *rit;

};

#endif /* REPORT_LIBRARY_H_ */
