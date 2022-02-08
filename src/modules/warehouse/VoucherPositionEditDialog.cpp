#include "VoucherPositionEditDialog.h"
#include "YesNoDialog.h"
#include "Configuration.h"
#include "../lib/CalculationLibrary.h"
#include "../lib/CommonPreferencesController.h"
#include "../lib/ConstantsLibrary.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(VoucherPositionEditDialog, wxDialog)
	EVT_CLOSE(VoucherPositionEditDialog::OnClose)
	EVT_MENU(CLOSE_EVENT, VoucherPositionEditDialog::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, VoucherPositionEditDialog::SaveKeyEvent)
END_EVENT_TABLE()

VoucherPositionEditDialog::VoucherPositionEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	VoucherPositionEntity *voucherPositionEntity,
	const bool writeable
) : bind(
	voucherPositionEntity
), changed(
	false
) {
	SetId(id);
	this->rit = rit;
	this->writeable = writeable;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("VoucherPositionEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("VoucherPositionEditDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton     = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	closeToolbarButton    = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	quantityTextCtrl      = XRCCTRL(*this, "quantityTextCtrl", DynSoft::DSTextCtrl);
	descriptionTextCtrl   = XRCCTRL(*this, "descriptionTextCtrl", DynSoft::DSTextCtrl);
	articleNumberTextCtrl = XRCCTRL(*this, "articleNumberTextCtrl", DynSoft::DSTextCtrl);
	articleTextCtrl       = XRCCTRL(*this, "articleTextCtrl", DynSoft::DSTextCtrl);
	taxComboBox           = XRCCTRL(*this, "taxComboBox", DynSoft::DSComboBox);
	taxPriceTextCtrl      = XRCCTRL(*this, "taxPriceTextCtrl", DynSoft::DSTextCtrl);
	priceTextCtrl         = XRCCTRL(*this, "priceTextCtrl", DynSoft::DSTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, VoucherPositionEditDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, VoucherPositionEditDialog::SaveEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling
	taxComboBox->Append(ConstantsLibrary::TaxKindToArrayString());
	taxComboBox->SetSelection(0);

	// Binding
	bind.Bind(_N("quantity"), quantityTextCtrl);
	bind.Bind(_N("description"), descriptionTextCtrl);
	bind.Bind(_N("articleNumber"), articleNumberTextCtrl);
	bind.Bind(_N("article"), articleTextCtrl);
	bind.Bind(_N("tax"), taxComboBox, DSBind::SELECTION);
	bind.Bind(_N("taxPrice"), taxPriceTextCtrl);
	bind.Bind(_N("price"), priceTextCtrl);

	if(writeable) {
		quantityTextCtrl->SetChange(&changed);
		descriptionTextCtrl->SetChange(&changed);
		articleNumberTextCtrl->SetChange(&changed);
		articleTextCtrl->SetChange(&changed);
		taxComboBox->SetChange(&changed);
		taxPriceTextCtrl->SetChange(&changed);
		priceTextCtrl->SetChange(&changed);
	} else {
		quantityTextCtrl->Disable();
		descriptionTextCtrl->Disable();
		articleNumberTextCtrl->Disable();
		articleTextCtrl->Disable();
		taxComboBox->Disable();
		taxPriceTextCtrl->Disable();
		priceTextCtrl->Disable();
		
		saveToolbarButton->SetEnabled(false);
	}

	quantityTextCtrl->SetFocus();

	if(voucherPositionEntity) {
		DSCurrency price;
		price = voucherPositionEntity->Get(_N("taxPrice"))->ToCurrency();
		voucherPositionEntity->SetValue(_N("taxPrice"), price);
		price = voucherPositionEntity->Get(_N("price"))->ToCurrency();
		voucherPositionEntity->SetValue(_N("price"), price);

		bind.Push();
	}

	quantityTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_NUMBER);
	taxPriceTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);
	priceTextCtrl->SetRegularExpression(DS_TEXTCTRL_REGULAR_EXPRESSION_CURRENCY);

	descriptionTextCtrl->SetChange(&changed);
	articleNumberTextCtrl->SetChange(&changed);
	articleTextCtrl->SetChange(&changed);
	taxComboBox->SetChange(&changed);
	taxPriceTextCtrl->SetChange(&changed);
	priceTextCtrl->SetChange(&changed);

	changed = false;

	ICON();
}

// Events
void VoucherPositionEditDialog::OnClose(wxCloseEvent &event) {
	bool ok = true;
	if(changed) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to save the changes?"),
			_("Close"),
			_("Save"),
			_("Cancel")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES)
			ok = Save();
		yesNoDialog->Destroy();
	}

	if(ok) {
		EndModal(wxID_CANCEL);
		event.Skip();
	} else {
		event.Veto();
	}
}

void VoucherPositionEditDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void VoucherPositionEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}


void VoucherPositionEditDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void VoucherPositionEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

// Methods
void VoucherPositionEditDialog::Close() {
	wxDialog::Close();
}

bool VoucherPositionEditDialog::Save() {
	if(!writeable) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not save data. Read only mode."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return false;
	} else {
		if(descriptionTextCtrl->GetValue().IsEmpty()) {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Please fill out all requested fields."),
				_("Save")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();

			return false;
		} else {
			bind.Pull();

			VoucherPositionEntity *voucherPositionEntity = static_cast<VoucherPositionEntity *>(bind.GetEntity());

			DSCurrency price;
			price = voucherPositionEntity->Get(_N("taxPrice"))->ToCurrency();
			voucherPositionEntity->SetValue(_N("taxPrice"), price);
			price = voucherPositionEntity->Get(_N("price"))->ToCurrency();
			voucherPositionEntity->SetValue(_N("price"), price);
			EndModal(wxID_OK);
		}
	}

	return true;
}
