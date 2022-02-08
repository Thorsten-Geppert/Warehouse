#ifndef WAREHOUSEFRAME_H_
#define WAREHOUSEFRAME_H_

#include <wx/wx.h>
#include <wx/timer.h>
#include "../lib/ContentPanel.h"
#include "../lib/RuntimeInformationType.h"
#include "../lib/Dock.h"
#include "ReminderDialog.h"

class WarehouseFrame : public wxFrame {

	public:
		WarehouseFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxPoint &pos,
			const wxSize &size
		);
		wxBoxSizer *mainBoxSizer;

		void Reminder(const bool showAndRaise = false);
	
	protected:
		DECLARE_EVENT_TABLE()

		enum {
			CLOSE_EVENT,
			PAGEUP_EVENT,
			PAGEDOWN_EVENT
		};

		void OnClose(wxCloseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void ReminderEvent(wxMouseEvent &event);
		void DemoVersionEvent(wxTimerEvent &event);
		void PageUpEvent(wxCommandEvent &event);
		void PageDownEvent(wxCommandEvent &event);

		void DemoVersion();
		void Page(const bool next);

		RuntimeInformationType *rit;
		wxTimer demoVersionTimer;
		unsigned int demoCounter;

		wxPanel *mainPanel;
		ContentPanel *contentPanel;
		ReminderDialog *reminderDialog;

		DockToggleButton *reminderDockToggleButton;
		Dock *dock;

};

#endif /* WAREHOUSEFRAME_H_ */
