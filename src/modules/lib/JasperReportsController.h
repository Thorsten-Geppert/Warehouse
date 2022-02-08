#ifndef JASPER_REPORTS_CONTROLLER_H_
#define JASPER_REPORTS_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "JasperReportsEntity.h"
#include "JasperReportsEntities.h"

using namespace DynSoft;

class JasperReportsController : public Controller {

	public:
		typedef enum { SHOW, PRINT, PRINTDIRECT, PDF } TodoType;

		typedef enum {
			UNKNOWN = 0,
			DONE,
			ERROR_COMMAND,
			ERROR_ARGUMENTS,
			ERROR_JDBC_LOAD,
			ERROR_DATA_ID,
			ERROR_TODO,
			ERROR_DATABASE_CONNECTION,
			ERROR_CONFIGURATION_OK,
			ERROR_CONFIGURATION_LOAD,
			ERROR_COMPANY_CONNECTION,
			ERROR_NO_REPORT_USE,
			ERROR_REPORT_GENERATION,
			ERROR_REPORT_FILE_DOES_NOT_EXIST,
			ERROR_DATA_NOT_FOUND,
			ERROR_REPORT_IN_TABLE_NOT_FOUND,
			ERROR_COMPANY_NOT_FOUND
		} ErrorType;

		JasperReportsController(
			RuntimeInformationType *rit
		);

		JasperReportsEntities *GetReports(const wxString &name);
		ErrorType DoReport(
			const wxString &report,
			const unsigned int id,
			const TodoType todo,
			wxString *generatedCommand = NULL
		);

		bool UseReports();

		ErrorType StringToErrorType(const wxString &error);
		wxString ErrorTypeToString(const ErrorType error);
		wxString GetErrorDescription(const ErrorType error);

	protected:
		wxString TodoToString(const TodoType todo);

};

#endif /* JASPER_REPORTS_CONTROLLER_H_ */
