#include "ReportFileTypes.h"
#include <wx/xml/xml.h>

ReportFileTypes::ReportFileTypes(const wxString &version) : DSTemplateContainer<ReportFileType *>() {
	SetVersion(version);
}

ReportFileTypes::~ReportFileTypes() {
	Clean();
}

void ReportFileTypes::SetVersion(const wxString &version) {
	this->version = version;
}

wxString ReportFileTypes::GetVersion() const {
	return version;
}

void ReportFileTypes::Add(const wxString &name, const wxString &content) {
	Add(new ReportFileType(name, content));
}

ReportFileType *ReportFileTypes::Get(const wxString &name) {
	ReportFileType *reportFileType = NULL;

	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		reportFileType = Get(i);
		if(reportFileType && reportFileType->GetName() == name)
			return reportFileType;
	}

	return NULL;
}

void ReportFileTypes::Clean() {
	Clear(true);
}

void ReportFileTypes::Print(const bool withSeparators) {
	DS_PL(_N("Version: ") + GetVersion());
	ReportFileType *reportFileType = NULL;
	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		reportFileType = Get(i);
		if(reportFileType) {
			if(withSeparators)
				DS_PL(">> BEGIN <<");
			reportFileType->Print();
			if(withSeparators)
				DS_PL(">> END <<");
		}
	}
}

bool ReportFileTypes::Save(const wxString &filename) {
	wxXmlDocument doc;
	
	wxXmlProperty versionXmlProperty(_N("version"), GetVersion());
	wxXmlNode *root = new wxXmlNode(NULL, wxXML_ELEMENT_NODE, _N("ReportFile"), wxEmptyString, &versionXmlProperty);
	doc.SetRoot(root);

	wxXmlNode *entry               = NULL;
	wxXmlNode *name                = NULL;
	wxXmlNode *nameContent         = NULL;
	wxXmlNode *data                = NULL;
	wxXmlNode *dataContent         = NULL;
	ReportFileType *reportFileType = NULL;
	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		reportFileType = Get(i);
		if(reportFileType) {
			entry       = new wxXmlNode(root, wxXML_ELEMENT_NODE, _N("Entry"));
			name        = new wxXmlNode(entry, wxXML_ELEMENT_NODE, _N("Name"));
			nameContent = new wxXmlNode(name, wxXML_TEXT_NODE, wxEmptyString, reportFileType->GetName());
			data        = new wxXmlNode(entry, wxXML_ELEMENT_NODE, _N("Data"));
			dataContent = new wxXmlNode(data, wxXML_CDATA_SECTION_NODE, wxEmptyString, reportFileType->GetContent());
		}
	}

	return doc.Save(filename);
}

bool ReportFileTypes::Load(const wxString &filename, const bool clean) {
	if(clean)
		Clean();

	wxXmlDocument doc;
	if(!doc.Load(filename))
		return false;

	wxXmlNode *root = doc.GetRoot();
	if(root && root->GetName() != _N("ReportFile"))
		return false;
	
	SetVersion(root->GetPropVal(_N("version"), wxEmptyString));

	wxString name;
	wxString content;
	wxXmlNode *data    = NULL;
	wxXmlNode *entry   = NULL;
	wxXmlNode *entries = root->GetChildren();
	while(entries) {
		if(entries->GetName() == _N("Entry")) {
			name.Clear();
			content.Clear();

			entry = entries->GetChildren();
			while(entry) {
				if(entry->GetName() == _N("Name"))
					name = entry->GetNodeContent();
				else if(entry->GetName() == _N("Data"))
					content = entry->GetNodeContent();
				entry = entry->GetNext();
			}

			if(!name.IsEmpty())
				Add(name, content);
		}
		entries = entries->GetNext();
	}

	return true;
}

wxString ReportFileTypes::TypeToString(const int type) {
	wxString typeAsString;

	switch(type) {
		case wxXML_ELEMENT_NODE: typeAsString = _N("wxXML_ELEMENT_NODE"); break;
		case wxXML_ATTRIBUTE_NODE: typeAsString = _N("wxXML_ATTRIBUTE_NODE"); break;
		case wxXML_TEXT_NODE: typeAsString = _N("wxXML_TEXT_NODE"); break;
		case wxXML_CDATA_SECTION_NODE: typeAsString = _N("wxXML_CDATA_SECTION_NODE"); break;
		case wxXML_ENTITY_REF_NODE: typeAsString = _N("wxXML_ENTITY_REF_NODE"); break;
		case wxXML_ENTITY_NODE: typeAsString = _N("wxXML_ENTITY_NODE"); break;
		case wxXML_PI_NODE: typeAsString = _N("wxXML_PI_NODE"); break;
		case wxXML_COMMENT_NODE: typeAsString = _N("wxXML_COMMENT_NODE"); break;
		case wxXML_DOCUMENT_NODE: typeAsString = _N("wxXML_DOCUMENT_NODE"); break;
		case wxXML_DOCUMENT_TYPE_NODE: typeAsString = _N("wxXML_DOCUMENT_TYPE_NODE"); break;
		case wxXML_DOCUMENT_FRAG_NODE: typeAsString = _N("wxXML_DOCUMENT_FRAG_NODE"); break;
		case wxXML_NOTATION_NODE: typeAsString = _N("wxXML_NOTATION_NODE"); break;
		case wxXML_HTML_DOCUMENT_NODE: typeAsString = _N("wxXML_HTML_DOCUMENT_NODE"); break;
		default: typeAsString = _N("Unknown"); break;
	}

	return typeAsString;
}
