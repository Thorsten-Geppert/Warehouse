#include "PermissionLibrary.h"

wxString PermissionLibrary::ToString(const unsigned int permission) {
	wxString tmp;
	switch(permission) {
		case 4:
			tmp = _("Administrator");
			break;
		case 3:
			tmp = _("Reader, Writer and Remover");
			break;
		case 2:
			tmp = _("Reader and Writer");
			break;
		case 1:
			tmp = _("Reader");
			break;
		case 0:
			tmp = _("Inactive");
			break;
		default:
			tmp = _("Unknown");
			break;
	}
	return tmp;
}
