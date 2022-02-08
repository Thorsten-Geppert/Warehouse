#ifndef UPDATE_TYPE_H_
#define UPDATE_TYPE_H_

#include <wx/wx.h>

class UpdateType {

	public:
		UpdateType();
		UpdateType(
			const wxString &version,
			const wxString &information
		);

		void SetVersion(const wxString &version);
		wxString GetVersion() const;

		void SetInformation(const wxString &version);
		wxString GetInformation() const;
	
	private:
		wxString version;
		wxString information;

};

#endif /* UPDATE_TYPE_H_ */
