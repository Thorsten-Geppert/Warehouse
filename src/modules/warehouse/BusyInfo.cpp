#include "BusyInfo.h"

BusyInfo::BusyInfo(wxWindow *parent, const wxString &message) : wxBusyInfo(message, parent) {
	wxYield();
	wxYield();
	wxYield();
	wxYield();
	wxYield();
}
