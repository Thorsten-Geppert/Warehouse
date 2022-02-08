#ifndef DATABASETYPE_H_
#define DATABASETYPE_H_

#include <wx/wx.h>

class DatabaseType {

	public:	
		void SetHostname(const wxString &hostname);
		wxString GetHostname() const;

		void SetDatabase(const wxString &database);
		wxString GetDatabase() const;

		void SetUsername(const wxString &username);
		wxString GetUsername() const;

		void SetPassword(const wxString &password);
		wxString GetPassword() const;

		void SetPort(const wxString &port);
		wxString GetPort() const;

		wxString ToString();
	
	protected:
		wxString hostname;
		wxString database;
		wxString username;
		wxString password;
		wxString port;

};

#endif /* DATABASETYPE_H_ */
