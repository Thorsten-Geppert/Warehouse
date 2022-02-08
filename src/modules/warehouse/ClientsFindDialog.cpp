#include "ClientsFindDialog.h"
#include "../lib/ListEntryController.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ClientsFindDialog, wxDialog)
	EVT_MENU(FIND_EVENT, ClientsFindDialog::FindKeyEvent)
	EVT_MENU(CANCEL_EVENT, ClientsFindDialog::CancelKeyEvent)
END_EVENT_TABLE()

ClientsFindDialog::ClientsFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ClientsFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ClientsFindDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton   = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
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

	clientIdTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(findToolbarButton, ClientsFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ClientsFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling
	ListEntryController::FillComboBox(rit, titleComboBox,      _N("titles"),       _N("title"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, salutationComboBox, _N("salutations"),  _N("salutation"),   ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, landComboBox,       _N("lands"),        _N("land"),         ListEntryController::FILL_NEW, true);

	ICON();
}

int ClientsFindDialog::ShowModal() {
	clientIdTextCtrl->SetFocus();
	return wxDialog::ShowModal();
}

void ClientsFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void ClientsFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ClientsFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void ClientsFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ClientsFindDialog::Find() {
	EndModal(wxID_OK);
}

void ClientsFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

ClientEntity *ClientsFindDialog::GetClientEntity() const {
	ClientEntity *clientEntity = new ClientEntity(rit);

	clientEntity->SetValue(_N("clientId"), clientIdTextCtrl->GetValue());
	clientEntity->SetValue(_N("salutation"), salutationComboBox->GetValue());
	clientEntity->SetValue(_N("title"), titleComboBox->GetValue());
	clientEntity->SetValue(_N("firstname"), firstnameTextCtrl->GetValue());
	clientEntity->SetValue(_N("name1"), nameTextCtrl->GetValue());
	clientEntity->SetValue(_N("name2"), nameTextCtrl->GetValue());
	clientEntity->SetValue(_N("name3"), nameTextCtrl->GetValue());
	clientEntity->SetValue(_N("name4"), nameTextCtrl->GetValue());
	clientEntity->SetValue(_N("address"), addressTextCtrl->GetValue());
	clientEntity->SetValue(_N("city"), cityTextCtrl->GetValue());
	clientEntity->SetValue(_N("land"), landComboBox->GetValue());
	clientEntity->SetValue(_N("email1"), emailTextCtrl->GetValue());
	clientEntity->SetValue(_N("email2"), emailTextCtrl->GetValue());

	return clientEntity;
}
