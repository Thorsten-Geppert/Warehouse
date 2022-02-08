#include "TranslatedAddressEditDialog.h"
#include "../lib/Tools.h"
#include "../lib/AddressEntity.h"
#include "../lib/AddressEntities.h"
#include "../lib/ClientController.h"
#include "../lib/AddressType.h"
#include "../lib/NameType.h"
#include "AddressSelectDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(TranslatedAddressEditDialog, wxDialog)
	EVT_MENU(USE_EVENT, TranslatedAddressEditDialog::UseKeyEvent)
	EVT_MENU(FIND_EVENT, TranslatedAddressEditDialog::FindKeyEvent)
	EVT_MENU(CANCEL_EVENT, TranslatedAddressEditDialog::CancelKeyEvent)
END_EVENT_TABLE()

TranslatedAddressEditDialog::TranslatedAddressEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	wxString *address,
	const unsigned int clientId,
	const bool readOnly
) {
	SetId(id);
	this->rit = rit;
	this->address = address;
	this->clientId = clientId;
	this->readOnly = readOnly;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("TranslatedAddressEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("TranslatedAddressEditDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	useToolbarButton    = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	findToolbarButton   = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	addressTextCtrl     = XRCCTRL(*this, "addressTextCtrl", wxTextCtrl);
	addressTextCtrl->SetValue(*address);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(useToolbarButton, TranslatedAddressEditDialog::UseEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, TranslatedAddressEditDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, TranslatedAddressEditDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'u',  USE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,   (int) 'f',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(320, 260));

	if(readOnly) {
		addressTextCtrl->Disable();
		useToolbarButton->SetEnabled(false);
		findToolbarButton->SetEnabled(false);
	}

	ICON();
}

void TranslatedAddressEditDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void TranslatedAddressEditDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void TranslatedAddressEditDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void TranslatedAddressEditDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void TranslatedAddressEditDialog::Use() {
	if(readOnly)
		return;

	if(address) {
		const wxString tmp(addressTextCtrl->GetValue());
		if(Tools::IsReallyEmpty(tmp))
			address->Clear();
		else
			*address = tmp;
	}
	EndModal(wxID_OK);
}

void TranslatedAddressEditDialog::Find() {
	if(readOnly)
		return;

	AddressEntity *addressEntity = NULL;
	ClientController clientController(rit);
	AddressEntities *addressEntities = clientController.GetAddresses(clientId);
	AddressSelectDialog *addressSelectDialog = new AddressSelectDialog(
		this,
		-1,
		rit,
		*addressEntities
	);
	addressSelectDialog->Center();
	if(addressSelectDialog->ShowModal() == wxID_OK) {
		addressEntity = addressSelectDialog->GetSelectedAddress();

		addressTextCtrl->SetValue(
			NameType(
				addressEntity->Get(_N("salutation"))->ToString(),
				addressEntity->Get(_N("title"))->ToString(),
				addressEntity->Get(_N("firstname"))->ToString(),
				addressEntity->Get(_N("name1"))->ToString(),
				addressEntity->Get(_N("name2"))->ToString(),
				addressEntity->Get(_N("name3"))->ToString(),
				addressEntity->Get(_N("name4"))->ToString()
			).GetConcatedFullName(_N("\n")) +
			_N("\n") +
			AddressType(
				addressEntity->Get(_N("address"))->ToString(),
				addressEntity->Get(_N("number"))->ToString(),
				addressEntity->Get(_N("zipcode"))->ToString(),
				addressEntity->Get(_N("city"))->ToString(),
				addressEntity->Get(_N("federalState"))->ToString(),
				addressEntity->Get(_N("land"))->ToString()
			).GetConcatedAddress(_N("\n"))
		);
	}

	addressSelectDialog->Destroy();

	addressEntities->Clear(true);
	delete addressEntities;
}

void TranslatedAddressEditDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void TranslatedAddressEditDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void TranslatedAddressEditDialog::Cancel() {
	EndModal(wxID_CANCEL);
}
