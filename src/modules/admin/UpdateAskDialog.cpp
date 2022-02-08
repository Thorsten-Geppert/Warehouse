#include "UpdateAskDialog.h"

BEGIN_EVENT_TABLE(UpdateAskDialog, wxDialog)
	EVT_BUTTON(CANCEL_BUTTON_EVENT, UpdateAskDialog::CancelEvent)
	EVT_BUTTON(UPDATE_BUTTON_EVENT, UpdateAskDialog::UpdateEvent)
END_EVENT_TABLE()

UpdateAskDialog::UpdateAskDialog(
	wxWindow *parent,
	wxWindowID id,
	const wxString &from,
	const wxString &to
) : wxDialog(
	parent,
	id,
	_("Update"),
	wxDefaultPosition,
	wxDefaultSize
) {
	wxString informations(_(
		"You want to update the enterprise database. You will not be able to undo this action.\n\n"
		"First of all you should backup your database with Warehouse Admin.\n\n"
		"Then start the update process.\n\n"
		"Do not cancel the process or kill this application. Maybe this application will not react. This is normal. Stay tuned.\n\n"
		"The update process can take a time."
	));

	mainPanel = new wxPanel(this);
	mainBoxSizer = new wxBoxSizer(wxVERTICAL);

	dataBoxSizer = new wxBoxSizer(wxVERTICAL);

	headlineStaticText = new wxStaticText(mainPanel, -1, _("Do the update from ") + from + _(" to ") + to);
	headlineStaticText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	headlineStaticText->SetForegroundColour(wxColour(255, 0, 0));
	dataBoxSizer->Add(headlineStaticText);

	dataBoxSizer->AddSpacer(5);

	informationsTextCtrl = new wxTextCtrl(mainPanel, -1, informations, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
	dataBoxSizer->Add(informationsTextCtrl, 1, wxEXPAND);

	dataBoxSizer->AddSpacer(5);

	optionsBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	cancelButton = new wxButton(mainPanel, CANCEL_BUTTON_EVENT, _("Cancel"));
	updateButton = new wxButton(mainPanel, UPDATE_BUTTON_EVENT, _("I understand. Do the update."));
	optionsBoxSizer->Add(cancelButton);
	optionsBoxSizer->AddStretchSpacer();
	optionsBoxSizer->Add(updateButton);
	dataBoxSizer->Add(optionsBoxSizer, 0, wxEXPAND);

	mainBoxSizer->Add(dataBoxSizer, 1, wxEXPAND | wxALL, 15);
	mainPanel->SetSizer(mainBoxSizer);
	mainBoxSizer->SetSizeHints(this);

	SetSize(wxSize(480, 330));
}

void UpdateAskDialog::CancelEvent(wxCommandEvent &event) {
	Cancel();
}

void UpdateAskDialog::UpdateEvent(wxCommandEvent &event) {
	Update();
}

void UpdateAskDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void UpdateAskDialog::Update() {
	EndModal(wxID_OK);
}
