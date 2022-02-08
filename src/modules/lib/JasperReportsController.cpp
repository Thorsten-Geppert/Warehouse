#include "JasperReportsController.h"
#include <DSPostgreSQLQuery.h>
#include <DSCasts.h>
#include <wx/filename.h>
#include "CommonPreferencesEntity.h"
#include "CommonPreferencesController.h"

JasperReportsController::JasperReportsController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("jasper_reports"),
	_N("report"),
	_N("rank")
) {
}

JasperReportsEntities *JasperReportsController::GetReports(const wxString &name) {
	JasperReportsEntities *jasperReportsEntities = new JasperReportsEntities;
	
	JasperReportsEntity *jasperReportsEntity = NULL;
	JasperReportsController jasperReportsController(rit);
	jasperReportsController.Get(_N("name"), name);
	while(jasperReportsController.Run()) {
		jasperReportsEntity = new JasperReportsEntity(rit);
		jasperReportsController.Get(jasperReportsEntity);
		jasperReportsEntities->Add(jasperReportsEntity);
	}

	return jasperReportsEntities;
}

JasperReportsController::ErrorType JasperReportsController::DoReport(
	const wxString &report,
	const unsigned int id,
	const TodoType todo,
	wxString *generatedCommand
) {
	const wxString todoString(TodoToString(todo));
	const wxString userConfigurationFile(rit->GetUserConfigurationFileWithPath());
	const wxString company(rit->GetEnterprisesName());

	CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());
	CommonPreferencesController commonPreferencesController(rit->GetDatabase());
	commonPreferencesController.Get();
	if(commonPreferencesController.Run()) {
		commonPreferencesController.Get(&commonPreferencesEntity);
		commonPreferencesController.Reset();
	}

	wxString jrePath;
	const wxChar sep = wxFileName::GetPathSeparator();

	#ifdef __WXMAC__
		jrePath = commonPreferencesEntity.Get(_N("jreHomeMacOS"))->ToString();
		if(jrePath.Len() > 0 && jrePath[jrePath.Len() - 1] != sep) jrePath += sep;
		jrePath += _N("bin/java");
	#elif defined __WXMSW__
		jrePath = commonPreferencesEntity.Get(_N("jreHomeWindows"))->ToString();
		if(jrePath.Len() > 0 && jrePath[jrePath.Len() - 1] != sep) jrePath += sep;
		jrePath += _N("bin\\java.exe");
	#else
		jrePath = commonPreferencesEntity.Get(_N("jreHomeUnix"))->ToString();
		if(jrePath.Len() > 0 && jrePath[jrePath.Len() - 1] != sep) jrePath += sep;
		jrePath += _N("bin/java");
	#endif

	const wxString jar(rit->GetExecutablePath() + _N("reports") + sep + _N("reports.jar"));

	const wxString command(
		_N("\"") + jrePath + _N("\" -jar ") +
		_N("\"") + jar + _N("\" ") +
		_N("\"") + userConfigurationFile + _N("\" ") +
		_N("\"") + company + _N("\" ") +
		_N("\"") + report + _N("\" ") +
		_N("\"") + DSCasts::ToString(id) + _N("\" ") +
		todoString
	);

	if(generatedCommand)
		*generatedCommand = command;
	
	if(!wxFileExists(jar))
		return ERROR_COMMAND;

	wxArrayString stdioArrayString, stderrArrayString;
	wxExecute(command, stdioArrayString, stderrArrayString);

	if(stdioArrayString.GetCount() > 0)
		return StringToErrorType(stdioArrayString[0]);

	return DONE;
}

wxString JasperReportsController::TodoToString(const TodoType todo) {
	wxString todoString;

	switch(todo) {
		case SHOW:
			todoString = _N("show");
			break;
		case PRINT:
			todoString = _N("print");
			break;
		case PRINTDIRECT:
			todoString = _N("printdirect");
			break;
		case PDF:
			todoString = _N("pdf");
			break;
	}

	return todoString;
}

bool JasperReportsController::UseReports() {
	CommonPreferencesController commonPreferencesController(rit->GetDatabase());
	commonPreferencesController.Get();
	if(commonPreferencesController.Run()) {
		CommonPreferencesEntity commonPreferencesEntity(rit->GetDatabase());
		commonPreferencesController.Get(&commonPreferencesEntity);
		commonPreferencesController.Reset();
		return commonPreferencesEntity.Get(_N("reportUse"))->ToBool();
	}

	return false;
}

