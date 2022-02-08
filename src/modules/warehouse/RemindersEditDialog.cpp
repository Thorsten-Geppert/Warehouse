#include "RemindersEditDialog.h"
#include "../lib/ReminderLibrary.h"
#include "Configuration.h"
#include "YesNoDialog.h"
#include "TemplateSelectDialog.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(RemindersEditDialog, wxDialog)
	EVT_MENU(SAVE_EVENT, RemindersEditDialog::SaveKeyEvent)
	EVT_MENU(CANCEL_EVENT, RemindersEditDialog::CancelKeyEvent)
END_EVENT_TABLE()

RemindersEditDialog::RemindersEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	ReminderType *reminderType
) {
	this->rit          = rit;
	this->reminderType = reminderType;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("RemindersEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("RemindersEditDialog"));

	mainPanel              = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer           = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar            = XRCCTRL(*this, "mainToolbar", Toolbar);	
	saveToolbarButton      = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	cancelToolbarButton    = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	reminderDatePickerCtrl = XRCCTRL(*this, "reminderDatePickerCtrl", wxDatePickerCtrl);
	hourComboBox           = XRCCTRL(*this, "hourComboBox", wxComboBox);
	minuteComboBox         = XRCCTRL(*this, "minuteComboBox", wxComboBox);
	notesStaticText        = XRCCTRL(*this, "notesStaticText", wxStaticText);
	notesTextCtrl          = XRCCTRL(*this, "notesTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(saveToolbarButton, RemindersEditDialog::SaveEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, RemindersEditDialog::CancelEvent);

	notesStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(RemindersEditDialog::TemplateSelectEvent), NULL, this);
	
	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ReminderLibrary::FillTimeComboBoxes(hourComboBox, minuteComboBox);
	reminderDatePickerCtrl->SetValue(reminderType->GetDateTime());
	hourComboBox->SetSelection(reminderType->GetDateTime().GetHour());
	minuteComboBox->SetSelection(reminderType->GetDateTime().GetMinute());
	notesTextCtrl->SetValue(reminderType->GetNotes());

	reminderDatePickerCtrl->SetFocus();

	ICON();
}

// Events
void RemindersEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void RemindersEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void RemindersEditDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void RemindersEditDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void RemindersEditDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

// Methods
void RemindersEditDialog::Save() {
	if(!rit->Access(A_WRITE, _("You have no permission to save reminder."), this))
		return;

	DSDateTime dt;
	dt.SetWxDateTime(reminderDatePickerCtrl->GetValue());
	dt.SetHour(hourComboBox->GetSelection());
	dt.SetMinute(minuteComboBox->GetSelection());

	ReminderType *tmpReminderType = new ReminderType(*reminderType);
	tmpReminderType->SetDateTime(dt);
	tmpReminderType->SetNotes(notesTextCtrl->GetValue());

	ReminderLibrary reminderLibrary(rit);
	bool ok = reminderType->GetId() == 0 ? reminderLibrary.Insert(*tmpReminderType) : reminderLibrary.Update(*tmpReminderType);
	if(!ok) {
		rit->Log(true, _N("RemindersEditDialog"), _("Could not save. Database error."), tmpReminderType->ToString());
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not save. Database error."),
			_("Save")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		ok = false;
	} else {
		rit->Log(false, _N("RemindersEditDialog"), _("Reminder was saved."), tmpReminderType->ToString());
		reminderType->SetId(tmpReminderType->GetId());
		reminderType->SetDateTime(tmpReminderType->GetDateTime());
		reminderType->SetNotes(tmpReminderType->GetNotes());
		ok = true;
	}

	delete tmpReminderType;

	if(ok)
		EndModal(wxID_OK);
}

void RemindersEditDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void RemindersEditDialog::TemplateSelect(wxObject *object) {
	wxTextCtrl *textCtrl = NULL;

	if(object == notesStaticText)
		textCtrl = notesTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}
