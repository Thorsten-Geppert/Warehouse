#include "BlockType.h"

BlockType::BlockType(	
	const wxString &key,
	const wxString &value,
	void *data
) {
	Set(key, value, data);
}

void BlockType::Set(
	const wxString &key,
	const wxString &value,
	void *data
) {
	SetKey(key);
	SetValue(value);
	SetData(data);
}

void BlockType::SetKey(const wxString &key) {
	this->key = key;
}

void BlockType::SetValue(const wxString &value) {
	this->value = value;
}

void BlockType::SetData(void *data) {
	this->data = data;
}

wxString BlockType::GetKey() const {
	return key;
}

wxString BlockType::GetValue() const {
	return value;
}

void *BlockType::GetData() const {
	return data;
}
