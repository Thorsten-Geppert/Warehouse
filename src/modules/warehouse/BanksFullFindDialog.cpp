#include "BanksFullFindDialog.h"
#include "ProgressDialog.h"
#include "../lib/BankController.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(BanksFullFindDialog, wxDialog)
	EVT_LIST_ITEM_ACTIVATED(XRCID("banksListCtrl"), BanksFullFindDialog::UseDirectEvent)
	EVT_MENU(CLOSE_EVENT, BanksFullFindDialog::CancelKeyEvent)
	EVT_MENU(FIND_EVENT, BanksFullFindDialog::FindKeyEvent)
	EVT_MENU(USE_EVENT, BanksFullFindDialog::UseKeyEvent)
END_EVENT_TABLE()

BanksFullFindDialog::BanksFullFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : bankEntity(
	NULL
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("BanksFullFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("BanksFullFindDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton   = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	useToolbarButton    = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	bankTextCtrl        = XRCCTRL(*this, "bankTextCtrl", wxTextCtrl);
	bankCodeTextCtrl    = XRCCTRL(*this, "bankCodeTextCtrl", DynSoft::DSTextCtrl);
	bicTextCtrl         = XRCCTRL(*this, "bicTextCtrl", DynSoft::DSTextCtrl);
	banksListCtrl       = XRCCTRL(*this, "banksListCtrl", DynSoft::DSListCtrl);

	bankCodeTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	banksListCtrl->InsertColumn(0, _("Bank"), wxLIST_FORMAT_LEFT, 250);
	banksListCtrl->InsertColumn(1, _("Bank code"), wxLIST_FORMAT_RIGHT, 200);
	banksListCtrl->InsertColumn(2, _("BIC"), wxLIST_FORMAT_LEFT, 200);

	CONNECT_TOOLBARBUTTON(findToolbarButton, BanksFullFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(useToolbarButton, BanksFullFindDialog::UseEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, BanksFullFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'u',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(800, 480));

	ICON();
}

BanksFullFindDialog::~BanksFullFindDialog() {
	ClearBanksListCtrl();
}

int BanksFullFindDialog::ShowModal() {
	bankTextCtrl->SetFocus();
	return wxDialog::ShowModal();
}

void BanksFullFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void BanksFullFindDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void BanksFullFindDialog::UseDirectEvent(wxListEvent &event) {
	Use();
}

void BanksFullFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void BanksFullFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void BanksFullFindDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void BanksFullFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void BanksFullFindDialog::Find() {
	BankEntity bankEntity(rit);

	bankEntity.SetValue(_N("bank"), bankTextCtrl->GetValue());
	bankEntity.SetValue(_N("bankCode"), bankCodeTextCtrl->GetValue());
	bankEntity.SetValue(_N("bic"), bicTextCtrl->GetValue());

	ClearBanksListCtrl();

	BankController bankController(rit);
	bankController.Find(&bankEntity);

	BankEntity *tmpBankEntity = NULL;
	ProgressDialog *progressDialog = new ProgressDialog(this, -1, rit, bankController.GetCount());
	progressDialog->Center();
	progressDialog->Start();
	unsigned int i = 0;
	while(bankController.Run()) {
		tmpBankEntity = new BankEntity(rit);
		bankController.Get(tmpBankEntity);
		AddToBanksListCtrl(tmpBankEntity);
		progressDialog->SetValue(++i);
		if(progressDialog->IsCanceled()) {
			bankController.Reset();
			break;
		}
	}
	progressDialog->Stop();

	if(i > 0)
		banksListCtrl->SetFocus();
}

void BanksFullFindDialog::Use() {
	BankEntity *bankEntity = static_cast<BankEntity *>(banksListCtrl->GetSingleSelectionPointer());
	if(!bankEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a bank."),
			_("Use")
		);
		errorMessage->ShowModal();
	} else {
		this->bankEntity = bankEntity;
		EndModal(wxID_OK);
	}
}

void BanksFullFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

BankEntity *BanksFullFindDialog::GetBankEntity() const {
	return bankEntity;
}

void BanksFullFindDialog::ClearBanksListCtrl() {
	banksListCtrl->CleanUp();
}

void BanksFullFindDialog::AddToBanksListCtrl(BankEntity *bankEntity) {
	if(!bankEntity)
		return;

	DSListCtrl::DSListCtrlRow *banksListCtrlRow = new DSListCtrl::DSListCtrlRow(bankEntity);

	banksListCtrlRow->Add(bankEntity->Get(_N("bank"))->ToString());
	banksListCtrlRow->Add(bankEntity->Get(_N("bankCode"))->ToString());
	banksListCtrlRow->Add(bankEntity->Get(_N("bic"))->ToString());

	banksListCtrl->Set(banksListCtrlRow, true);
}
