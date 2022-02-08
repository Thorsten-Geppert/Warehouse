#include "NetworkAccessDialog.h"
#include "NetworkAccessEditDialog.h"
#include "../lib/PgHbaLibrary.h"
#include "../lib/PgHbaHostType.h"
#include <wx/filedlg.h>
#include <DSDebug.h>

BEGIN_EVENT_TABLE(NetworkAccessDialog, wxDialog)
	EVT_BUTTON(SAVE_EVENT, NetworkAccessDialog::SaveEvent)
	EVT_BUTTON(BROWSE_EVENT, NetworkAccessDialog::BrowseEvent)
	EVT_BUTTON(REMOVE_EVENT, NetworkAccessDialog::RemoveEvent)
	EVT_BUTTON(EDIT_EVENT, NetworkAccessDialog::EditEvent)
	EVT_BUTTON(ADD_EVENT, NetworkAccessDialog::AddEvent)
	EVT_LIST_ITEM_ACTIVATED(EDIT_LIST_EVENT, NetworkAccessDialog::EditListEvent)
END_EVENT_TABLE()

NetworkAccessDialog::~NetworkAccessDialog() {
	hostsListCtrl->CleanUp();
}

NetworkAccessDialog::NetworkAccessDialog(
	wxWindow *parent,
	wxWindowID id
) : wxDialog(
	parent,
	id,
	_("Network Access"),
	wxDefaultPosition,
	wxDefaultSize
) {
	mainPanel = new wxPanel(this, -1);
	mainBoxSizer = new wxBoxSizer(wxVERTICAL);

	browseBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	fileStaticText = new wxStaticText(mainPanel, -1, _("File:"));
	browseBoxSizer->Add(fileStaticText, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT, 5);

	fileTextCtrl = new wxTextCtrl(mainPanel, -1);
	browseBoxSizer->Add(fileTextCtrl, 1, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

	fileButton = new wxButton(mainPanel, BROWSE_EVENT, _("Browse..."));
	browseBoxSizer->Add(fileButton, 0);
	mainBoxSizer->Add(browseBoxSizer, 0, wxEXPAND | wxLEFT | wxTOP | wxRIGHT, 15);

	mainBoxSizer->AddSpacer(10);

	hostsListCtrl = new DSListCtrl(mainPanel, EDIT_LIST_EVENT, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_VRULES | wxBORDER_SUNKEN | wxLC_SINGLE_SEL);
	hostsListCtrl->InsertColumn(0, _("Host"), wxLIST_FORMAT_LEFT, 150);
	hostsListCtrl->InsertColumn(1, _("Database"), wxLIST_FORMAT_LEFT, 100);
	hostsListCtrl->InsertColumn(2, _("User"), wxLIST_FORMAT_LEFT, 150);
	hostsListCtrl->InsertColumn(3, _("Address"), wxLIST_FORMAT_LEFT, 100);
	hostsListCtrl->InsertColumn(4, _("Subnet"), wxLIST_FORMAT_LEFT, 100);
	hostsListCtrl->InsertColumn(5, _("Method"), wxLIST_FORMAT_LEFT, 100);

	mainBoxSizer->Add(hostsListCtrl, 1, wxEXPAND | wxLEFT | wxRIGHT, 15);

	mainBoxSizer->AddSpacer(10);

	listBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	removeHostButton = new wxButton(mainPanel, REMOVE_EVENT, _("Remove"));
	listBoxSizer->Add(removeHostButton, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
	listBoxSizer->AddStretchSpacer();
	editHostButton = new wxButton(mainPanel, EDIT_EVENT, _("Edit"));
	listBoxSizer->Add(editHostButton, 0, wxALIGN_CENTER_VERTICAL);
	addHostButton = new wxButton(mainPanel, ADD_EVENT, _("Add"));
	listBoxSizer->Add(addHostButton, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
	mainBoxSizer->Add(listBoxSizer, 0, wxEXPAND | wxLEFT | wxRIGHT, 15);

	mainBoxSizer->AddSpacer(15);

	optionsBoxSizer = new wxBoxSizer(wxHORIZONTAL);
	optionsBoxSizer->AddStretchSpacer();
	saveButton = new wxButton(mainPanel, SAVE_EVENT, _("Save"));
	optionsBoxSizer->Add(saveButton, 0, wxALIGN_CENTER_VERTICAL);
	mainBoxSizer->Add(optionsBoxSizer, 0, wxEXPAND | wxRIGHT | wxBOTTOM, 15);

	mainPanel->SetSizer(mainBoxSizer);

	mainBoxSizer->SetSizeHints(this);
	SetMinSize(wxSize(400, 300));
}

void NetworkAccessDialog::SaveEvent(wxCommandEvent &event) {
	Save(choosenFile);
}

void NetworkAccessDialog::BrowseEvent(wxCommandEvent &event) {
	Browse();
}

void NetworkAccessDialog::RemoveEvent(wxCommandEvent &event) {
	Remove();
}

void NetworkAccessDialog::EditEvent(wxCommandEvent &event) {
	Edit();
}

void NetworkAccessDialog::EditListEvent(wxListEvent &event) {
	Edit();
}

void NetworkAccessDialog::AddEvent(wxCommandEvent &event) {
	Add();
}

void NetworkAccessDialog::Save(const wxString &filename) {
	PgHbaLibrary pgHbaLibrary;
	wxString message;
	unsigned int error = pgHbaLibrary.WritePgHba(filename, lines, pgHbaHostsType);
	switch(error) {
		case PGHBA_ERROR_OK:
			message = _("Write: Everythings ok");
			break;
		case PGHBA_ERROR_FILE_NOT_OPENED:
			message = _("Write: Could not open file");
			break;
		case PGHBA_ERROR_FILE_NOT_WRITTEN:
			message = _("Write: Could not write file");
			break;
		case PGHBA_ERROR_FILE_NOT_CLOSED:
			message = _("Write: Could not close file");
			break;
	}

	if(error != PGHBA_ERROR_OK) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			message,
			_("Loading Hosts")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	}
}

void NetworkAccessDialog::Browse() {
	wxString defaultFile;
	wxString defaultDir;
	
#ifdef __WXMSW__ 
	defaultFile = _N("%PROGRAMFILES%\\PostgreSQL\\9.0\\data\\");
	defaultDir = _N("%PROGRAMFILES%\\PostgreSQL\\9.0\\data");
#elif __WXMAC__ 
	defaultFile = _N("/Library/PostgreSQL/9.0/data/");
	defaultDir = _N("/Library/PostgreSQL/9.0/data");
#elif __WXGTK__
	defaultFile = _N("/opt/PostgreSQL/9.0/data/");
	defaultDir = _N("/opt/PostgreSQL/9.0/data");
#endif
	defaultFile += _N("pg_hba.conf");
	
	wxFileDialog *file = new wxFileDialog(this, _("Choose the pg_hba.conf file"), defaultDir, defaultFile, _N("*.conf"), wxFD_DEFAULT_STYLE | wxFD_FILE_MUST_EXIST);
	file->Center();
	if(file->ShowModal() == wxID_OK) {
		choosenFile = file->GetPath();
		fileTextCtrl->SetValue(choosenFile);
		if(!choosenFile.IsEmpty())
			LoadHosts(choosenFile);
	} else {
		choosenFile = wxEmptyString;
	}
	file->Destroy();
}

void NetworkAccessDialog::Remove() {
	if(hostsListCtrl->GetSingleSelectionPosition() < 0) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to remove."),
			_("Error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	} else {
		PgHbaHostType *hostEntity = static_cast<PgHbaHostType *>(hostsListCtrl->GetSingleSelectionPointer());
		hostsListCtrl->DeleteItem(hostsListCtrl->GetSingleSelectionPosition());
		pgHbaHostsType.Remove(hostEntity, true);
		UpdateHosts();
	}
}

void NetworkAccessDialog::Edit() {
	PgHbaHostType *hostEntity = static_cast<PgHbaHostType *>(hostsListCtrl->GetSingleSelectionPointer());
	if(!hostEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to edit."),
			_("Error")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	} else {
		NetworkAccessEditDialog *networkAccessEditDialog = new NetworkAccessEditDialog(this, -1, hostEntity);
		networkAccessEditDialog->Center();
		if(networkAccessEditDialog->ShowModal() == wxID_OK)
			UpdateHosts();
		networkAccessEditDialog->Destroy();
	}
}

void NetworkAccessDialog::Add() {
	PgHbaHostType *hostEntity = new PgHbaHostType();
	NetworkAccessEditDialog *networkAccessEditDialog = new NetworkAccessEditDialog(this, -1, hostEntity);
	networkAccessEditDialog->Center();
	if(networkAccessEditDialog->ShowModal() == wxID_OK) {
		pgHbaHostsType.Add(hostEntity);
		
		AddInList(hostsListCtrl, hostEntity);
	}
	networkAccessEditDialog->Destroy();
}

unsigned int NetworkAccessDialog::UpdateHosts() {
	hostsListCtrl->DeleteAllItems();

	PgHbaHostType *hostEntity = NULL;

	const unsigned int count = pgHbaHostsType.GetCount();;
	for(unsigned int i = 0; i < count; i++) {
		hostEntity = pgHbaHostsType.Get(i);

		AddInList(hostsListCtrl, hostEntity);
	}
	
	return count;
}

unsigned int NetworkAccessDialog::LoadHosts(const wxString &filename) {
	hostsListCtrl->DeleteAllItems();

	PgHbaHostType *hostEntity = NULL;

	wxString message(_("No message defined"));

	PgHbaLibrary pgHbaLibrary;
	unsigned int error = pgHbaLibrary.ReadPgHba(filename, lines, pgHbaHostsType);
	switch(error) {
		case PGHBA_ERROR_OK:
			message = _("Read: Everythings ok");
			break;
		case PGHBA_ERROR_FILE_NOT_OPENED:
			message = _("Read: Could not open file");
			break;
		case PGHBA_ERROR_FILE_NOT_CLOSED:
			message = _("Read: Could not close file");
			break;
	}

	if(error != PGHBA_ERROR_OK) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			message,
			_("Loading Hosts")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	}

	const unsigned int count = pgHbaHostsType.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		hostEntity = pgHbaHostsType.Get(i);

		AddInList(hostsListCtrl, hostEntity);
	}

	return count;
}

void NetworkAccessDialog::AddInList(DSListCtrl *hostsListCtrl, PgHbaHostType *hostEntity) {
		DSListCtrl::DSListCtrlRow *hostListCtrlRow = new DSListCtrl::DSListCtrlRow(hostEntity);
		hostListCtrlRow->Add(hostEntity->GetType());
		hostListCtrlRow->Add(hostEntity->GetDatabase());
		hostListCtrlRow->Add(hostEntity->GetUser());
		hostListCtrlRow->Add(hostEntity->GetAddress());
		hostListCtrlRow->Add(hostEntity->GetSubnet());
		hostListCtrlRow->Add(hostEntity->GetMethod());

		hostsListCtrl->Set(hostListCtrlRow, true);
}

