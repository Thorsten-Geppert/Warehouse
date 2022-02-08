#ifndef PGHBA_HOST_TYPE_H_
#define PGHBA_HOST_TYPE_H_

#include <wx/wx.h>

class PgHbaHostType {

	public:
		PgHbaHostType();
		PgHbaHostType(
			const wxString &type,
			const wxString &database,
			const wxString &user,
			const wxString &address,
			const wxString &subnet,
			const wxString &method
		);

		void SetType(const wxString &type);
		wxString GetType() const;

		void SetDatabase(const wxString &database);
		wxString GetDatabase() const;

		void SetUser(const wxString &user);
		wxString GetUser() const;

		void SetAddress(const wxString &address);
		wxString GetAddress() const;

		void SetSubnet(const wxString &subnet);
		wxString GetSubnet() const;

		void SetMethod(const wxString &method);
		wxString GetMethod() const;

		wxString ToLine();
	
	protected:
		wxString type;
		wxString database;
		wxString user;
		wxString address;
		wxString subnet;
		wxString method;

};

#endif /* PGHBA_HOST_TYPE_H_ */
