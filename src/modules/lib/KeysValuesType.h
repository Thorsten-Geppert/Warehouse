#ifndef KEYS_VALUES_TYPE_H_
#define KEYS_VALUES_TYPE_H_

#include <DSTemplateContainer.h>

using namespace DynSoft;

class KeyValueType {

	public:
		KeyValueType(const wxString &key, const wxString &value);

		void Set(const wxString &key, const wxString &value);
		void Get(wxString *key, wxString *value) const;
		void SetKey(const wxString &key);
		wxString GetKey() const;
		void SetValue(const wxString &value);
		wxString GetValue() const;

		void SetEdited(const bool edited);
		bool GetEdited() const;
	
	protected:
		wxString key;
		wxString value;
		bool edited;

};

class KeysValuesType : public DSTemplateContainer<KeyValueType *> {

	public:
		KeysValuesType();
		~KeysValuesType();

		void SetKeyValue(const wxString &key, const wxString &value);
		wxString GetValueByKey(const wxString &key);
		bool Exists(const wxString &key, unsigned int *index = NULL);

};

#endif /* KEYS_VALUES_TYPE_H_ */
