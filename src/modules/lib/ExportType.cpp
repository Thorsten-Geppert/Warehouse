#include "ExportType.h"

ExportType::ExportType() {
	fieldDelimiter  = _N(";");
	textDelimiter   = _N("\"");
	replaceLineFeed = false;
	SetLineFeed(0);
}

ExportType::ExportType(
	const wxString &filename,
	const wxString &fieldDelimiter,
	const wxString &textDelimiter,
	const unsigned int lineFeed,
	const wxString &replaceLineFeedWith,
	const bool replaceLineFeed
) {
	SetFilename(filename);
	SetFieldDelimiter(fieldDelimiter);
	SetTextDelimiter(textDelimiter);
	SetLineFeed(lineFeed);
	SetReplaceLineFeedWith(replaceLineFeedWith);
	SetReplaceLineFeed(replaceLineFeed);
}

void ExportType::SetFilename(const wxString &filename) {
	this->filename = filename;
}

wxString ExportType::GetFilename() const {
	return filename;
}

void ExportType::SetFieldDelimiter(const wxString &fieldDelimiter) {
	this->fieldDelimiter = fieldDelimiter;
}

wxString ExportType::GetFieldDelimiter() const {
	return fieldDelimiter;
}

void ExportType::SetTextDelimiter(const wxString &textDelimiter) {
	this->textDelimiter = textDelimiter;
}

wxString ExportType::GetTextDelimiter() const {
	return textDelimiter;
}

void ExportType::SetLineFeed(const unsigned int lineFeed) {
	this->lineFeed = lineFeed;
}

unsigned int ExportType::GetLineFeed() const {
	return lineFeed;
}

void ExportType::SetReplaceLineFeedWith(const wxString &replaceLineFeedWith) {
	this->replaceLineFeedWith = replaceLineFeedWith;
}

wxString ExportType::GetReplaceLineFeedWith() const {
	return replaceLineFeedWith;
}

void ExportType::SetReplaceLineFeed(const bool replaceLineFeed) {
	this->replaceLineFeed = replaceLineFeed;
}

bool ExportType::GetReplaceLineFeed() const {
	return replaceLineFeed;
}
