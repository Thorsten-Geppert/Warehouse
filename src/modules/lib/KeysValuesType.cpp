#include "KeysValuesType.h"

KeyValueType::KeyValueType(const wxString &key, const wxString &value) {
	Set(key, value);
}

void KeyValueType::Set(const wxString &key, const wxString &value) {
	SetKey(key);
	SetValue(value);
}

void KeyValueType::Get(wxString *key, wxString *value) const {
	if(key)
		*key = GetKey();
	if(value)
		*value = GetValue();
}

void KeyValueType::SetKey(const wxString &key) {
	this->key = key;
}

wxString KeyValueType::GetKey() const {
	return key;
}

void KeyValueType::SetValue(const wxString &value) {
	this->value = value;
}

wxString KeyValueType::GetValue() const {
	return value;
}

void KeyValueType::SetEdited(const bool edited) {
	this->edited = edited;
}

bool KeyValueType::GetEdited() const {
	return edited;
}

KeysValuesType::KeysValuesType() : DSTemplateContainer<KeyValueType *>() {
}

KeysValuesType::~KeysValuesType() {
	Clear(true);
}

void KeysValuesType::SetKeyValue(const wxString &key, const wxString &value) {
	unsigned int index = 0;
	if(Exists(key, &index)) {
		Get(index)->SetValue(value);
		Get(index)->SetEdited(true);
	} else {
		Add(new KeyValueType(key, value));
	}
}

wxString KeysValuesType::GetValueByKey(const wxString &key) {
	unsigned int index = 0;
	if(Exists(key, &index))
		return Get(index)->GetValue();
	return wxEmptyString;
}

bool KeysValuesType::Exists(const wxString &key, unsigned int *index) {
	bool found = false;

	KeyValueType *keyValueType = NULL;
	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		keyValueType = Get(i);
		if(keyValueType && keyValueType->GetKey() == key) {
			if(index)
				*index = i;
			found = true;
			break;
		}
	}

	return found;
}
