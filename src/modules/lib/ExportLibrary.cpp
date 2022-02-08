#include "ExportLibrary.h"

ExportLibrary::ExportLibrary(
	ExportProgressDialog *exportProgressDialog,
	ExportType *exportType,
	const unsigned int exportCount,
	const bool exportCleanUp
) : exportEntityContainer(
	exportCleanUp
) {
	this->exportProgressDialog  = exportProgressDialog;
	this->exportType            = exportType;
	this->exportCount           = exportCount;
	this->exportCount           = 0;
	this->exportInternalCounter = 0;

	exportProgressDialog->Center();
	exportProgressDialog->SetGauge(exportProgressDialog->SECOND);
	exportProgressDialog->SetValue(0);
	exportProgressDialog->SetGauge(exportProgressDialog->FIRST);
	exportProgressDialog->SetValue(0);
	exportProgressDialog->SetCount(exportCount);
}

ExportLibrary::~ExportLibrary() {
}

void ExportLibrary::ExportAdd(DSEntity *entity) {
	exportEntityContainer.Add(entity);
	exportProgressDialog->SetGauge(exportProgressDialog->FIRST);
	exportProgressDialog->SetValue(++exportInternalCounter);
	wxYield();
}

bool ExportLibrary::ExportStart() {
	exportInternalCounter = 0;
	exportProgressDialog->SetGauge(exportProgressDialog->SECOND);
	exportProgressDialog->SetCount(exportEntityContainer.GetCount());
	exportProgressDialog->Start();
	return exportFile.Open(exportType->GetFilename(), _N("w"));
}

bool ExportLibrary::ExportRun() {
	wxString lineFeed;
	switch(exportType->GetLineFeed()) {
		case 2:
			lineFeed = _N("\r");
			break;
		case 1:
			lineFeed = _N("\r\n");
			break;
		case 0:
		default:
			lineFeed = _N("\n");
			break;
	}

	bool ok = true;
	DSEntity *entity = NULL;
	const unsigned int count = exportEntityContainer.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		entity = exportEntityContainer.Get(i);

		ok = true;
		if(i == 0)
			ok = exportFile.Write(entity->ToCsv(true, exportType->GetFieldDelimiter(), exportType->GetTextDelimiter(), exportType->GetReplaceLineFeedWith(), exportType->GetReplaceLineFeed()) + lineFeed);
		if(ok)
			ok = exportFile.Write(entity->ToCsv(false, exportType->GetFieldDelimiter(), exportType->GetTextDelimiter(), exportType->GetReplaceLineFeedWith(), exportType->GetReplaceLineFeed()) + lineFeed);

		if(!ok) {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				exportProgressDialog,
				_("Could not save file."),
				_("Export")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();

			exportProgressDialog->Stop();

			return false;
		}

		exportProgressDialog->SetGauge(exportProgressDialog->SECOND);
		exportProgressDialog->SetValue(++exportInternalCounter);

		wxYield();
	}

	return true;
}

bool ExportLibrary::ExportStop() {
	exportProgressDialog->Stop();
	return exportFile.Close();
}
