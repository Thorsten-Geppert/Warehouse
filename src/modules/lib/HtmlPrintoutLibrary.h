#ifndef HTML_PRINTOUT_LIBRARY_H_
#define HTML_PRINTOUT_LIBRARY_H_

#include <wx/html/htmprint.h>
#include <DSTemplate.h>
#include <DSListCtrl.h>
#include "RuntimeInformationType.h"

using namespace DynSoft;

class HtmlPrintoutLibrary : public wxHtmlPrintout {

	public:
		HtmlPrintoutLibrary(
			const wxString &title,
			RuntimeInformationType *rit
		);

		DSTemplate GetTemplateByFile(const wxString &file);
		DSTemplate GetTemplateByDatabase(const wxString &name);

		void SetWithContainer(const wxString &htmlText);
		void SetWithoutContainer(const wxString &htmlText);

		wxString CreateTableByListCtrl(DSListCtrl *listCtrl);

		void SetHeader(const wxString &header, const int pg = wxPAGE_ALL);
		wxString GetHeader() const;
		int GetPgHeader() const;

		void SetFooter(const wxString &footer, const int pg = wxPAGE_ALL);
		wxString GetFooter() const;
		int GetPgFooter() const;

		wxString GetContent() const;
		wxString GetTitle() const;

		HtmlPrintoutLibrary *Copy();
	
	private:
		RuntimeInformationType *rit;

		wxString header, content, footer;
		int pgHeader, pgFooter;
		wxString title;

};

#endif /* HTML_PRINTOUT_LIBRARY_H_ */
