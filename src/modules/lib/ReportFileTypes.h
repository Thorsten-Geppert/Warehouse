#ifndef REPORT_FILE_TYPES_H_
#define REPORT_FILE_TYPES_H_

#include <DSTemplateContainer.h>
#include "ReportFileType.h"

using namespace DynSoft;

class ReportFileTypes : public DSTemplateContainer<ReportFileType *> {

	public:
		ReportFileTypes(const wxString &version = wxEmptyString);
		~ReportFileTypes();

		void SetVersion(const wxString &version);
		wxString GetVersion() const;

		using DSTemplateContainer::Add;
		using DSTemplateContainer::Get;

		void Add(const wxString &name, const wxString &content);
		ReportFileType *Get(const wxString &name);

		void Clean();
		void Print(const bool withSeparators = true);

		bool Save(const wxString &filename);
		bool Load(const wxString &filename, const bool clean = true);
		
		wxString TypeToString(const int type);
	
	protected:
		wxString version;

};

#endif /* REPORT_FILE_TYPES_H_ */