JasperReportsController::ErrorType JasperReportsController::StringToErrorType(const wxString &error) {
	if(error == _N("DONE")) return DONE;
	else if(error == _N("ERROR_COMMAND")) return ERROR_COMMAND;
	else if(error == _N("ERROR_ARGUMENTS")) return ERROR_ARGUMENTS;
	else if(error == _N("ERROR_JDBC_LOAD")) return ERROR_JDBC_LOAD;
	else if(error == _N("ERROR_DATA_ID")) return ERROR_DATA_ID;
	else if(error == _N("ERROR_TODO")) return ERROR_TODO;
	else if(error == _N("ERROR_DATABASE_CONNECTION")) return ERROR_DATABASE_CONNECTION;
	else if(error == _N("ERROR_CONFIGURATION_OK")) return ERROR_CONFIGURATION_OK;
	else if(error == _N("ERROR_CONFIGURATION_LOAD")) return ERROR_CONFIGURATION_LOAD;
	else if(error == _N("ERROR_COMPANY_CONNECTION")) return ERROR_COMPANY_CONNECTION;
	else if(error == _N("ERROR_NO_REPORT_USE")) return ERROR_NO_REPORT_USE;
	else if(error == _N("ERROR_REPORT_GENERATION")) return ERROR_REPORT_GENERATION;
	else if(error == _N("ERROR_REPORT_FILE_DOES_NOT_EXIST")) return ERROR_REPORT_FILE_DOES_NOT_EXIST;
	else if(error == _N("ERROR_DATA_NOT_FOUND")) return ERROR_DATA_NOT_FOUND;
	else if(error == _N("ERROR_REPORT_IN_TABLE_NOT_FOUND")) return ERROR_REPORT_IN_TABLE_NOT_FOUND;
	else if(error == _N("ERROR_COMPANY_NOT_FOUND")) return ERROR_COMPANY_NOT_FOUND;
	else return UNKNOWN;
}

wxString JasperReportsController::ErrorTypeToString(const ErrorType error) {
	wxString errorName;

	switch(error) {
		case DONE: errorName = _N("DONE"); break;
		case ERROR_COMMAND: errorName = _N("ERROR_COMMAND"); break;
		case ERROR_ARGUMENTS: errorName = _N("ERROR_ARGUMENTS"); break;
		case ERROR_JDBC_LOAD: errorName = _N("ERROR_JDBC_LOAD"); break;
		case ERROR_DATA_ID: errorName = _N("ERROR_DATA_ID"); break;
		case ERROR_TODO: errorName = _N("ERROR_TODO"); break;
		case ERROR_DATABASE_CONNECTION: errorName = _N("ERROR_DATABASE_CONNECTION"); break;
		case ERROR_CONFIGURATION_OK: errorName = _N("ERROR_CONFIGURATION_OK"); break;
		case ERROR_CONFIGURATION_LOAD: errorName = _N("ERROR_CONFIGURATION_LOAD"); break;
		case ERROR_COMPANY_CONNECTION: errorName = _N("ERROR_COMPANY_CONNECTION"); break;
		case ERROR_NO_REPORT_USE: errorName = _N("ERROR_NO_REPORT_USE"); break;
		case ERROR_REPORT_GENERATION: errorName = _N("ERROR_REPORT_GENERATION"); break;
		case ERROR_REPORT_FILE_DOES_NOT_EXIST: errorName = _N("ERROR_REPORT_FILE_DOES_NOT_EXIST"); break;
		case ERROR_DATA_NOT_FOUND: errorName = _N("ERROR_DATA_NOT_FOUND"); break;
		case ERROR_REPORT_IN_TABLE_NOT_FOUND: errorName = _N("ERROR_REPORT_IN_TABLE_NOT_FOUND"); break;
		case ERROR_COMPANY_NOT_FOUND: errorName = _N("ERROR_COMPANY_NOT_FOUND"); break;
		default: errorName = _N("UNKNOWN"); break;
	}

	return errorName;
}

wxString JasperReportsController::GetErrorDescription(const ErrorType error) {
	wxString description;

	switch(error) {
		case DONE: description = _("No error"); break;
		case ERROR_COMMAND: description = _("Could not execute command. Maybe the report subsystem is not in the correct path."); break;
		case ERROR_ARGUMENTS: description = _("The argument count is wrong. Please see documentation for the correct parameters."); break;
		case ERROR_JDBC_LOAD: description = _("The PostgreSQL JDBC Driver could not be loaded."); break;
		case ERROR_DATA_ID: description = _("The data with this id could not be found."); break;
		case ERROR_TODO: description = _("The selected todo is not available."); break;
		case ERROR_DATABASE_CONNECTION: description = _("Could not connect to main database."); break;
		case ERROR_CONFIGURATION_OK: description = _("Could not read configuration. May be incompleted (you need database fields)."); break;
		case ERROR_CONFIGURATION_LOAD: description = _("Could not load configuration. Not available or no permissions."); break;
		case ERROR_COMPANY_CONNECTION: description = _("Could not connect to company database."); break;
		case ERROR_NO_REPORT_USE: description = _("Reports are not turned on in Warehouse."); break;
		case ERROR_REPORT_GENERATION: description = _("The report could not be generated."); break;
		case ERROR_REPORT_FILE_DOES_NOT_EXIST: description = _("The report file does not exist in the given path."); break;
		case ERROR_DATA_NOT_FOUND: description = _("No data found."); break;
		case ERROR_REPORT_IN_TABLE_NOT_FOUND: description = _("The report is not available in database."); break;
		case ERROR_COMPANY_NOT_FOUND: description = _("The company does not exist."); break;
		default: description = _("Unknown error."); break;
	}

	return description;
}
