#ifndef REPORT_FILE_TYPE_H_
#define REPORT_FILE_TYPE_H_

#include <wx/wx.h>

class ReportFileType {

	public:
		ReportFileType(
			const wxString &name = wxEmptyString,
			const wxString &content = wxEmptyString
		);

		void SetName(const wxString &name);
		wxString GetName() const;

		void SetContent(const wxString &content);
		wxString GetContent() const;

		size_t GetNameLength() const;
		size_t GetContentLength() const;

		void Print();
	
	protected:
		wxString name;
		wxString content;
		size_t nameLength;
		size_t contentLength;

};

#endif /* REPORT_FILE_TYPE_H_ */
