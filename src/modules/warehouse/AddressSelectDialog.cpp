#include "AddressSelectDialog.h"
#include "../lib/AddressType.h"
#include "../lib/NameType.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(AddressSelectDialog, wxDialog)
	EVT_MENU(USE_EVENT, AddressSelectDialog::UseKeyEvent)
	EVT_MENU(CANCEL_EVENT, AddressSelectDialog::CancelKeyEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("addressesListCtrl"), AddressSelectDialog::UseSelectEvent)
END_EVENT_TABLE()

AddressSelectDialog::AddressSelectDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	AddressEntities &addressEntities
) : resultAddressEntity(
	NULL
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("AddressSelectDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("AddressSelectDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	useToolbarButton    = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	addressesListCtrl   = XRCCTRL(*this, "addressesListCtrl", DynSoft::DSListCtrl);

	addressesListCtrl->InsertColumn(0, _("Description"), wxLIST_FORMAT_LEFT, 150);
	addressesListCtrl->InsertColumn(1, _("Name"), wxLIST_FORMAT_LEFT, 150);
	addressesListCtrl->InsertColumn(2, _("Address"), wxLIST_FORMAT_LEFT, 150);
	addressesListCtrl->InsertColumn(3, _("Zipcode"), wxLIST_FORMAT_LEFT, 80);
	addressesListCtrl->InsertColumn(4, _("City"), wxLIST_FORMAT_LEFT, 120);
	addressesListCtrl->InsertColumn(5, _("Federal state"), wxLIST_FORMAT_LEFT, 120);
	addressesListCtrl->InsertColumn(6, _("Land"), wxLIST_FORMAT_LEFT, 120);

	AddressEntity *addressEntity = NULL;
	const unsigned int count = addressEntities.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		addressEntity = addressEntities.Get(i);

		DSListCtrl::DSListCtrlRow *addressesListCtrlRow = new DSListCtrl::DSListCtrlRow(addressEntity);
		addressesListCtrlRow->Add(i == 0 ? wxString(_("Main address")) : addressEntity->Get(_N("description"))->ToString());
		addressesListCtrlRow->Add(NameType(
			addressEntity->Get(_N("salutation"))->ToString(),
			addressEntity->Get(_N("title"))->ToString(),
			addressEntity->Get(_N("firstname"))->ToString(),
			addressEntity->Get(_N("name1"))->ToString(),
			addressEntity->Get(_N("name2"))->ToString(),
			addressEntity->Get(_N("name3"))->ToString(),
			addressEntity->Get(_N("name4"))->ToString()
		).GetConcatedName(_N(", ")));
			addressesListCtrlRow->Add(AddressType::GetAddressAndNumber(
			addressEntity->Get(_N("address"))->ToString(),
			addressEntity->Get(_N("number"))->ToString()
		));
		addressesListCtrlRow->Add(addressEntity->Get(_N("zipcode"))->ToString());
		addressesListCtrlRow->Add(addressEntity->Get(_N("city"))->ToString());
		addressesListCtrlRow->Add(addressEntity->Get(_N("federalState"))->ToString());
		addressesListCtrlRow->Add(addressEntity->Get(_N("land"))->ToString());

		addressesListCtrl->Set(addressesListCtrlRow, true);
	}

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(useToolbarButton, AddressSelectDialog::UseEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, AddressSelectDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'u',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(800, 500));

	ICON();
}

void AddressSelectDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void AddressSelectDialog::UseSelectEvent(wxListEvent &event) {
	Use();
}

void AddressSelectDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void AddressSelectDialog::Use() {
	resultAddressEntity = static_cast<AddressEntity *>(addressesListCtrl->GetSingleSelectionPointer());
	if(!resultAddressEntity) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the address you want to use."),
			_("Use")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		EndModal(wxID_OK);
	}
}

void AddressSelectDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void AddressSelectDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void AddressSelectDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

AddressEntity *AddressSelectDialog::GetSelectedAddress() const {
	return resultAddressEntity;
}
