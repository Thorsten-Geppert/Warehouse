#ifndef SQL_UPDATE_TYPE_H_
#define SQL_UPDATE_TYPE_H_

#include <DSLib.h>

class SqlUpdateType {

	public:
		SqlUpdateType();
		SqlUpdateType(
			wxArrayString &versions,
			const wxString &description,
			const wxString &sql
		);
		SqlUpdateType(
			const wxString &version,
			const wxString &description,
			const wxString &sql
		);

		bool ExistsVersion(const wxString &version);
		unsigned int GetVersionCount() const;
		wxString GetVersion(const unsigned int number) const;
		wxString GetVersions(const wxString &delimiter = _N(", ")) const;
		void SetVersions(const wxArrayString &versions);
		void ClearVersions();
		void AddVersion(const wxString &version);
		wxString GetDescription() const;
		void SetDescription(const wxString &description);
		wxString GetSql() const;
		void SetSql(const wxString &sql);
		wxString ToString();

	protected:
		wxArrayString versions;
		wxString description;
		wxString sql;

};


#endif /* SQL_UPDATE_TYPE_H_ */
