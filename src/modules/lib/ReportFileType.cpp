#include "ReportFileType.h"
#include <DSLib.h>

ReportFileType::ReportFileType(
	const wxString &name,
	const wxString &content
) : nameLength(
	0
), contentLength(
	0
) {
	SetName(name);
	SetContent(content);
}

void ReportFileType::SetName(const wxString &name) {
	this->name = name;
	nameLength = name.Length();
}

wxString ReportFileType::GetName() const {
	return name;
}

void ReportFileType::SetContent(const wxString &content) {
	this->content = content;
	contentLength = content.Length();
}

wxString ReportFileType::GetContent() const {
	return content;
}

size_t ReportFileType::GetNameLength() const {
	return nameLength;
}

size_t ReportFileType::GetContentLength() const {
	return contentLength;
}

void ReportFileType::Print() {
	DS_PL(GetName());
	DS_PL(GetContent());
}
