#include "TemplateController.h"
#include <DSPostgreSQLQuery.h>

TemplateController::TemplateController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("templates"), // Table
	_N("templateId"),
	_N("name")
) {
	LoadResource(rit->GRP(_N("sql"), _N("Templates.res")));
}

bool TemplateController::ExistsByName(const wxString &name, const wxString &oldName) {
	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Templates.Exists")));
	query.Bind(_N("table"), GetTable(), false);
	query.Bind(_N("name"), name);
	query.Bind(_N("oldName"), oldName);
	query.Exec();
	query.NextRecord();
	const bool exists = query.GetField(_N("counter")).ToUnsignedInt() > 0;
	query.FreeResult();
	return exists;
}
