#include "UserDialog.h"

BEGIN_EVENT_TABLE(UserDialog, wxDialog)
	EVT_BUTTON(ABORT_EVENT, UserDialog::AbortEvent)
	EVT_BUTTON(ADD_EVENT, UserDialog::AddEvent)
END_EVENT_TABLE()

UserDialog::UserDialog(
	wxWindow *parent,
	wxWindowID id
) : wxDialog(
	parent,
	id,
	_("User"),
	wxDefaultPosition,
	wxDefaultSize
) {
	const wxString informations(_(
		"Please notice: if the user exists, his password will be changed, otherwise "
		"the user will be created."
	));

	mainPanel = new wxPanel(this, -1);
	mainBoxSizer = new wxBoxSizer(wxVERTICAL);

	informationTextCtrl = new wxTextCtrl(mainPanel, -1, informations, wxDefaultPosition, wxSize(-1, 100), wxTE_READONLY | wxTE_MULTILINE);
	mainBoxSizer->Add(informationTextCtrl, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 15);

	mainBoxSizer->AddSpacer(5);
	informationStaticLine = new wxStaticLine(mainPanel);
	mainBoxSizer->Add(informationStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	dataFlexGridSizer = new wxFlexGridSizer(2, 5, 5);
	dataFlexGridSizer->AddGrowableCol(1);

	usernameStaticText = new wxStaticText(mainPanel, -1, _("Username:"));
	usernameTextCtrl = new wxTextCtrl(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxSize(150, -1));
	usernameTextCtrl->SetMaxLength(64);
	dataFlexGridSizer->Add(usernameStaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(usernameTextCtrl, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	password1StaticText = new wxStaticText(mainPanel, -1, _("Password:"));
	password1TextCtrl = new wxTextCtrl(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxTE_PASSWORD);
	password1TextCtrl->SetMaxLength(64);
	dataFlexGridSizer->Add(password1StaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(password1TextCtrl, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	password2StaticText = new wxStaticText(mainPanel, -1, _("Repeat password:"));
	password2TextCtrl = new wxTextCtrl(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxSize(150, -1), wxTE_PASSWORD);
	password2TextCtrl->SetMaxLength(64);
	dataFlexGridSizer->Add(password2StaticText, 0, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
	dataFlexGridSizer->Add(password2TextCtrl, 0, wxEXPAND | wxALIGN_CENTER_VERTICAL);

	mainBoxSizer->Add(dataFlexGridSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 15);

	optionsStaticLine = new wxStaticLine(mainPanel);
	mainBoxSizer->Add(optionsStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	optionsBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	abortButton = new wxButton(mainPanel, ABORT_EVENT, _("Abort"));
	addButton = new wxButton(mainPanel, ADD_EVENT, _("Add"));
	optionsBoxSizer->Add(abortButton);
	optionsBoxSizer->AddSpacer(5);
	optionsBoxSizer->Add(addButton);

	mainBoxSizer->Add(optionsBoxSizer, 0, wxALIGN_RIGHT | wxRIGHT | wxLEFT | wxBOTTOM, 15);
	
	mainPanel->SetSizer(mainBoxSizer);

	mainBoxSizer->SetSizeHints(this);

	addButton->SetDefault();
	usernameTextCtrl->SetFocus();
}

void UserDialog::AbortEvent(wxCommandEvent &event) {
	Abort();
}

void UserDialog::AddEvent(wxCommandEvent &event) {
	Add();
}

void UserDialog::Abort() {
	EndModal(wxID_CANCEL);
}

void UserDialog::Add() {
	const wxString username(usernameTextCtrl->GetValue());
	const wxString password1(password1TextCtrl->GetValue());
	const wxString password2(password2TextCtrl->GetValue());
	
	if(username.IsEmpty()) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("The username cannot be empty."),
			_("Add")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	if(password1 != password2) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("The passwords don't match."),
			_("Add")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	if(password1.IsEmpty()) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("The password cannot be empty."),
			_("Add")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	EndModal(wxID_OK);
}

void UserDialog::GetUsernameAndPassword(wxString &username, wxString &password) {
	username = usernameTextCtrl->GetValue();
	password = password1TextCtrl->GetValue();
}
