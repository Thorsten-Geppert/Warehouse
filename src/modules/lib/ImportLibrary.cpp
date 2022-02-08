#include "ImportLibrary.h"
#include <DSCasts.h>
#include <DSDebug.h>

ImportLibrary::ImportLibrary() {
	this->count           = 0;
}

ImportLibrary::ImportLibrary(const wxString &filename) {
	this->count           = 0;

	ImportSetFilename(filename);
}

void ImportLibrary::ImportSetFilename(const wxString &filename) {
	this->filename = filename;
}

bool ImportLibrary::ImportStart() {
	if(importFile.Open(filename, _N("r"))) {
		lines.Clear();

		wxString tmp;
		importFile.ReadAll(&tmp, wxConvISO8859_1); // Ugly, but if I replace all \r it explodes

		lines = DSLib::Split(tmp, '\n');
		count = lines.GetCount();

		if(importFile.Close())
			return true;
	}

	return false;
}

unsigned int ImportLibrary::ImportGetCount() const {
	return count;
}

wxString ImportLibrary::ImportGetLine(unsigned int row) {
	if(row < count - 1) {
		wxString tmp(lines[row]);
		tmp.Replace(_N("\r"), wxEmptyString);
		return tmp;
	}
	return wxEmptyString;
}

bool ImportLibrary::ImportStop() {
	return true;
}
