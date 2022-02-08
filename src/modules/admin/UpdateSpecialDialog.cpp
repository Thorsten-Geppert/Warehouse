#include "UpdateSpecialDialog.h"
#include <wx/filedlg.h>
#include "../lib/UpdateLibrary.h"
#include "UpdateAskDialog.h"

BEGIN_EVENT_TABLE(UpdateSpecialDialog, wxDialog)
	EVT_BUTTON(FILE_CHOOSE_BUTTON, UpdateSpecialDialog::FileChooseEvent)
	EVT_BUTTON(UPDATE_BUTTON, UpdateSpecialDialog::UpdateEvent)
END_EVENT_TABLE()

UpdateSpecialDialog::UpdateSpecialDialog(
	wxWindow *parent,
	wxWindowID id,
	EnterpriseEntity *enterpriseEntity,
	const wxString &version
) : wxDialog(
	parent,
	id,
	_("Update Special"),
	wxDefaultPosition,
	wxDefaultSize
), sqlUpdateType(
	NULL
) {
	this->enterpriseEntity = enterpriseEntity;
	this->version          = version;

	mainPanel = new wxPanel(this, -1);
	mainBoxSizer = new wxBoxSizer(wxVERTICAL);

	dataBoxSizer = new wxBoxSizer(wxVERTICAL);

	headlineStaticText = new wxStaticText(mainPanel, -1, _("Update Special"));
	headlineStaticLine = new wxStaticLine(mainPanel);

	headlineStaticText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));

	dataBoxSizer->Add(headlineStaticText);
	dataBoxSizer->AddSpacer(5);
	dataBoxSizer->Add(headlineStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 15);

	fileBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	filenameStaticText = new wxStaticText(mainPanel, -1, _("Filename:"));
	filenameTextCtrl = new wxTextCtrl(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
	fileChooseButton = new wxButton(mainPanel, FILE_CHOOSE_BUTTON, _("Choose update file"));

	fileBoxSizer->Add(filenameStaticText, 0, wxALIGN_CENTER_VERTICAL);
	fileBoxSizer->AddSpacer(5);
	fileBoxSizer->Add(filenameTextCtrl, 1, wxALIGN_CENTER_VERTICAL);
	fileBoxSizer->AddSpacer(5);
	fileBoxSizer->Add(fileChooseButton, 0, wxALIGN_CENTER_VERTICAL);

	dataBoxSizer->Add(fileBoxSizer, 0, wxEXPAND);

	fileStaticLine = new wxStaticLine(mainPanel);

	dataBoxSizer->AddSpacer(5);
	dataBoxSizer->Add(fileStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 15);

	logTextCtrl = new wxTextCtrl(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxTE_MULTILINE);
	dataBoxSizer->Add(logTextCtrl, 1, wxEXPAND | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 15);

	logStaticLine = new wxStaticLine(mainPanel);
	dataBoxSizer->Add(logStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 15);

	updateButton = new wxButton(mainPanel, UPDATE_BUTTON, _("- Update - Please create a backup before -"));
	updateButton->Disable();
	dataBoxSizer->Add(updateButton, 0, wxEXPAND | wxTOP | wxBOTTOM | wxALIGN_CENTER_VERTICAL, 15);

	mainBoxSizer->Add(dataBoxSizer, 1, wxEXPAND | wxALL, 15);

	mainPanel->SetSizer(mainBoxSizer);
	mainBoxSizer->SetSizeHints(this);
	
	SetSize(wxSize(800, 600));
}

UpdateSpecialDialog::~UpdateSpecialDialog() {
	delete sqlUpdateType;
}

void UpdateSpecialDialog::FileChooseEvent(wxCommandEvent &event) {
	FileChoose();
}

void UpdateSpecialDialog::UpdateEvent(wxCommandEvent &event) {
	Update();
}

void UpdateSpecialDialog::FileChoose() {
	wxFileDialog *fileDialog = new wxFileDialog(
		this,
		_("Choose the update file"),
		path,
		wxEmptyString,
		_N("*.whu"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);
	fileDialog->Center();
	if(fileDialog->ShowModal() == wxID_OK) {
		path = fileDialog->GetDirectory();
		filenameTextCtrl->SetValue(fileDialog->GetPath());

		UpdateLibrary updateLibrary;
		if(sqlUpdateType)
			delete sqlUpdateType;
		sqlUpdateType = updateLibrary.ParseWarehouseUpdateFile(fileDialog->GetPath());
		SetInformations(sqlUpdateType);
	}
	fileDialog->Destroy();
}

void UpdateSpecialDialog::Update() {
	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		_("Do you really want to perform the update? Did you do a backup before?"),
		_("Update"),
		wxYES_NO
	);
	messageDialog->Center();
	if(messageDialog->ShowModal() == wxID_YES) {
		UpdateLibrary updateLibrary;

		logTextCtrl->AppendText(_N("\n\n=========================================================\n\n"));
		logTextCtrl->AppendText(_("Starting ...\n"));
		logTextCtrl->AppendText(wxString(_("Result: ")) + (updateLibrary.Query(*enterpriseEntity, sqlUpdateType->GetSql()) ? _("SUCCESS") : _("ERROR")) + _N("\n"));
		logTextCtrl->AppendText(_("Finished."));
		logTextCtrl->AppendText(_N("\n\n========================================================="));

		updateButton->Disable();
	}
	messageDialog->Destroy();
}

void UpdateSpecialDialog::SetInformations(SqlUpdateType *sqlUpdateType) {
	logTextCtrl->Clear();

	if(sqlUpdateType && sqlUpdateType->ExistsVersion(version)) {
		logTextCtrl->AppendText(_("Version(s):") + _N("\n\n") + sqlUpdateType->GetVersions() + _N("\n\n\n"));
		logTextCtrl->AppendText(_("Description:") + _N("\n\n") + sqlUpdateType->GetDescription());

		updateButton->Enable();
	} else if(sqlUpdateType && !sqlUpdateType->ExistsVersion(version)) {
		logTextCtrl->AppendText(_("The update is not for your database version"));
		updateButton->Disable();
	} else {
		logTextCtrl->AppendText(_("Could not read the file or the file is corrupted or invalid."));
		updateButton->Disable();
	}
}
