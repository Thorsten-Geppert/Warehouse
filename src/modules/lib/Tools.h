#ifndef TOOLS_H_
#define TOOLS_H_

#include <DSLib.h>

class Tools {

	public:
		static wxString ArrayStringToCommaSeparatedList(wxArrayString &list, const wxString &separator = _N(", "));
		static wxString Description(const wxString &value, const wxString &emptyValue = wxEmptyString, const wxString &key = wxEmptyString);
		static bool IsReallyEmpty(wxString string);
		static wxString RemoveLastZeros(wxString string);
		static wxString ConcatStringAndRemoveEmptyLines(wxString value, const wxString &separator = _N(", "));

};

#endif /* TOOLS_H_ */
