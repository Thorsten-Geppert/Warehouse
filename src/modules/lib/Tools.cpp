#include "Tools.h"

wxString Tools::ArrayStringToCommaSeparatedList(wxArrayString &list, const wxString &separator) {
	wxString s;

	const unsigned int count = list.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		s += list[i];
		if(i < count - 1)
			s + separator;
	}

	return s;
}

wxString Tools::Description(const wxString &value, const wxString &emptyValue, const wxString &key) {
	if(!value.IsEmpty())
		return key + value;
	
	if(!key.IsEmpty())
		return key + emptyValue;
	
	return emptyValue;
}

bool Tools::IsReallyEmpty(wxString string) {
	string.Replace(_N("\r"), _N(""));
	string.Replace(_N("\n"), _N(""));
	string = string.Trim();
	string = string.Trim(true);
	return string.IsEmpty();
}

wxString Tools::RemoveLastZeros(wxString string) {
	string.Replace(_N("."), _N(","));
	unsigned int forward      = 0;
	const unsigned int length = string.Length();
	for(unsigned int i = length - 1; i > 0; i--) {
		if(string[i] == ',') {
			forward++;
			break;
		}
		if(string[i] == '0')
			forward++;
		else
			break;
	}

	return string.Mid(0, length - forward);
}

wxString Tools::ConcatStringAndRemoveEmptyLines(wxString value, const wxString &separator) {
	value.Trim().Trim(false);
	value.Replace(_N("\r\n"), _N("\n"));
	value.Replace(_N("\r"), _N("\n"));
	
	wxString result;
	wxArrayString tmp(DynSoft::DSLib::Split(value, '\n'));
	const unsigned int count = tmp.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		if(!tmp[i].IsEmpty()) {
			result += tmp[i];
			if(i + 1 < count)
				result += separator;
		}
	}

	return result;
}
