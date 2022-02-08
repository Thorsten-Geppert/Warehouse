#include "RemindersDialog.h"
#include <DSDateTime.h>
#include "../lib/ReminderLibrary.h"
#include "../lib/RemindersType.h"
#include "Configuration.h"
#include "YesNoDialog.h"
#include "resources/images/WarehouseIcon.xpm"
#include "RemindersEditDialog.h"

BEGIN_EVENT_TABLE(RemindersDialog, wxDialog)
	EVT_MENU(ADD_EVENT, RemindersDialog::AddKeyEvent)
	EVT_MENU(REMOVE_EVENT, RemindersDialog::RemoveKeyEvent)
	EVT_MENU(CANCEL_EVENT, RemindersDialog::CancelKeyEvent)
	EVT_LISTBOX(XRCID("remindersListBox"), RemindersDialog::SelectEvent)
	EVT_LISTBOX_DCLICK(XRCID("remindersListBox"), RemindersDialog::EditEvent)
END_EVENT_TABLE()

RemindersDialog::RemindersDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const unsigned int module,
	const unsigned int referenceId
) {
	this->rit         = rit;
	this->module      = module;;
	this->referenceId = referenceId;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("RemindersDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("RemindersDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar         = XRCCTRL(*this, "mainToolbar", Toolbar);	
	addToolbarButton    = XRCCTRL(*this, "addToolbarButton", ToolbarButton);
	removeToolbarButton = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	remindersListBox    = XRCCTRL(*this, "remindersListBox", wxListBox);
	notesTextCtrl       = XRCCTRL(*this, "notesTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(addToolbarButton, RemindersDialog::AddEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, RemindersDialog::RemoveEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, RemindersDialog::CancelEvent);
	
	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'a',  ADD_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,   (int) 'd',  REMOVE_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(400, 600));

	remindersListBox->SetFocus();

	Update();

	ICON();
}

RemindersDialog::~RemindersDialog() {
	CleanUp();
}

// Events
void RemindersDialog::AddEvent(wxMouseEvent &event) {
	Add();
	SKIP();
}

void RemindersDialog::AddKeyEvent(wxCommandEvent &event) {
	Add();
}

void RemindersDialog::EditEvent(wxCommandEvent &event) {
	Edit();
}

void RemindersDialog::SelectEvent(wxCommandEvent &event) {
	Select();
}

void RemindersDialog::RemoveEvent(wxMouseEvent &event) {
	Remove();
	SKIP();
}

void RemindersDialog::RemoveKeyEvent(wxCommandEvent &event) {
	Remove();
}

void RemindersDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void RemindersDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

// Methods
void RemindersDialog::Add() {
	if(!rit->Access(A_WRITE, _("You have no permission to add reminders."), this))
		return;

	DSDateTime dt;
	dt.SetWxDateTime(wxDateTime::Now());

	ReminderType *reminderType = new ReminderType(
		0,
		rit->GetUserConfigurationFileEntity()->GetLastUsername(),
		module,
		referenceId,
		wxEmptyString,
		dt,
		wxEmptyString
	);

	RemindersEditDialog *remindersEditDialog = new RemindersEditDialog(
		this,
		-1,
		rit,
		reminderType
	);
	remindersEditDialog->Center();
	if(remindersEditDialog->ShowModal() == wxID_OK) {
		AddOrChangeToRemindersListBox(reminderType);
	} else {
		delete reminderType;
	}
	remindersEditDialog->Destroy();
}

void RemindersDialog::Edit() {
	if(!rit->Access(A_WRITE, _("You have no permission to edit reminders."), this))
		return;

	ReminderType *reminderType = static_cast<ReminderType *>(remindersListBox->GetClientData(remindersListBox->GetSelection()));

	if(!reminderType) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to edit."),
			_("Edit")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	RemindersEditDialog *remindersEditDialog = new RemindersEditDialog(
		this,
		-1,
		rit,
		reminderType
	);
	remindersEditDialog->Center();
	if(remindersEditDialog->ShowModal() == wxID_OK)
		AddOrChangeToRemindersListBox(reminderType);
	remindersEditDialog->Destroy();
}

void RemindersDialog::Remove() {
	if(!rit->Access(A_WRITE, _("You have no permission to remove reminders."), this))
		return;

	ReminderType *reminderType = static_cast<ReminderType *>(remindersListBox->GetClientData(remindersListBox->GetSelection()));

	if(!reminderType) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to set done."),
			_("Done")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to set this remind done?"),
		_("Close"),
		_("Done"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		ReminderLibrary reminderLibrary(rit);
		if(!reminderLibrary.Delete(reminderType->GetId())) {
			rit->Log(true, _N("RemindersDialog"), _("Could not set done. Database error."), reminderType->ToString());
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not set done. Database error."),
				_("Save")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		} else {
			rit->Log(false, _N("RemindersDialog"), _("Reminder set as done."), reminderType->ToString());
			remindersListBox->Delete(remindersListBox->GetSelection());
			delete reminderType;
			notesTextCtrl->Clear();
		}
	}
	yesNoDialog->Destroy();
}

void RemindersDialog::Cancel() {
	Close();
}

void RemindersDialog::Update() {
	if(!rit->Access(A_WRITE, _("You have no permission to read reminders."), this))
		return;

	CleanUp();

	ReminderLibrary reminderLibrary(rit);
	RemindersType *remindersType = reminderLibrary.Get(rit->GetUserConfigurationFileEntity()->GetLastUsername(), referenceId, module);
	const unsigned int count = remindersType->GetCount();
	for(unsigned int i = 0; i < count; i++)
		AddOrChangeToRemindersListBox(remindersType->Get(i));
	delete remindersType;

	notesTextCtrl->Clear();
}

void RemindersDialog::CleanUp() {
	const unsigned int count = remindersListBox->GetCount();
	for(unsigned int i = 0; i < count; i++)
		delete static_cast<ReminderType *>(remindersListBox->GetClientData(i));
	remindersListBox->Clear();
}

void RemindersDialog::AddOrChangeToRemindersListBox(ReminderType *reminderType) {
	if(!reminderType)
		return;
	
	unsigned int selection = 0;
	bool found = false;
	const unsigned int count = remindersListBox->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		if(static_cast<ReminderType *>(remindersListBox->GetClientData(i)) == reminderType) {
			selection = i;
			found = true;
			break;
		}
	}

	const wxString dateTime(reminderType->GetDateTime().ToDatabaseDateTime());
	if(!found) {
		remindersListBox->Append(dateTime, reminderType);
	} else {
		remindersListBox->SetString(selection, dateTime);
		notesTextCtrl->SetValue(reminderType->GetNotes());
	}
}

void RemindersDialog::Select() {
	if(!rit->Access(A_READ, _("You have no permission to open reminders."), this))
		return;

	ReminderType *reminderType = static_cast<ReminderType *>(remindersListBox->GetClientData(remindersListBox->GetSelection()));
	if(reminderType)
		notesTextCtrl->SetValue(reminderType->GetNotes());
}
