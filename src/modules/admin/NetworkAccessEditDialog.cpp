#include "NetworkAccessEditDialog.h"

BEGIN_EVENT_TABLE(NetworkAccessEditDialog, wxDialog)
	EVT_BUTTON(SAVE_EVENT, NetworkAccessEditDialog::SaveEvent)
	EVT_BUTTON(ABORT_EVENT, NetworkAccessEditDialog::AbortEvent)
END_EVENT_TABLE()

NetworkAccessEditDialog::NetworkAccessEditDialog(
	wxWindow *parent,
	wxWindowID id,
	PgHbaHostType *hostEntity
) : wxDialog(
	parent,
	id,
	_("Network Access Edit"),
	wxDefaultPosition,
	wxDefaultSize
) {
	this->hostEntity = hostEntity;

	wxArrayString type;
	type.Add(_N("host"));
	type.Add(_N("hostssl"));

	wxArrayString database;
	database.Add(_N("all"));

	wxArrayString user;
	user.Add(_N("all"));

	wxArrayString address;
	address.Add(_N("192.168.0.0"));
	address.Add(_N("192.168.1.0"));
	address.Add(_N("192.168.178.0"));

	wxArrayString subnet;
	subnet.Add(_N("32"));
	subnet.Add(_N("24"));
	subnet.Add(_N("255.255.255.255"));
	subnet.Add(_N("255.255.255.0"));
	subnet.Add(_N("255.255.0.0"));
	subnet.Add(_N("255.0.0.0"));

	wxArrayString method;
	method.Add(_N("md5"));
	method.Add(_N("trust"));

	mainPanel = new wxPanel(this);
	mainBoxSizer = new wxBoxSizer(wxVERTICAL);

	dataFlexGridSizer = new wxFlexGridSizer(2, 5, 5);
	dataFlexGridSizer->AddGrowableCol(1);

	typeStaticText = new wxStaticText(mainPanel, -1, _("Type:"));
	dataFlexGridSizer->Add(typeStaticText, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);
	typeComboBox = new wxComboBox(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, type);
	dataFlexGridSizer->Add(typeComboBox, 0, wxEXPAND);

	databaseStaticText = new wxStaticText(mainPanel, -1, _("Database:"));
	dataFlexGridSizer->Add(databaseStaticText, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);
	databaseComboBox = new wxComboBox(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, database);
	dataFlexGridSizer->Add(databaseComboBox, 0, wxEXPAND);

	userStaticText = new wxStaticText(mainPanel, -1, _("User:"));
	dataFlexGridSizer->Add(userStaticText, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);
	userComboBox = new wxComboBox(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, user);
	dataFlexGridSizer->Add(userComboBox, 0, wxEXPAND);

	addressStaticText = new wxStaticText(mainPanel, -1, _("Address:"));
	dataFlexGridSizer->Add(addressStaticText, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);
	addresseComboBox = new wxComboBox(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, address);
	dataFlexGridSizer->Add(addresseComboBox, 0, wxEXPAND);

	subnetStaticText = new wxStaticText(mainPanel, -1, _("Subnet:"));
	dataFlexGridSizer->Add(subnetStaticText, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);
	subnetComboBox = new wxComboBox(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, subnet);
	dataFlexGridSizer->Add(subnetComboBox, 0, wxEXPAND);

	methodStaticText = new wxStaticText(mainPanel, -1, _("Method:"));
	dataFlexGridSizer->Add(methodStaticText, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);
	methodComboBox = new wxComboBox(mainPanel, -1, wxEmptyString, wxDefaultPosition, wxDefaultSize, method);
	dataFlexGridSizer->Add(methodComboBox, 0, wxEXPAND);

	mainBoxSizer->Add(dataFlexGridSizer, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 15);


	mainBoxSizer->AddSpacer(5);


	optionsBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	optionsBoxSizer->AddStretchSpacer();
	abortButton = new wxButton(mainPanel, ABORT_EVENT, _("Abort"));
	optionsBoxSizer->Add(abortButton, 0, wxRIGHT, 5);
	saveButton = new wxButton(mainPanel, SAVE_EVENT, _("Save"));
	optionsBoxSizer->Add(saveButton);
	mainBoxSizer->Add(optionsBoxSizer, 0, wxEXPAND | wxLEFT | wxBOTTOM | wxRIGHT, 15);


	mainPanel->SetSizer(mainBoxSizer);

	mainBoxSizer->SetSizeHints(this);
	//SetMinSize(wxSize(200, 300));

	LoadType();
}

void NetworkAccessEditDialog::SaveEvent(wxCommandEvent &devent) {
	Save();
}

void NetworkAccessEditDialog::AbortEvent(wxCommandEvent &devent) {
	Abort();
}

void NetworkAccessEditDialog::Save() {
	hostEntity->SetType(typeComboBox->GetValue());
	hostEntity->SetDatabase(databaseComboBox->GetValue());
	hostEntity->SetUser(userComboBox->GetValue());
	hostEntity->SetAddress(addresseComboBox->GetValue());
	hostEntity->SetSubnet(subnetComboBox->GetValue());
	hostEntity->SetMethod(methodComboBox->GetValue());

	EndModal(wxID_OK);
}

void NetworkAccessEditDialog::Abort() {
	EndModal(wxID_CANCEL);
}

void NetworkAccessEditDialog::LoadType() {
	typeComboBox->SetValue(hostEntity->GetType());
	databaseComboBox->SetValue(hostEntity->GetDatabase());
	userComboBox->SetValue(hostEntity->GetUser());
	addresseComboBox->SetValue(hostEntity->GetAddress());
	subnetComboBox->SetValue(hostEntity->GetSubnet());
	methodComboBox->SetValue(hostEntity->GetMethod());
}
