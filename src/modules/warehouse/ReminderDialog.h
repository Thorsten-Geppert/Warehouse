#ifndef REMINDERDIALOG_H_
#define REMINDERDIALOG_H_

#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/calctrl.h>
#include <wx/datectrl.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ReminderType.h"

using namespace DynSoft;

class ReminderDialog : public wxDialog {

	public:
		ReminderDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~ReminderDialog();
		int ShowModal();
	
	protected:
		enum {
			CANCEL_EVENT,
			UPDATE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void OpenEvent(wxMouseEvent &event);
		void OpenKeyEvent(wxListEvent &event);
		void UpdateEvent(wxMouseEvent &event);
		void UpdateKeyEvent(wxCommandEvent &event);
		void CloseEvent(wxCloseEvent &event);
		void SelectEvent(wxListEvent &event);
		void SaveEvent(wxCommandEvent &event);
		void DoneEvent(wxCommandEvent &event);
		void OpenToRemindEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);
		void RemindEvent(wxTimerEvent &event);

		void Cancel();
		void Open();
		void Update();
		void EnableEditing(const bool enable);
		void AddOrChangeToRemindersListCtrl(ReminderType *reminderType);
		void CleanUp();
		void Select(ReminderType *reminderType);
		void Save();
		void Done();
		void CleanCalendars();
		void UpdateCalendars();
		void OpenToRemind();
		void TemplateSelect(wxObject *object);
		void Remind();

		RuntimeInformationType *rit;
		wxTimer timer;
		wxColour backgroundColour;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		ToolbarButton    *cancelToolbarButton;;
		ToolbarButton    *openToolbarButton;
		ToolbarButton    *updateToolbarButton;
		wxCalendarCtrl   *firstCalendarCtrl;
		wxCalendarCtrl   *secondCalendarCtrl;
		wxCalendarCtrl   *thirdCalendarCtrl;
		DSListCtrl       *remindersListCtrl;
		wxStaticText     *changeStaticText;
		wxDatePickerCtrl *reminderDatePickerCtrl;
		wxComboBox       *hourComboBox;
		wxComboBox       *minuteComboBox;
		wxButton         *saveButton;
		wxButton         *doneButton;
		wxStaticText     *notesStaticText;
		wxTextCtrl       *notesTextCtrl;
		wxCheckBox       *openToRemindCheckBox;

};

#endif /* REMINDERDIALOG_H_ */
