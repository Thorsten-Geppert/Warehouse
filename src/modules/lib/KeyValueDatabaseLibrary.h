#ifndef KEY_VALUE_DATABASE_LIBRARY_H_
#define KEY_VALUE_DATABASE_LIBRARY_H_

#include <DSType.h>
#include <DSCasts.h>
#include "RuntimeInformationType.h"
#include "KeysValuesType.h"

using namespace DynSoft;

class KeyValueDatabaseLibrary {

	public:
		KeyValueDatabaseLibrary(
			RuntimeInformationType *rit,
			const wxString &table
		);

		bool Set(const wxString &key, const wxString &value);
		DSCasts Get(const wxString &key);
		bool Exists(const wxString &key);
		bool Exists(const wxString &key, const wxString &value);
		KeysValuesType Get();
	
	protected:
		RuntimeInformationType *rit;
		DSType type;
		wxString table;

};

#endif /* KEY_VALUE_DATABASE_LIBRARY_H_ */
