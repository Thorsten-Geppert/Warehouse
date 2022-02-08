#include "ContactPersonEditDialog.h"
#include "../lib/ListEntryController.h"
#include "TemplateSelectDialog.h"
#include "YesNoDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ContactPersonEditDialog, wxDialog)
	EVT_CLOSE(ContactPersonEditDialog::OnClose)
	EVT_MENU(CLOSE_EVENT, ContactPersonEditDialog::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, ContactPersonEditDialog::SaveKeyEvent)
END_EVENT_TABLE()

ContactPersonEditDialog::ContactPersonEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	ContactPersonEntity *contactPersonEntity,
	const bool writeable
) : bind(
	contactPersonEntity
), changed(
	false
) {
	SetId(id);
	this->rit = rit;
	this->writeable = writeable;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ContactPersonEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ContactPersonEditDialog"));

	mainPanel                = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer             = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton        = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	closeToolbarButton       = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	functionComboBox         = XRCCTRL(*this, "functionComboBox", DynSoft::DSComboBox);
	departmentComboBox       = XRCCTRL(*this, "departmentComboBox", DynSoft::DSComboBox);
	salutationComboBox       = XRCCTRL(*this, "salutationComboBox", DynSoft::DSComboBox);
	titleComboBox            = XRCCTRL(*this, "titleComboBox", DynSoft::DSComboBox);
	firstnameTextCtrl        = XRCCTRL(*this, "firstnameTextCtrl", DynSoft::DSTextCtrl);
	lastnameTextCtrl         = XRCCTRL(*this, "lastnameTextCtrl", DynSoft::DSTextCtrl);
	informationStaticText    = XRCCTRL(*this, "informationStaticText", wxStaticText);
	informationTextCtrl      = XRCCTRL(*this, "informationTextCtrl", DynSoft::DSTextCtrl);
	telephoneCountryTextCtrl = XRCCTRL(*this, "telephoneCountryTextCtrl", DynSoft::DSTextCtrl);
	telephonePrefixTextCtrl  = XRCCTRL(*this, "telephonePrefixTextCtrl", DynSoft::DSTextCtrl);
	telephoneNumberTextCtrl  = XRCCTRL(*this, "telephoneNumberTextCtrl", DynSoft::DSTextCtrl);
	mobileCountryTextCtrl    = XRCCTRL(*this, "mobileCountryTextCtrl", DynSoft::DSTextCtrl);
	mobilePrefixTextCtrl     = XRCCTRL(*this, "mobilePrefixTextCtrl", DynSoft::DSTextCtrl);
	mobileNumberTextCtrl     = XRCCTRL(*this, "mobileNumberTextCtrl", DynSoft::DSTextCtrl);
	faxCountryTextCtrl       = XRCCTRL(*this, "faxCountryTextCtrl", DynSoft::DSTextCtrl);
	faxPrefixTextCtrl        = XRCCTRL(*this, "faxPrefixTextCtrl", DynSoft::DSTextCtrl);
	faxNumberTextCtrl        = XRCCTRL(*this, "faxNumberTextCtrl", DynSoft::DSTextCtrl);
	emailTextCtrl            = XRCCTRL(*this, "emailTextCtrl", DynSoft::DSTextCtrl);
	internetTextCtrl         = XRCCTRL(*this, "internetTextCtrl", DynSoft::DSTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, ContactPersonEditDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, ContactPersonEditDialog::SaveEvent);

	informationStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ContactPersonEditDialog::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling
	ListEntryController::FillComboBox(rit, departmentComboBox, _N("departments"), _N("department"), ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, functionComboBox,   _N("functions"),   _N("function"),   ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, titleComboBox,      _N("titles"),      _N("title"),      ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, salutationComboBox, _N("salutations"), _N("salutation"), ListEntryController::FILL_NEW, true);

	// Binding
	bind.Bind(_N("function"), functionComboBox);
	bind.Bind(_N("department"), departmentComboBox);
	bind.Bind(_N("salutation"), salutationComboBox, DSBind::VALUE);
	bind.Bind(_N("title"), titleComboBox, DSBind::VALUE);
	bind.Bind(_N("firstname"), firstnameTextCtrl);
	bind.Bind(_N("lastname"), lastnameTextCtrl);
	bind.Bind(_N("information"), informationTextCtrl);
	bind.Bind(_N("telephoneCountry"), telephoneCountryTextCtrl);
	bind.Bind(_N("telephonePrefix"), telephonePrefixTextCtrl);
	bind.Bind(_N("telephoneNumber"), telephoneNumberTextCtrl);
	bind.Bind(_N("mobileCountry"), mobileCountryTextCtrl);
	bind.Bind(_N("mobilePrefix"), mobilePrefixTextCtrl);
	bind.Bind(_N("mobileNumber"), mobileNumberTextCtrl);
	bind.Bind(_N("faxCountry"), faxCountryTextCtrl);
	bind.Bind(_N("faxPrefix"), faxPrefixTextCtrl);
	bind.Bind(_N("faxNumber"), faxNumberTextCtrl);
	bind.Bind(_N("email"), emailTextCtrl);
	bind.Bind(_N("internet"), internetTextCtrl);

	if(writeable) {
		functionComboBox->SetChange(&changed);
		departmentComboBox->SetChange(&changed);
		salutationComboBox->SetChange(&changed);
		titleComboBox->SetChange(&changed);
		firstnameTextCtrl->SetChange(&changed);
		lastnameTextCtrl->SetChange(&changed);
		informationTextCtrl->SetChange(&changed);
		telephoneCountryTextCtrl->SetChange(&changed);
		telephonePrefixTextCtrl->SetChange(&changed);
		telephoneNumberTextCtrl->SetChange(&changed);
		mobileCountryTextCtrl->SetChange(&changed);
		mobilePrefixTextCtrl->SetChange(&changed);
		mobileNumberTextCtrl->SetChange(&changed);
		faxCountryTextCtrl->SetChange(&changed);
		faxPrefixTextCtrl->SetChange(&changed);
		faxNumberTextCtrl->SetChange(&changed);
		emailTextCtrl->SetChange(&changed);
		internetTextCtrl->SetChange(&changed);
	} else {
		functionComboBox->Disable();
		departmentComboBox->Disable();
		salutationComboBox->Disable();
		titleComboBox->Disable();
		firstnameTextCtrl->Disable();
		lastnameTextCtrl->Disable();
		informationTextCtrl->Disable();
		telephoneCountryTextCtrl->Disable();
		telephonePrefixTextCtrl->Disable();
		telephoneNumberTextCtrl->Disable();
		mobileCountryTextCtrl->Disable();
		mobilePrefixTextCtrl->Disable();
		mobileNumberTextCtrl->Disable();
		faxCountryTextCtrl->Disable();
		faxPrefixTextCtrl->Disable();
		faxNumberTextCtrl->Disable();
		emailTextCtrl->Disable();
		internetTextCtrl->Disable();

		saveToolbarButton->SetEnabled(false);
	}

	functionComboBox->SetFocus();

	if(contactPersonEntity)
		bind.Push();

	changed = false;

	ICON();
}

// Events
void ContactPersonEditDialog::OnClose(wxCloseEvent &event) {
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

void ContactPersonEditDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void ContactPersonEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}


void ContactPersonEditDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void ContactPersonEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void ContactPersonEditDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

// Methods
void ContactPersonEditDialog::Close() {
	wxDialog::Close();
}

bool ContactPersonEditDialog::Save() {
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
		if(lastnameTextCtrl->GetValue().IsEmpty()) {
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
			EndModal(wxID_OK);
		}
	}

	return true;
}

void ContactPersonEditDialog::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == informationStaticText)
		textCtrl = informationTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}
