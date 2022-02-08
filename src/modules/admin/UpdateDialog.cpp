#include "UpdateDialog.h"
#include <DSPostgreSQLDatabase.h>
#include "../lib/UpdateLibrary.h"
#include "UpdateAskDialog.h"

BEGIN_EVENT_TABLE(UpdateDialog, wxDialog)
	EVT_BUTTON(UPDATE_BUTTON_EVENT, UpdateDialog::UpdateEvent)
END_EVENT_TABLE()

UpdateDialog::UpdateDialog(
	wxWindow *parent,
	wxWindowID id,
	EnterpriseEntity *enterpriseEntity,
	wxArrayString *versions,
	const wxString &from
) : wxDialog(
	parent,
	id,
	_("Update enterprise"),
	wxDefaultPosition,
	wxDefaultSize
) {
	this->versions = versions;
	this->enterpriseEntity = enterpriseEntity;
	this->from = from;

	mainPanel = new wxPanel(this, -1);
	mainBoxSizer = new wxBoxSizer(wxVERTICAL);

	dataBoxSizer = new wxBoxSizer(wxVERTICAL);

	headlineStaticText = new wxStaticText(mainPanel, -1, _("Update enterprise database"));
	updateStaticText = new wxStaticText(mainPanel, -1, _("Please make a backup before updating!"));
	headlineStaticLine = new wxStaticLine(mainPanel);

	headlineStaticText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	updateStaticText->SetFont(wxFont(10, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	updateStaticText->SetForegroundColour(wxColour(255, 0, 0));

	dataBoxSizer->Add(headlineStaticText);
	dataBoxSizer->AddSpacer(5);
	dataBoxSizer->Add(updateStaticText);
	dataBoxSizer->Add(headlineStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 15);

	versionBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	
	versionStaticText = new wxStaticText(mainPanel, -1, _("Actual version: ") + from);
	versionBoxSizer->Add(versionStaticText, 0, wxALIGN_CENTER_VERTICAL);

	versionLeftStaticLine = new wxStaticLine(mainPanel);
	versionBoxSizer->Add(versionLeftStaticLine, 1, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 15);

	updateButton = new wxButton(mainPanel, UPDATE_BUTTON_EVENT, _("Update to version"));
	versionBoxSizer->Add(updateButton, 0, wxALIGN_CENTER_VERTICAL);

	versionRightStaticLine = new wxStaticLine(mainPanel);
	versionBoxSizer->Add(versionRightStaticLine, 1, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 15);

	versionsComboBox = new wxComboBox(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxSize(100, -1), *versions, wxCB_READONLY);
	versionsComboBox->SetSelection(versionsComboBox->GetCount() - 1);
	versionBoxSizer->Add(versionsComboBox, 0, wxALIGN_CENTER_VERTICAL);

	dataBoxSizer->Add(versionBoxSizer, 0, wxEXPAND);

	progressStaticLine = new wxStaticLine(mainPanel);
	dataBoxSizer->Add(progressStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 15);

	progressStaticText = new wxStaticText(mainPanel, -1, _("Progress:"));
	progressGauge = new wxGauge(mainPanel, -1, 1);
	dataBoxSizer->Add(progressStaticText);
	dataBoxSizer->AddSpacer(5);
	dataBoxSizer->Add(progressGauge, 0, wxEXPAND | wxLEFT, 30);

	dataBoxSizer->AddSpacer(15);

	informationsStaticText = new wxStaticText(mainPanel, -1, _("Informations:"));
	informationsTextCtrl = new wxTextCtrl(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
	dataBoxSizer->Add(informationsStaticText);
	dataBoxSizer->AddSpacer(5);
	dataBoxSizer->Add(informationsTextCtrl, 1, wxEXPAND | wxLEFT, 30);

	mainBoxSizer->Add(dataBoxSizer, 1, wxEXPAND | wxALL, 15);

	mainPanel->SetSizer(mainBoxSizer);
	mainBoxSizer->SetSizeHints(this);
	
	SetSize(wxSize(640, 480));
}

void UpdateDialog::UpdateEvent(wxCommandEvent &event) {
	Update();
}

void UpdateDialog::Update() {
	const wxString to(versionsComboBox->GetValue());

	UpdateAskDialog *updateAskDialog = new UpdateAskDialog(this, -1, from, to);
	updateAskDialog->Center();
	const bool ok = updateAskDialog->ShowModal() == wxID_OK;
	updateAskDialog->Destroy();

	if(!ok)
		return;

	informationsTextCtrl->Clear();

	EnableControls(false);

	Log(_("Connecting to database..."));

	UpdateLibrary updateLibrary;
	DSPostgreSQLDatabase *db = updateLibrary.GetDatabaseObject(*enterpriseEntity);

	if(db) {
		Log(_("Connection established."));

		wxString version;
		const unsigned int count = versionsComboBox->GetCount();
		progressGauge->SetRange(count);
		progressGauge->SetValue(0);
		for(unsigned int i = 0; i < count; i++) {
			version = versionsComboBox->GetString(i);

			updateLibrary.Update(*enterpriseEntity, version, informationsTextCtrl);
			progressGauge->SetValue(i + 1);
			
			if(version == to)
				break;
		}

		Log(_("Updates done."));

		db->Disconnect();
		Log(_("Disconnected from database."));
		delete db;
	} else {
		Log(_("Could not connect to database."));
	}
}

void UpdateDialog::EnableControls(const bool enable) {
	versionsComboBox->Enable(enable);
	updateButton->Enable(enable);
}

void UpdateDialog::Log(const wxString &log) {
	informationsTextCtrl->AppendText(log + _N("\n"));
}
