#include "ReportTypes.h"

ReportPlaceholdersType::ReportPlaceholdersType(
	const wxString &placeholder,
	const wxString &description
) {
	SetDirect(placeholder, description);
}

ReportPlaceholdersType::~ReportPlaceholdersType() {
}

void ReportPlaceholdersType::SetDirect(const wxString &placeholder, const wxString &description) {
	this->placeholder = placeholder;
	this->description = description;
}

void ReportPlaceholdersType::GetDirect(wxString *placeholder, wxString *description) const {
	if(placeholder)
		*placeholder = GetPlaceholder();
	if(description)
		*description = GetDescription();
}

wxString ReportPlaceholdersType::GetPlaceholder() const {
	return placeholder;
}

wxString ReportPlaceholdersType::GetDescription() const {
	return description;
}

ReportElementsType::ReportElementsType(
	const wxString &element,
	const wxString &description,
	const wxString &content
) : DSTemplateContainer<ReportPlaceholdersType *>(
), htmlEditTextCtrl(
	NULL
) {
	SetDirect(element, description, content);
}

ReportElementsType::~ReportElementsType() {
	Clear(true);
}

void ReportElementsType::SetDirect(const wxString &element, const wxString &description, const wxString &content) {
	this->element = element;
	this->description = description;
	SetContent(content);
}

void ReportElementsType::GetDirect(wxString *element, wxString *description, wxString *content) const {
	if(element)
		*element = GetElement();
	if(description)
		*description = GetDescription();
	if(content)
		*content = GetContent();
}

wxString ReportElementsType::GetElement() const {
	return element;
}

wxString ReportElementsType::GetDescription() const {
	return description;
}

wxString ReportElementsType::GetContent() const {
	return content;
}

void ReportElementsType::SetContent(const wxString &content) {
	this->content = content;
}

HtmlEditPanel *ReportElementsType::GetHtmlEditPanel() const {
	return htmlEditTextCtrl;
}

HtmlEditPanel *ReportElementsType::CreateHtmlEditPanel(wxWindow *parent, wxWindowID id, RuntimeInformationType *rit) {
	if(htmlEditTextCtrl)
		return htmlEditTextCtrl;
	
	htmlEditTextCtrl = new HtmlEditPanel(parent, id, rit, GetContent());

	return htmlEditTextCtrl;
}

void ReportElementsType::SetHtmlEditPanel(HtmlEditPanel *htmlEditTextCtrl, const bool deleteOldIfExists) {
	if(this->htmlEditTextCtrl)
		delete this->htmlEditTextCtrl;
	
	this->htmlEditTextCtrl = htmlEditTextCtrl;
}

void ReportElementsType::TransferFromHtmlEditPanelToContent() {
	if(GetHtmlEditPanel()) {
		SetContent(GetHtmlEditPanel()->GetContent());
	}
}


ReportAreasType::ReportAreasType(const wxString &area, const wxString &description) : DSTemplateContainer<ReportElementsType *>() {
	SetDirect(area, description);
}

ReportAreasType::~ReportAreasType() {
	Clear(true);
}

void ReportAreasType::SetDirect(const wxString &area, const wxString &description) {
	this->area = area;
	this->description = description;
}

void ReportAreasType::GetDirect(wxString *area, wxString *description) const {
	if(area)
		*area = GetArea();
	if(description)
		*description = GetDescription();
}

wxString ReportAreasType::GetArea() const {
	return area;
}

wxString ReportAreasType::GetDescription() const {
	return description;
}

ReportType::ReportType() : DSTemplateContainer<ReportAreasType *>() {
}

ReportType::~ReportType() {
	Clear(true);
}

void ReportType::Push(
	const wxString &area, const wxString &areaDescription,
	const wxString &element, const wxString &elementDescription,
	const wxString &placeholder, const wxString &placeholderDescription
) {
	ReportAreasType *reportAreasType = NULL;
	const unsigned int areaCount = GetCount();
	for(unsigned int i = 0; i < areaCount && ! reportAreasType; i++)
		if(Get(i)->GetArea() == area)
			reportAreasType = Get(i);
	
	if(!reportAreasType) {
		reportAreasType = new ReportAreasType(area, areaDescription);
		Add(reportAreasType);
	} else {
		reportAreasType->SetDirect(area, areaDescription);
	}
	
	ReportElementsType *reportElementsType = NULL;
	const unsigned int elementsCount = reportAreasType->GetCount();
	for(unsigned int i = 0; i < elementsCount && !reportElementsType; i++)
		if(reportAreasType->Get(i)->GetElement() == element)
			reportElementsType = reportAreasType->Get(i);

	if(!reportElementsType) {
		reportElementsType = new ReportElementsType(element, elementDescription);
		reportAreasType->Add(reportElementsType);
	} else {
		reportElementsType->SetDirect(element, elementDescription);
	}

	ReportPlaceholdersType *reportPlaceholderType = NULL;
	const unsigned int placeholderCount = reportElementsType->GetCount();
	for(unsigned int i = 0; i < placeholderCount && !reportPlaceholderType; i++)
		if(reportElementsType->Get(i)->GetPlaceholder() == placeholder)
			reportPlaceholderType = reportElementsType->Get(i);
	
	if(!reportPlaceholderType) {
		reportPlaceholderType = new ReportPlaceholdersType(placeholder, placeholderDescription);
		reportElementsType->Add(reportPlaceholderType);
	} else {
		reportPlaceholderType->SetDirect(placeholder, placeholderDescription);
	}
}

void ReportType::Transfer() {
	unsigned int elementsCount    = 0;
	const unsigned int areasCount = GetCount();
	for(unsigned int i = 0; i < areasCount; i++) {
		elementsCount = Get(i)->GetCount();
		for(unsigned int j = 0; j < elementsCount; j++)
			Get(i)->Get(j)->TransferFromHtmlEditPanelToContent();
	}
}
