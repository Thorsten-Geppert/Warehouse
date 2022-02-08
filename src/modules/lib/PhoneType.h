#ifndef PHONE_TYPE_H_
#define PHONE_TYPE_H_

#include <DSLib.h>

using namespace DynSoft;

class PhoneType {

	public:	
		PhoneType();
		PhoneType(
			const wxString &country,
			const wxString &prefix,
			const wxString &number
		);

		void Set(
			const wxString &country,
			const wxString &prefix,
			const wxString &number
		);
		void SetCountry(const wxString &country);
		void SetPrefix(const wxString &prefix);
		void SetNumber(const wxString &number);

		void Get(
			wxString *country,
			wxString *prefix,
			wxString *number
		) const;
		wxString GetCountry() const;
		wxString GetPrefix() const;
		wxString GetNumber() const;

		wxString ToString() const;
	
	protected:
		wxString country;
		wxString prefix;
		wxString number;

};

#endif /* PHONE_TYPE_H_ */
