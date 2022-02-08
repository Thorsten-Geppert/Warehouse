#ifndef REMINDER_INTERFACE_H_
#define REMINDER_INTERFACE_H_

#include <wx/wx.h>

class ReminderInterface {

	public:
		virtual wxString GetReminderDescription(const unsigned int id) = 0;
		virtual ~ReminderInterface();

};

#endif /* REMINDER_INTERFACE_H_ */
