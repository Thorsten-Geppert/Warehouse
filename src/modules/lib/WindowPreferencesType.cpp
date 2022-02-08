#include "WindowPreferencesType.h"

void WindowPreferencesType::SetSize(const wxSize &size) {
	this->size = size;
}

void WindowPreferencesType::SetSize(const int width, const int height) {
	this->size = wxSize(width, height);
}	

wxSize WindowPreferencesType::GetSize() const {
	return size;
}

void WindowPreferencesType::GetSize(int *width, int *height) const {
	if(width)  *width  = GetSize().GetWidth();
	if(height) *height = GetSize().GetHeight();
}


void WindowPreferencesType::SetPosition(const wxPoint &position) {
	this->position = position;
}

void WindowPreferencesType::SetPosition(const int x, const int y) {
	this->position = wxPoint(x, y);
}

wxPoint WindowPreferencesType::GetPosition() const {
	return position;
}

void WindowPreferencesType::GetPosition(int *x, int *y) const {
	if(x) *x = GetPosition().x;
	if(y) *y = GetPosition().y;
}

