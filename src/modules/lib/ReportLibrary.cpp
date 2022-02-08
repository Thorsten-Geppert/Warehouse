#include "ReportLibrary.h"
#include <DSPostgreSQLQuery.h>
#include <DSFile.h>

using namespace DynSoft;

ReportLibrary::ReportLibrary(RuntimeInformationType *rit) {
	this->rit = rit;
}

bool ReportLibrary::LoadFromFile(const wxString &filename, ReportType &reportType) {
	DSFile file(filename, _N("r"));
	if(!file.IsOpened())
		return false;
	
	wxString tmp;
	if(!file.ReadAll(&tmp)) {
		file.Close();
		return false;
	}
	file.Close();

	wxArrayString fields;
	wxArrayString lines(DSLib::Split(tmp, '\n'));
	const unsigned int count = lines.GetCount();
	for(unsigned int i = 1; i < count; i++) {
		fields = DSLib::Split(lines[i], ';');
		if(fields.GetCount() == 6) {
			reportType.Push(
				fields[0],
				fields[1],
				fields[2],
				fields[3],
				fields[4],
				fields[5]
			);
		}
	}

	return true;
}

void ReportLibrary::FillUp(ReportType &reportType) {
	DSPostgreSQLQuery query(rit->GetDatabase());
	query.LoadResource(rit->GRP(_N("sql"), _N("Reports.res")));
	
	unsigned int elementsCount    = 0;
	const unsigned int areasCount = reportType.GetCount();
	for(unsigned int i = 0; i < areasCount; i++) {
		elementsCount = reportType.Get(i)->GetCount();
		for(unsigned int j = 0; j < elementsCount; j++) {
			query.PrepareByResource(_N("Reports.GetContent"));
			query.Bind(_N("name"), reportType.Get(i)->Get(j)->GetElement());
			query.Exec();
			if(query.NextRecord())
				reportType.Get(i)->Get(j)->SetContent(query.GetField(_N("content")).ToString());
			query.FreeResult();
		}
	}
}

bool ReportLibrary::Save(ReportType &reportType) {
	reportType.Transfer();

	DSPostgreSQLQuery query(rit->GetDatabase());
	query.LoadResource(rit->GRP(_N("sql"), _N("Reports.res")));

	query.Begin();
	query.PrepareByResource(_N("Reports.RemoveAll"));
	query.Exec(false);
	
	unsigned int elementsCount    = 0;
	const unsigned int areasCount = reportType.GetCount();
	for(unsigned int i = 0; i < areasCount; i++) {
		elementsCount = reportType.Get(i)->GetCount();
		for(unsigned int j = 0; j < elementsCount; j++) {
			query.PrepareByResource(_N("Reports.Save"));
			query.Bind(_N("name"), reportType.Get(i)->Get(j)->GetElement());
			query.Bind(_N("content"), reportType.Get(i)->Get(j)->GetContent());
			if(!query.Exec(false))
				break;
		}
	}

	if(!query.Commit()) {
		query.Rollback();
		return false;
	}

	return true;
}

wxString ReportLibrary::Get(const wxString &name) {
	wxString content;

	DSPostgreSQLQuery query(rit->GetDatabase());
	query.LoadResource(rit->GRP(_N("sql"), _N("Reports.res")));
	query.PrepareByResource(_N("Reports.Get"));
	query.Bind(_N("name"), name);
	query.Exec();
	if(query.NextRecord())
		content = query.GetField(_N("content")).ToString();
	query.FreeResult();

	return content;
}
