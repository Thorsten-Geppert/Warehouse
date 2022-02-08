#ifndef ADDRESS_TYPE_H_
#define ADDRESS_TYPE_H_

#include <DSLib.h>

using namespace DynSoft;

class AddressType {

	public:
		AddressType();
		AddressType(
			const wxString &address,
			const wxString &number,
			const wxString &zipcode,
			const wxString &city,
			const wxString &federalState,
			const wxString &land
		);

		void Set(
			const wxString &address,
			const wxString &number,
			const wxString &zipcode,
			const wxString &city,
			const wxString &federalState,
			const wxString &land
		);
		void SetAddress(const wxString &address);
		void SetNumber(const wxString &number);
		void SetZipcode(const wxString &zipcode);
		void SetCity(const wxString &city);
		void SetFederalState(const wxString &federalState);
		void SetLand(const wxString &land);

		void Get(
			wxString *address,
			wxString *number,
			wxString *zipcode,
			wxString *city,
			wxString *federalState,
			wxString *land
		);
		wxString GetAddress() const;
		wxString GetNumber() const;
		wxString GetZipcode() const;
		wxString GetCity() const;
		wxString GetFederalState() const;
		wxString GetLand() const;

		static wxString GetAddressAndNumber(const wxString &address, const wxString &number);
		wxString GetAddressAndNumber() const;
		wxString GetConcatedAddress(const wxString &separator, const bool withFederalState = false, const bool withLand = true) const;
	
	protected:
		wxString address;
		wxString number;
		wxString zipcode;
		wxString city;
		wxString federalState;
		wxString land;

};

#endif /* ADDRESS_TYPE_H_ */
