#ifndef EXPORT_LIBRARY_H_
#define EXPORT_LIBRARY_H_

#include <wx/wx.h>
#include <DSFile.h>
#include "EntityContainer.h"
#include "ExportType.h"
#include "../warehouse/ExportProgressDialog.h"

class ExportLibrary {

	public:
		ExportLibrary(
			ExportProgressDialog *exportProgressDialog,
			ExportType *exportType,
			const unsigned int exportCount,
			const bool exportCleanUp
		);
		~ExportLibrary();

		void ExportAdd(DSEntity *entity);
		bool ExportStart();
		bool ExportRun();
		bool ExportStop();
	
	protected:
		ExportProgressDialog *exportProgressDialog;
		ExportType *exportType;
		unsigned int exportCount;
		unsigned int exportInternalCounter;
		EntityContainer exportEntityContainer;
		DSFile exportFile;

};

#endif /* EXPORT_LIBRARY_H_ */
