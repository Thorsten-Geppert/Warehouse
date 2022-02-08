#ifndef NAME_TYPE_H_
#define NAME_TYPE_H_

#include <DSLib.h>

using namespace DynSoft;

class NameType {

	public:
		NameType();
		NameType(
			const wxString &salutation,
			const wxString &title,
			const wxString &firstname,
			const wxString &name1,
			const wxString &name2,
			const wxString &name3,
			const wxString &name4
		);

		void Set(
			const wxString &salutation,
			const wxString &title,
			const wxString &firstname,
			const wxString &name1,
			const wxString &name2,
			const wxString &name3,
			const wxString &name4
		);

		void SetSalutation(const wxString &salutation);
		void SetTitle(const wxString &title);
		void SetFirstname(const wxString &firstname);
		void SetName1(const wxString &name1);
		void SetName2(const wxString &name2);
		void SetName3(const wxString &name3);
		void SetName4(const wxString &name4);

		void Get(
			wxString *salutation,
			wxString *title,
			wxString *firstname,
			wxString *name1,
			wxString *name2,
			wxString *name3,
			wxString *name4
		);

		wxString GetSalutation() const;
		wxString GetTitle() const;
		wxString GetFirstname() const;
		wxString GetName1() const;
		wxString GetName2() const;
		wxString GetName3() const;
		wxString GetName4() const;

		wxString GetConcatedName(const wxString &separator) const;
		wxString GetConcatedFullName(const wxString &separator) const;
	
	protected:
		wxString salutation;
		wxString title;
		wxString firstname;
		wxString name1;
		wxString name2;
		wxString name3;
		wxString name4;

};

#endif /* NAME_TYPE_H_ */
