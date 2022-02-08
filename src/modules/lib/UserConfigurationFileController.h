#ifndef USERCONFIGURATIONFILECONTROLLER_H_
#define USERCONFIGURATIONFILECONTROLLER_H_

#include <wx/wx.h>
#include "UserConfigurationFileEntity.h"

class UserConfigurationFileController {

	public:
		static const int ALL = 0;
		static const int GUI = 1;
		static const int DB  = 2;
		static const int SN  = 3;

		static bool CreateFile(const wxString &file);
		static bool Load(
			UserConfigurationFileEntity *configuration,
			const wxString &file
		);
		static bool Save(
			UserConfigurationFileEntity *configuration,
			const wxString &file,
			const int kind
		);

};

#endif /* USERCONFIGURATIONFILECONTROLLER_H_ */
