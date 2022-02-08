#include "ObjectType.h"

ObjectType::ObjectType(const wxString &name, void *object) {
	SetName(name);
	SetObject(object);
}

void ObjectType::SetName(const wxString &name) {
	this->name = name;
}

wxString ObjectType::GetName() const {
	return name;
}

void ObjectType::SetObject(void *object) {
	this->object = object;
}

void *ObjectType::GetObject() const {
	return object;
}
