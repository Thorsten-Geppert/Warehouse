#ifndef UPDATE_LIBRARY_H_
#define UPDATE_LIBRARY_H_

#include <wx/wx.h>
#include <DSPostgreSQLDatabase.h>
#include "UpdateType.h"
#include "EnterpriseEntity.h"
#include "SqlUpdateType.h"

using namespace DynSoft;

class UpdateLibrary {

	public:
		UpdateLibrary(const wxString &url = wxEmptyString);

		UpdateType *CheckForUpdates(
			const wxString &version,
			wxString *errorMessage
		);

		wxArrayString GetVersions(const wxString &actualVersion);

		bool GetVersion(EnterpriseEntity &enterpriseEntity, wxString *version);
		bool SetVersion(EnterpriseEntity &enterpriseEntity, const wxString &version);
		bool AlterTable(EnterpriseEntity &enterpriseEntity, const wxString &table, const wxString &sql);

		DSPostgreSQLDatabase *GetDatabaseObject(EnterpriseEntity &enterpriseEntity);

		bool Update(EnterpriseEntity &enterpriseEntity, const wxString &version, wxTextCtrl *informationsTextCtrl);
		bool Query(EnterpriseEntity &enterpriseEntity, const wxString &queryString);

		SqlUpdateType *ParseWarehouseUpdateFile(const wxString &updateFile);
	
	private:
		wxString url;
		wxArrayString versions;

		bool Update_1_6_0(EnterpriseEntity &enterpriseEntity);

};

#endif /* UPDATE_LIBRARY_H_ */
