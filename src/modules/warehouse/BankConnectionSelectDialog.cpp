#include "BankConnectionSelectDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(BankConnectionSelectDialog, wxDialog)
	EVT_MENU(USE_EVENT, BankConnectionSelectDialog::UseKeyEvent)
	EVT_MENU(CANCEL_EVENT, BankConnectionSelectDialog::CancelKeyEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("bankConnectionsListCtrl"), BankConnectionSelectDialog::UseSelectEvent)
END_EVENT_TABLE()

BankConnectionSelectDialog::BankConnectionSelectDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	BankConnectionEntities &bankConnectionEntities
) : bankConnectionEntity(
	NULL
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("BankConnectionSelectDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("BankConnectionSelectDialog"));

	mainPanel               = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer            = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	useToolbarButton        = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	cancelToolbarButton     = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	bankConnectionsListCtrl = XRCCTRL(*this, "bankConnectionsListCtrl", DynSoft::DSListCtrl);

	bankConnectionsListCtrl->InsertColumn(0, _("Description"), wxLIST_FORMAT_LEFT, 150);
	bankConnectionsListCtrl->InsertColumn(1, _("Bank"), wxLIST_FORMAT_LEFT, 200);
	bankConnectionsListCtrl->InsertColumn(2, _("Bank code"), wxLIST_FORMAT_RIGHT, 150);
	bankConnectionsListCtrl->InsertColumn(3, _("Account number"), wxLIST_FORMAT_RIGHT, 110);
	bankConnectionsListCtrl->InsertColumn(4, _("Owner"), wxLIST_FORMAT_LEFT, 150);
	bankConnectionsListCtrl->InsertColumn(5, _("BIC"), wxLIST_FORMAT_LEFT, 150);
	bankConnectionsListCtrl->InsertColumn(6, _("IBAN"), wxLIST_FORMAT_LEFT, 150);
	bankConnectionsListCtrl->InsertColumn(7, _("Reference number"), wxLIST_FORMAT_LEFT, 150);
	bankConnectionsListCtrl->InsertColumn(8, _("Date"), wxLIST_FORMAT_LEFT, 150);
	bankConnectionsListCtrl->InsertColumn(9, _("State"), wxLIST_FORMAT_LEFT, 150);
	bankConnectionsListCtrl->InsertColumn(10, _("Account"), wxLIST_FORMAT_LEFT, 150);
	bankConnectionsListCtrl->InsertColumn(11, _("Notes"), wxLIST_FORMAT_LEFT, 250);

	BankConnectionEntity *bankConnectionEntity = NULL;
	const unsigned int count = bankConnectionEntities.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		bankConnectionEntity = bankConnectionEntities.Get(i);

		if(bankConnectionEntity->Get(_N("active"))->ToBool()) {
			DSListCtrl::DSListCtrlRow *bankConnectionsListCtrlRow = new DSListCtrl::DSListCtrlRow(bankConnectionEntity);
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("description"))->ToString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("bank"))->ToString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("bankCode"))->ToString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("accountNumber"))->ToString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("bankOwner"))->ToString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("bic"))->ToString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("iban"))->ToString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->Get(_N("referenceNumber"))->ToString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->GetReferenceDate().ToGermanDate());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->GetReferenceStateString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->GetReferenceAccountString());
			bankConnectionsListCtrlRow->Add(bankConnectionEntity->GetNotesInOneLine());

			bankConnectionsListCtrl->Set(bankConnectionsListCtrlRow, true);
		}
	}

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(useToolbarButton, BankConnectionSelectDialog::UseEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, BankConnectionSelectDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'u',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(800, 300));

	ICON();
}

void BankConnectionSelectDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void BankConnectionSelectDialog::UseSelectEvent(wxListEvent &event) {
	Use();
}

void BankConnectionSelectDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void BankConnectionSelectDialog::Use() {
	bankConnectionEntity = static_cast<BankConnectionEntity *>(bankConnectionsListCtrl->GetSingleSelectionPointer());
	if(!bankConnectionEntity) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the bank you want to use."),
			_("Use")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		EndModal(wxID_OK);
	}
}

void BankConnectionSelectDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void BankConnectionSelectDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void BankConnectionSelectDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

BankConnectionEntity *BankConnectionSelectDialog::GetSelectedBankConnection() const {
	return bankConnectionEntity;
}
