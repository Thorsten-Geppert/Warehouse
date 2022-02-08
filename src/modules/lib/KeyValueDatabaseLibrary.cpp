#include "KeyValueDatabaseLibrary.h"
#include <DSPostgreSQLQuery.h>

KeyValueDatabaseLibrary::KeyValueDatabaseLibrary(
	RuntimeInformationType *rit,
	const wxString &table
) {
	this->rit   = rit;
	this->table = table;

	type.LoadResource(rit->GRP(_N("sql"), _N("KeyValueDatabaseLibrary.res")));
}

bool KeyValueDatabaseLibrary::Set(const wxString &key, const wxString &value) {
	DSPostgreSQLQuery query(rit->GetDatabase());

	query.Prepare(type.GetResourceByName(
		Exists(key)
		?
		_N("KeyValueDatabaseLibrary.Update")
		:
		_N("KeyValueDatabaseLibrary.Insert")
	));

	query.Bind(_N("table"), table, false);
	query.Bind(_N("key"), key);
	query.Bind(_N("value"), value);

	return query.Exec(false);
}

DSCasts KeyValueDatabaseLibrary::Get(const wxString &key) {
	DSCasts value;

	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Prepare(type.GetResourceByName(_N("KeyValueDatabaseLibrary.Get")));
	query.Bind(_N("table"), table, false);
	query.Bind(_N("key"), key);
	query.Exec();
	if(query.NextRecord())
		value = query.GetField(_N("value"));
	query.FreeResult();

	return value;
}

bool KeyValueDatabaseLibrary::Exists(const wxString &key) {
	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Prepare(type.GetResourceByName(_N("KeyValueDatabaseLibrary.ExistsByKey")));
	query.Bind(_N("table"), table, false);
	query.Bind(_N("key"), key);
	query.Exec();
	query.NextRecord();
	const bool exists = query.GetField(_N("counter")).ToUnsignedInt() > 0;
	query.FreeResult();
	return exists;
}

bool KeyValueDatabaseLibrary::Exists(const wxString &key, const wxString &value) {
	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Prepare(type.GetResourceByName(_N("KeyValueDatabaseLibrary.ExistsByKeyAndValue")));
	query.Bind(_N("table"), table, false);
	query.Bind(_N("key"), key);
	query.Bind(_N("value"), value);
	query.Exec();
	query.NextRecord();
	const bool exists = query.GetField(_N("counter")).ToUnsignedInt() > 0;
	query.FreeResult();
	return exists;
}

KeysValuesType KeyValueDatabaseLibrary::Get() {
	KeysValuesType keysValues;

	DSPostgreSQLQuery query(rit->GetDatabase());
	query.Prepare(type.GetResourceByName(_N("KeyValueDatabaseLibrary.GetAll")));
	query.Bind(_N("table"), table, false);
	query.Exec();
	while(query.NextRecord())
		keysValues.SetKeyValue(
			query.GetField(_N("key")).ToString(),
			query.GetField(_N("value")).ToString()
		);
	query.FreeResult();

	return keysValues;
}
