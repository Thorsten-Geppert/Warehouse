#include "ReportFilledType.h"
#include <DSFile.h>

ReportFilledType::ReportFilledType() {
}

bool ReportFilledType::LoadFromFile(const wxString &filename) {
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
			Push(
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

ReportFilledType::~ReportFilledType() {
	Clear(true);
}
