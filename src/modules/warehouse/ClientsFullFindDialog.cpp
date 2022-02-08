#include "ClientsFullFindDialog.h"
#include "ProgressDialog.h"
#include "../lib/ClientController.h"
#include "../lib/AddressType.h"
#include "../lib/PhoneType.h"
#include "../lib/ListEntryController.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ClientsFullFindDialog, wxDialog)
	EVT_LIST_ITEM_ACTIVATED(XRCID("clientsListCtrl"), ClientsFullFindDialog::UseDirectEvent)
	EVT_MENU(CLOSE_EVENT, ClientsFullFindDialog::CancelKeyEvent)
	EVT_MENU(FIND_EVENT, ClientsFullFindDialog::FindKeyEvent)
	EVT_MENU(USE_EVENT, ClientsFullFindDialog::UseKeyEvent)
END_EVENT_TABLE()

ClientsFullFindDialog::ClientsFullFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const int kind
) : clientId(
	0
) {
	SetId(id);
	this->rit = rit;
	this->kind = kind;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ClientsFullFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ClientsFullFindDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton   = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	useToolbarButton    = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	clientIdTextCtrl    = XRCCTRL(*this, "clientIdTextCtrl", DynSoft::DSTextCtrl);
	salutationComboBox  = XRCCTRL(*this, "salutationComboBox", wxComboBox);
	titleComboBox       = XRCCTRL(*this, "titleComboBox", wxComboBox);
	firstnameTextCtrl   = XRCCTRL(*this, "firstnameTextCtrl", wxTextCtrl);
	nameTextCtrl        = XRCCTRL(*this, "nameTextCtrl", wxTextCtrl);
	addressTextCtrl     = XRCCTRL(*this, "addressTextCtrl", wxTextCtrl);
	zipcodeTextCtrl     = XRCCTRL(*this, "zipcodeTextCtrl", wxTextCtrl);
	cityTextCtrl        = XRCCTRL(*this, "cityTextCtrl", wxTextCtrl);
	landComboBox        = XRCCTRL(*this, "landComboBox", wxComboBox);
	emailTextCtrl       = XRCCTRL(*this, "emailTextCtrl", wxTextCtrl);
	clientsListCtrl     = XRCCTRL(*this, "clientsListCtrl", DynSoft::DSListCtrl);

	clientIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	clientsListCtrl->InsertColumn(0, _("Customer id"), wxLIST_FORMAT_RIGHT, 90);
	clientsListCtrl->InsertColumn(1, _("Own customer id"), wxLIST_FORMAT_RIGHT, 90);
	clientsListCtrl->InsertColumn(2, _("Name"), wxLIST_FORMAT_LEFT, 200);
	clientsListCtrl->InsertColumn(3, _("Firstname"), wxLIST_FORMAT_LEFT, 200);
	clientsListCtrl->InsertColumn(4, _("Address"), wxLIST_FORMAT_LEFT, 200);
	clientsListCtrl->InsertColumn(5, _("Zipcode"), wxLIST_FORMAT_LEFT, 70);
	clientsListCtrl->InsertColumn(6, _("City"), wxLIST_FORMAT_LEFT, 150);
	clientsListCtrl->InsertColumn(7, _("Land"), wxLIST_FORMAT_LEFT, 150);
	clientsListCtrl->InsertColumn(8, _("Telephone"), wxLIST_FORMAT_LEFT, 150);
	clientsListCtrl->InsertColumn(9, _("Email"), wxLIST_FORMAT_LEFT, 160);

	CONNECT_TOOLBARBUTTON(findToolbarButton, ClientsFullFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(useToolbarButton, ClientsFullFindDialog::UseEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ClientsFullFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'u',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling
	ListEntryController::FillComboBox(rit, titleComboBox,        _N("titles"),       _N("title"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, salutationComboBox,   _N("salutations"),  _N("salutation"),   ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, landComboBox,         _N("lands"),        _N("land"),         ListEntryController::FILL_NEW, true);

	SetSize(wxSize(1000, -1));

	ICON();
}

int ClientsFullFindDialog::ShowModal() {
	clientIdTextCtrl->SetFocus();
	return wxDialog::ShowModal();
}

void ClientsFullFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void ClientsFullFindDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void ClientsFullFindDialog::UseDirectEvent(wxListEvent &event) {
	Use();
}

void ClientsFullFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ClientsFullFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void ClientsFullFindDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void ClientsFullFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ClientsFullFindDialog::Find() {
	ClientEntity clientEntity(rit);

	clientEntity.SetValue(_N("clientId"), clientIdTextCtrl->GetValue());
	clientEntity.SetValue(_N("ownClientId"), clientIdTextCtrl->GetValue());
	clientEntity.SetValue(_N("salutation"), salutationComboBox->GetValue());
	clientEntity.SetValue(_N("title"), titleComboBox->GetValue());
	clientEntity.SetValue(_N("firstname"), firstnameTextCtrl->GetValue());
	clientEntity.SetValue(_N("name1"), nameTextCtrl->GetValue());
	clientEntity.SetValue(_N("name2"), nameTextCtrl->GetValue());
	clientEntity.SetValue(_N("name3"), nameTextCtrl->GetValue());
	clientEntity.SetValue(_N("name4"), nameTextCtrl->GetValue());
	clientEntity.SetValue(_N("address"), addressTextCtrl->GetValue());
	clientEntity.SetValue(_N("city"), cityTextCtrl->GetValue());
	clientEntity.SetValue(_N("land"), landComboBox->GetValue());
	clientEntity.SetValue(_N("email1"), emailTextCtrl->GetValue());
	clientEntity.SetValue(_N("email2"), emailTextCtrl->GetValue());

	ClearClientsListCtrl();

	ClientController clientController(rit);
	clientController.Find(&clientEntity, true);
	ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, clientController.GetCount());
	progressDialog->Center();
	progressDialog->Start();
	unsigned int i = 0;
	while(clientController.Run()) {
		clientController.Get(&clientEntity);
		AddToClientsListCtrl(&clientEntity);
		progressDialog->SetValue(++i);
		if(progressDialog->IsCanceled()) {
			clientController.Reset();
			break;
		}
	}
	progressDialog->Stop();

	if(i > 0)
		clientsListCtrl->SetFocus();
}

void ClientsFullFindDialog::Use() {
	unsigned int *clientId = static_cast<unsigned int *>(clientsListCtrl->GetSingleSelectionPointer());
	if(!clientId) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a client."),
			_("Use")
		);
		errorMessage->ShowModal();
	} else {
		this->clientId = *clientId;
		EndModal(wxID_OK);
	}
}

void ClientsFullFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

ClientEntity *ClientsFullFindDialog::GetClientEntity() const {
	if(clientId == 0)
		return NULL;

	ClientEntity *clientEntity = NULL;
	ClientController clientController(rit);
	clientController.Get(DSCasts::ToString(clientId));
	if(clientController.Run()) {
		clientEntity = new ClientEntity(rit);
		clientController.Get(clientEntity);
		clientController.Reset();
	}

	return clientEntity;
}

void ClientsFullFindDialog::ClearClientsListCtrl() {
	clientsListCtrl->CleanUp();
}

void ClientsFullFindDialog::AddToClientsListCtrl(ClientEntity *clientEntity) {
	if(!clientEntity)
		return;
	
	const wxString realKind(clientEntity->Get(_N("kind"))->ToString());
	bool ok = kind == KIND_ALL;
	if(kind == KIND_CLIENT && (realKind == _N("Kunde") || realKind == _N("Kunde/Lieferant")))
		ok = true;
	else if(kind == KIND_DISTRIBUTOR && (realKind == _N("Lieferant") || realKind == _N("Kunde/Lieferant")))
		ok = true;
	else if(kind == KIND_CLIENT_AND_DISTRIBUTOR && realKind == _N("Kunde/Lieferant"))
		ok = true;

	if(ok) {
		DSListCtrl::DSListCtrlRow *clientsListCtrlRow = new DSListCtrl::DSListCtrlRow(new unsigned int(clientEntity->Get(_N("clientId"))->ToUnsignedInt()));
		clientsListCtrlRow->SetBackgroundColour(wxColour(clientEntity->Get(_N("formOfOrganization"))->ToUnsignedInt() == 0 ? wxColour(165, 255, 150) : wxColour(255, 177, 150)));

		clientsListCtrlRow->Add(clientEntity->Get(_N("clientId"))->ToString());
		clientsListCtrlRow->Add(clientEntity->Get(_N("ownClientId"))->ToString());
		clientsListCtrlRow->Add(clientEntity->Get(_N("name1"))->ToString());
		clientsListCtrlRow->Add(clientEntity->Get(_N("firstname"))->ToString());
		clientsListCtrlRow->Add(AddressType::GetAddressAndNumber(
			clientEntity->Get(_N("address"))->ToString(),
			clientEntity->Get(_N("number"))->ToString()
		));
		clientsListCtrlRow->Add(clientEntity->Get(_N("zipcode"))->ToString());
		clientsListCtrlRow->Add(clientEntity->Get(_N("city"))->ToString());
		clientsListCtrlRow->Add(clientEntity->Get(_N("land"))->ToString());
		clientsListCtrlRow->Add(PhoneType(
			clientEntity->Get(_N("telephone1Country"))->ToString(),
			clientEntity->Get(_N("telephone1Prefix"))->ToString(),
			clientEntity->Get(_N("telephone1Number"))->ToString()
		).ToString());
		clientsListCtrlRow->Add(clientEntity->Get(_N("email1"))->ToString());

		clientsListCtrl->Set(clientsListCtrlRow, true);
	}
}
