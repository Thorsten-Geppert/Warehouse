#include "HtmlPrintoutLibrary.h"
#include "ReportLibrary.h"

HtmlPrintoutLibrary::HtmlPrintoutLibrary(
	const wxString &title,
	RuntimeInformationType *rit
) : wxHtmlPrintout(
	title
), pgHeader(
	0
), pgFooter(
	0
) {
	this->rit = rit;
	this->title = title;
	SetMargins(10, 5, 10, 10);
}

DSTemplate HtmlPrintoutLibrary::GetTemplateByFile(const wxString &file) {
	DSTemplate tmp(rit->GRP(_N("print"), _N("de")));
	tmp.SetFile(file);
	return tmp;
}

DSTemplate HtmlPrintoutLibrary::GetTemplateByDatabase(const wxString &name) {
	ReportLibrary reportLibrary(rit);
	DSTemplate tmp;
	tmp.SetContent(reportLibrary.Get(name));
	return tmp;
}

void HtmlPrintoutLibrary::SetWithContainer(const wxString &htmlText) {
	wxString container;
	container += _N("<html>");
	container += _N("<head>");
	container += _N("</head>");
	container += _N("<body><font size=\"1\">");
	container += htmlText;
	container += _N("</font></body>");
	container += _N("</html>");

	SetHtmlText(container);

	this->content = container;
}

void HtmlPrintoutLibrary::SetWithoutContainer(const wxString &htmlText) {
	SetHtmlText(htmlText);

	this->content = htmlText;
}

wxString HtmlPrintoutLibrary::CreateTableByListCtrl(DSListCtrl *listCtrl) {
	if(!listCtrl)
		return wxEmptyString;

	wxString html;
	html += _N("<table border=\"1\">");
	html += _N("<tr>");

	wxListItem listItem;
	const unsigned int columnCount = listCtrl->GetColumnCount();
	for(unsigned int i = 0; i < columnCount; i++) {
		listItem.SetMask(wxLIST_MASK_TEXT);
		listCtrl->GetColumn(i, listItem);
		html += _N("<th><b>") + listItem.GetText() + _N("</b></th>");
	}

	html += _N("</tr>");

	wxString align;
	const unsigned int rowCount = listCtrl->GetItemCount();
	for(unsigned int i = 0; i < rowCount; i++) {
		html += _N("<tr>");
		for(unsigned int j = 0; j < columnCount; j++) {
			listCtrl->GetColumn(j, listItem);
			switch(listItem.GetAlign()) {
				case wxLIST_FORMAT_RIGHT:
					align = _N("right");
					break;
				case wxLIST_FORMAT_CENTRE:
					align = _N("center");
					break;
				case wxLIST_FORMAT_LEFT:
				default:
					align = _N("left");
					break;
			}
			html += _N("<td align=\"") + align + _N("\">") + listCtrl->GetText(i, j) + _N("</td>");
		}
		html += _N("</tr>");
	}

	html += _N("</table>");

	return html;
}

void HtmlPrintoutLibrary::SetHeader(const wxString &header, const int pg) {
	this->header = header;
	this->pgHeader = pg;

	wxHtmlPrintout::SetHeader(header, pg);
}

wxString HtmlPrintoutLibrary::GetHeader() const {
	return header;
}

int HtmlPrintoutLibrary::GetPgHeader() const {
	return pgHeader;
}

void HtmlPrintoutLibrary::SetFooter(const wxString &footer, const int pg) {
	this->footer = footer;
	this->pgFooter = pg;

	wxHtmlPrintout::SetFooter(footer, pg);
}

wxString HtmlPrintoutLibrary::GetFooter() const {
	return footer;
}

int HtmlPrintoutLibrary::GetPgFooter() const {
	return pgFooter;
}

wxString HtmlPrintoutLibrary::GetContent() const {
	return content;
}

wxString HtmlPrintoutLibrary::GetTitle() const {
	return title;
}

HtmlPrintoutLibrary *HtmlPrintoutLibrary::Copy() {
	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(GetTitle(), rit);
	htmlPrintoutLibrary->SetHeader(GetHeader(), GetPgHeader());
	htmlPrintoutLibrary->SetFooter(GetFooter(), GetPgFooter());
	htmlPrintoutLibrary->SetWithoutContainer(GetContent());
	return htmlPrintoutLibrary;
}
