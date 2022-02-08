#ifndef USERCONFIGURATIONFILEENTITY_H_
#define USERCONFIGURATIONFILEENTITY_H_

#include <wx/wx.h>
#include "WindowPreferencesType.h"
#include "DatabaseType.h"

class UserConfigurationFileEntity {

	public:
		UserConfigurationFileEntity();
		virtual ~UserConfigurationFileEntity();

		void SetLastEnterprise(const wxString &lastEnterprise);
		wxString GetLastEnterprise() const;

		void SetLastUsername(const wxString &lastUsername);
		wxString GetLastUsername() const;

		void SetMainWindowPreferences(WindowPreferencesType *mainWindowPreferences);
		WindowPreferencesType *GetMainWindowPreferences() const;

		void SetDatabase(DatabaseType *database);
		DatabaseType *GetDatabase() const;

		void SetLicenceNumber(const unsigned int licenceNumber);
		unsigned int GetLicenceNumber() const;

	protected:
		wxString lastEnterprise;
		wxString lastUsername;
		WindowPreferencesType *mainWindowPreferences;
		DatabaseType *database;
		unsigned int licenceNumber;

};

#endif /* USERCONFIGURATIONFILEENTITY_H_ */
