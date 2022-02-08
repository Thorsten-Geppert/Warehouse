#include "BankConnectionEditDialog.h"
#include "BanksFullFindDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(BankConnectionEditDialog, wxDialog)
	EVT_CLOSE(BankConnectionEditDialog::OnClose)
	EVT_MENU(SAVE_EVENT, BankConnectionEditDialog::SaveKeyEvent)
	EVT_MENU(CANCEL_EVENT, BankConnectionEditDialog::CloseKeyEvent)
END_EVENT_TABLE()

BankConnectionEditDialog::BankConnectionEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	BankConnectionEntity *bankConnectionEntity
) {
	SetId(id);
	this->rit                  = rit;
	this->bankConnectionEntity = bankConnectionEntity;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("BankConnectionEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("BankConnectionEditDialog"));

	mainPanel                   = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton           = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	findToolbarButton           = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	closeToolbarButton          = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	descriptionTextCtrl         = XRCCTRL(*this, "descriptionTextCtrl", DynSoft::DSTextCtrl);
	bankTextCtrl                = XRCCTRL(*this, "bankTextCtrl", DynSoft::DSTextCtrl);
	bankCodeTextCtrl            = XRCCTRL(*this, "bankCodeTextCtrl", DynSoft::DSTextCtrl);
	accountNumberTextCtrl       = XRCCTRL(*this, "accountNumberTextCtrl", DynSoft::DSTextCtrl);
	bicTextCtrl                 = XRCCTRL(*this, "bicTextCtrl", DynSoft::DSTextCtrl);
	ibanTextCtrl                = XRCCTRL(*this, "ibanTextCtrl", DynSoft::DSTextCtrl);
	bankOwnerTextCtrl           = XRCCTRL(*this, "bankOwnerTextCtrl", DynSoft::DSTextCtrl);
	referenceNumberTextCtrl     = XRCCTRL(*this, "referenceNumberTextCtrl", DynSoft::DSTextCtrl);
	referenceDateDatePickerCtrl = XRCCTRL(*this, "referenceDateDatePickerCtrl", DynSoft::DSDatePickerCtrl);
	referenceStateComboBox      = XRCCTRL(*this, "referenceStateComboBox", DynSoft::DSComboBox);
	referenceAccountComboBox    = XRCCTRL(*this, "referenceAccountComboBox", DynSoft::DSComboBox);
	notesTextCtrl               = XRCCTRL(*this, "notesTextCtrl", DynSoft::DSTextCtrl);
	activeCheckBox              = XRCCTRL(*this, "activeCheckBox", DynSoft::DSCheckBox);

	bankCodeTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	accountNumberTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, BankConnectionEditDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, BankConnectionEditDialog::SaveEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, BankConnectionEditDialog::FindEvent);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(350, -1));

	if(bankConnectionEntity) {
		descriptionTextCtrl->SetValue(bankConnectionEntity->Get(_N("description"))->ToString());
		bankTextCtrl->SetValue(bankConnectionEntity->Get(_N("bank"))->ToString());
		bankCodeTextCtrl->SetValue(bankConnectionEntity->Get(_N("bankCode"))->ToString());
		accountNumberTextCtrl->SetValue(bankConnectionEntity->Get(_N("accountNumber"))->ToString());
		bicTextCtrl->SetValue(bankConnectionEntity->Get(_N("bic"))->ToString());
		ibanTextCtrl->SetValue(bankConnectionEntity->Get(_N("iban"))->ToString());
		bankOwnerTextCtrl->SetValue(bankConnectionEntity->Get(_N("bankOwner"))->ToString());
		referenceNumberTextCtrl->SetValue(bankConnectionEntity->Get(_N("referenceNumber"))->ToString());
		referenceDateDatePickerCtrl->SetValue(bankConnectionEntity->GetReferenceDate());
		referenceStateComboBox->SetSelection(bankConnectionEntity->Get(_N("referenceState"))->ToUnsignedInt());
		referenceAccountComboBox->SetSelection(bankConnectionEntity->Get(_N("referenceAccount"))->ToUnsignedInt());
		notesTextCtrl->SetValue(bankConnectionEntity->Get(_N("notes"))->ToString());
		activeCheckBox->SetValue(bankConnectionEntity->Get(_N("active"))->ToBool());
	}

	descriptionTextCtrl->SetFocus();

	ICON();
}

// Events
void BankConnectionEditDialog::OnClose(wxCloseEvent &event) {
	//Destroy();
	event.Skip();
}

void BankConnectionEditDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void BankConnectionEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void BankConnectionEditDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void BankConnectionEditDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void BankConnectionEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void BankConnectionEditDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

// Methods
void BankConnectionEditDialog::Close() {
	wxWindow::Close();
}

void BankConnectionEditDialog::Save() {
	if(
		descriptionTextCtrl->GetValue().IsEmpty() ||
		bankTextCtrl->GetValue().IsEmpty() ||
		bankOwnerTextCtrl->GetValue().IsEmpty() ||

		(
			(
				bankCodeTextCtrl->GetValue().IsEmpty() ||
				accountNumberTextCtrl->GetValue().IsEmpty()
			) && (
				bicTextCtrl->GetValue().IsEmpty() ||
				ibanTextCtrl->GetValue().IsEmpty()
			)
		)
	) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please fill in all fields. Choose if you want to enter account number and bank code or bic and iban."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	bankConnectionEntity->SetValue(_N("description"), descriptionTextCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("bank"), bankTextCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("bankCode"), bankCodeTextCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("accountNumber"), accountNumberTextCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("bic"), bicTextCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("iban"), ibanTextCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("bankOwner"), bankOwnerTextCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("referenceNumber"), referenceNumberTextCtrl->GetValue());
	bankConnectionEntity->SetReferenceDate(referenceDateDatePickerCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("referenceState"), referenceStateComboBox->GetSelection());
	bankConnectionEntity->SetValue(_N("referenceAccount"), referenceAccountComboBox->GetSelection());
	bankConnectionEntity->SetValue(_N("notes"), notesTextCtrl->GetValue());
	bankConnectionEntity->SetValue(_N("active"), activeCheckBox->GetValue());

	EndModal(wxID_OK);
}

void BankConnectionEditDialog::Find() {
	BanksFullFindDialog *banksFullFindDialog = new BanksFullFindDialog(
		this,
		-1,
		rit
	);
	banksFullFindDialog->Center();

	if(banksFullFindDialog->ShowModal() == wxID_OK) {
		BankEntity *bankEntity = banksFullFindDialog->GetBankEntity(); // Will be destroyed automatically
		if(bankEntity) {
			bankTextCtrl->SetValue(bankEntity->Get(_N("bank"))->ToString());
			bankCodeTextCtrl->SetValue(bankEntity->Get(_N("bankCode"))->ToString());
			bicTextCtrl->SetValue(bankEntity->Get(_N("bic"))->ToString());
			accountNumberTextCtrl->SetFocus();
		}
	}

	banksFullFindDialog->Destroy();
}
