#ifndef JASPER_REPORTS_LIBRARY_H_
#define JASPER_REPORTS_LIBRARY_H_

#include <wx/wx.h>
#include "RuntimeInformationType.h"
#include "JasperReportsController.h"

class JasperReportsLibrary {

	public:
		static bool DoReport(
			wxWindow *parent,
			RuntimeInformationType *rit,
			const wxString &name,
			const unsigned int id,
			JasperReportsController::TodoType todo
		);

		static bool UseReports(RuntimeInformationType *rit);

};

#endif /* JASPER_REPORTS_LIBRARY_H_ */
