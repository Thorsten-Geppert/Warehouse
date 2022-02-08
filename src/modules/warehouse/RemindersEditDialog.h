#ifndef REMINDERSEDITDIALOG_H_
#define REMINDERSEDITDIALOG_H_

#include <wx/wx.h>
#include <wx/datectrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ReminderType.h"

using namespace DynSoft;

class RemindersEditDialog : public wxDialog {

	public:
		RemindersEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			ReminderType *reminderType
		);

	protected:
		enum {
			SAVE_EVENT,
			CANCEL_EVENT
		};
		
		DECLARE_EVENT_TABLE();
		
		void SaveEvent(wxMouseEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);
		
		void Save();
		void Cancel();
		void TemplateSelect(wxObject *object);

		RuntimeInformationType *rit;
		ReminderType *reminderType;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		Toolbar          *mainToolbar;
		ToolbarButton    *saveToolbarButton;
		ToolbarButton    *cancelToolbarButton;
		wxDatePickerCtrl *reminderDatePickerCtrl;
		wxComboBox       *hourComboBox;
		wxComboBox       *minuteComboBox;
		wxStaticText     *notesStaticText;
		wxTextCtrl       *notesTextCtrl;

};

#endif /* REMINDERSEDITDIALOG_H_ */
