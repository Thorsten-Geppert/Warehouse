#include "Controller.h"
#include <DSTemplate.h>

Controller::Controller(
	RuntimeInformationType *rit,
	const wxString &table,
	const wxString &primaryKey,
	const wxString &orderByField,
	const unsigned int order
) : Moose(
	rit
), DSPostgreSQLEntityData(
	rit->GetDatabase(),
	table,
	primaryKey,
	orderByField,
	order
) {
	SetRuntimeInformationType(rit);
	LoadResource(rit->GRP(_N("sql"), _N("KeysValues.res")));
}

wxString Controller::WildCard(const wxString &value, const bool NullIsEmpty) {
	return value.IsEmpty() || (NullIsEmpty && value == _N("0")) ? _N("%") : value;
}

bool Controller::SaveKeysValues(
	DSPostgreSQLQuery &query,
	const unsigned int id,
	const wxString &table,
	KeysValuesType &keysValuesType
) {
	query.Prepare(GetResourceByName(_N("KeysValues.Remove")));
	query.Bind(_N("table"), table, false);
	query.Bind(_N("id"), id);
	bool ok = query.Exec(false);
	if(ok) {
		KeyValueType *keyValueType = NULL;
		const unsigned int count = keysValuesType.GetCount();
		for(unsigned int i = 0; i < count; i++) {
			keyValueType = keysValuesType.Get(i);
			if(keyValueType) {
				query.Prepare(GetResourceByName(_N("KeysValues.Insert")));
				query.Bind(_N("table"), table, false);
				query.Bind(_N("id"), id);
				query.Bind(_N("key"), keyValueType->GetKey());
				query.Bind(_N("value"), keyValueType->GetValue());
				ok = query.Exec(false);

				if(!ok)
					break;
			}
		}
	}

	return ok;
}

unsigned int Controller::LoadKeysValues(
	const unsigned int id,
	const wxString &table,
	KeysValuesType &keysValuesType
) {
	unsigned int count = 0;

	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("KeysValues.Select")));
	query.Bind(_N("table"), table, false);
	query.Bind(_N("id"), id);
	query.Exec();
	while(query.NextRecord()) {
		keysValuesType.SetKeyValue(
			query.GetField(_N("key")).ToString(),
			query.GetField(_N("value")).ToString()
		);
	}
	query.FreeResult();

	return count;
}

wxString Controller::PrintKeysValues(HtmlPrintoutLibrary &htmlPrintoutLibrary, KeysValuesType &keysValuesType) {
	const unsigned int count = keysValuesType.GetCount();
	if(count == 0)
		return wxEmptyString;

	DSTemplate keyValueTemplate = htmlPrintoutLibrary.GetTemplateByDatabase(_N("KeyValue"));
	DSTemplate keyValueRowTemplate = htmlPrintoutLibrary.GetTemplateByDatabase(_N("KeyValueRow"));
	KeyValueType *keyValueType = NULL;
	for(unsigned int i = 0; i < count; i++) {
		keyValueType = keysValuesType.Get(i);
		if(keyValueType) {
			keyValueRowTemplate.SetVariable(_N("key"), keyValueType->GetKey(), true);
			keyValueRowTemplate.SetVariable(_N("value"), keyValueType->GetValue(), true);
			keyValueRowTemplate.Parse();
		}
	}

	keyValueTemplate.SetDynamicVariable(_N("rows"), keyValueRowTemplate.Get());
	keyValueTemplate.Parse();
	return keyValueTemplate.Get();
}
