#ifndef BUSYINFO_H_
#define BUSYINFO_H_

#include <wx/wx.h>
#include <wx/busyinfo.h>

class BusyInfo : public wxBusyInfo {

	public:
		BusyInfo(wxWindow *parent, const wxString &message = _("Please wait..."));

};

#endif /* BUSYINFO_H_ */
