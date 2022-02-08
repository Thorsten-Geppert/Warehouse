#ifndef REMINDERSDIALOG_H_
#define REMINDERSDIALOG_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ReminderType.h"

using namespace DynSoft;

class RemindersDialog : public wxDialog {

	public:
		RemindersDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int module,
			const unsigned int referenceId
		);
		~RemindersDialog();

	protected:
		enum {
			ADD_EVENT,
			REMOVE_EVENT,
			CANCEL_EVENT
		};
		
		DECLARE_EVENT_TABLE();
		
		void AddEvent(wxMouseEvent &event);
		void AddKeyEvent(wxCommandEvent &event);
		void EditEvent(wxCommandEvent &event);
		void SelectEvent(wxCommandEvent &event);
		void RemoveEvent(wxMouseEvent &event);
		void RemoveKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		
		void Add();
		void Edit();
		void Select();
		void Remove();
		void Cancel();
		void Update();
		void CleanUp();
		void AddOrChangeToRemindersListBox(ReminderType *reminderType);

		RuntimeInformationType *rit;
		unsigned int module;
		unsigned int referenceId;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		Toolbar       *mainToolbar;
		ToolbarButton *addToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *cancelToolbarButton;
		wxListBox     *remindersListBox;
		wxTextCtrl    *notesTextCtrl;

};

#endif /* REMINDERSDIALOG_H_ */
