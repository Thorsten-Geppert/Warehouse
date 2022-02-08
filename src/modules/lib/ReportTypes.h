/*

ReportType
|-> ReportAreasType
|   |-> ReportElementsType
|   |   |-> ReportPlaceholdersType
|   |   |-> ReportPlaceholdersType
|   |   |-> ReportPlaceholdersType
|   |-> ReportElementsType
|       |-> ReportPlaceholdersType
|       |-> ReportPlaceholdersType
|-> ReportAreasType
    |-> ReportElementsType
	    |-> ReportPlaceholdersType
		|-> ReportPlaceholdersType

*/

#ifndef REPORT_TYPES_H_
#define REPORT_TYPES_H_

#include <wx/wx.h>
#include <DSTemplateContainer.h>
#include "HtmlEditPanel.h"

using namespace DynSoft;

class ReportPlaceholdersType {

	public:
		ReportPlaceholdersType(const wxString &placeholder, const wxString &description = wxEmptyString);
		~ReportPlaceholdersType();

		void SetDirect(const wxString &placeholder, const wxString &description);
		void GetDirect(wxString *placeholder, wxString *description = NULL) const;
		wxString GetPlaceholder() const;
		wxString GetDescription() const;
	
	private:
		wxString placeholder;
		wxString description;

};

class ReportElementsType : public DSTemplateContainer<ReportPlaceholdersType *> {

	public:
		ReportElementsType(const wxString &element, const wxString &description = wxEmptyString, const wxString &content = wxEmptyString);
		~ReportElementsType();

		void SetDirect(const wxString &element, const wxString &description, const wxString &content = wxEmptyString);
		void GetDirect(wxString *element, wxString *description = NULL, wxString *content = NULL) const;
		wxString GetElement() const;
		wxString GetDescription() const;
		wxString GetContent() const;
		void SetContent(const wxString &content);
		HtmlEditPanel *GetHtmlEditPanel() const;
		HtmlEditPanel *CreateHtmlEditPanel(wxWindow *parent, wxWindowID id, RuntimeInformationType *rit);
		void SetHtmlEditPanel(HtmlEditPanel *htmlEditTextCtrl, const bool deleteOldIfExists = true);
		void TransferFromHtmlEditPanelToContent();
	
	private:
		wxString element;
		wxString description;
		wxString content;
		HtmlEditPanel *htmlEditTextCtrl;

};

class ReportAreasType : public DSTemplateContainer<ReportElementsType *> {

	public:
		ReportAreasType(const wxString &area, const wxString &description = wxEmptyString);
		~ReportAreasType();

		void SetDirect(const wxString &area, const wxString &description);
		void GetDirect(wxString *area, wxString *description = NULL) const;
		wxString GetArea() const;
		wxString GetDescription() const;
	
	private:
		wxString area;
		wxString description;

};

class ReportType : public DSTemplateContainer<ReportAreasType *> {

	public:
		ReportType();
		~ReportType();

		void Push(
			const wxString &area, const wxString &areaDescription,
			const wxString &element, const wxString &elementDescription,
			const wxString &placeholder, const wxString &placeholderDescription
		);

		void Transfer();

};

#endif /* REPORT_TYPES_H_ */
