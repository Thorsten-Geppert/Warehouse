#include "AddressEditDialog.h"
#include "../lib/ListEntryController.h"
#include "TemplateSelectDialog.h"
#include "YesNoDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(AddressEditDialog, wxDialog)
	EVT_CLOSE(AddressEditDialog::OnClose)
	EVT_MENU(CLOSE_EVENT, AddressEditDialog::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, AddressEditDialog::SaveKeyEvent)
END_EVENT_TABLE()

AddressEditDialog::AddressEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	AddressEntity *addressEntity,
	const bool writeable
) : bind(
	addressEntity
), changed(
	false
) {
	SetId(id);
	this->rit = rit;
	this->writeable = writeable;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("AddressEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("AddressEditDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton     = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	closeToolbarButton    = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	descriptionComboBox   = XRCCTRL(*this, "descriptionComboBox", DynSoft::DSComboBox);
	salutationComboBox    = XRCCTRL(*this, "salutationComboBox", DynSoft::DSComboBox);
	titleComboBox         = XRCCTRL(*this, "titleComboBox", DynSoft::DSComboBox);
	firstnameTextCtrl     = XRCCTRL(*this, "firstnameTextCtrl", DynSoft::DSTextCtrl);
	name1TextCtrl         = XRCCTRL(*this, "name1TextCtrl", DynSoft::DSTextCtrl);
	name2TextCtrl         = XRCCTRL(*this, "name2TextCtrl", DynSoft::DSTextCtrl);
	addressTextCtrl       = XRCCTRL(*this, "addressTextCtrl", DynSoft::DSTextCtrl);
	numberTextCtrl        = XRCCTRL(*this, "numberTextCtrl", DynSoft::DSTextCtrl);
	zipcodeTextCtrl       = XRCCTRL(*this, "zipcodeTextCtrl", DynSoft::DSTextCtrl);
	cityTextCtrl          = XRCCTRL(*this, "cityTextCtrl", DynSoft::DSTextCtrl);
	federalStateComboBox  = XRCCTRL(*this, "federalStateComboBox", DynSoft::DSComboBox);
	landComboBox          = XRCCTRL(*this, "landComboBox", DynSoft::DSComboBox);
	name3TextCtrl         = XRCCTRL(*this, "name3TextCtrl", DynSoft::DSTextCtrl);
	name4TextCtrl         = XRCCTRL(*this, "name4TextCtrl", DynSoft::DSTextCtrl);
	informationStaticText = XRCCTRL(*this, "informationStaticText", wxStaticText);
	informationTextCtrl   = XRCCTRL(*this, "informationTextCtrl", DynSoft::DSTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, AddressEditDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, AddressEditDialog::SaveEvent);

	informationStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(AddressEditDialog::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Filling
	ListEntryController::FillComboBox(rit, descriptionComboBox,  _N("descriptions"), _N("description"),  ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, titleComboBox,        _N("titles"),       _N("title"),        ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, salutationComboBox,   _N("salutations"),  _N("salutation"),   ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, landComboBox,         _N("lands"),        _N("land"),         ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, federalStateComboBox, _N("federalstate"), _N("federalstate"), ListEntryController::FILL_NEW, true);

	// Binding
	bind.Bind(_N("description"), descriptionComboBox);
	bind.Bind(_N("salutation"), salutationComboBox, DSBind::VALUE);
	bind.Bind(_N("title"), titleComboBox, DSBind::VALUE);
	bind.Bind(_N("firstname"), firstnameTextCtrl);
	bind.Bind(_N("name1"), name1TextCtrl);
	bind.Bind(_N("name2"), name2TextCtrl);
	bind.Bind(_N("name3"), name3TextCtrl);
	bind.Bind(_N("name4"), name4TextCtrl);
	bind.Bind(_N("address"), addressTextCtrl);
	bind.Bind(_N("number"), numberTextCtrl);
	bind.Bind(_N("zipcode"), zipcodeTextCtrl);
	bind.Bind(_N("city"), cityTextCtrl);
	bind.Bind(_N("federalState"), federalStateComboBox, DSBind::VALUE);
	bind.Bind(_N("land"), landComboBox, DSBind::VALUE);
	bind.Bind(_N("information"), informationTextCtrl);

	if(writeable) {
		descriptionComboBox->SetChange(&changed);
		salutationComboBox->SetChange(&changed);
		titleComboBox->SetChange(&changed);
		firstnameTextCtrl->SetChange(&changed);
		name1TextCtrl->SetChange(&changed);
		name2TextCtrl->SetChange(&changed);
		addressTextCtrl->SetChange(&changed);
		numberTextCtrl->SetChange(&changed);
		zipcodeTextCtrl->SetChange(&changed);
		cityTextCtrl->SetChange(&changed);
		federalStateComboBox->SetChange(&changed);
		landComboBox->SetChange(&changed);
		name3TextCtrl->SetChange(&changed);
		name4TextCtrl->SetChange(&changed);
		informationTextCtrl->SetChange(&changed);
	} else {
		descriptionComboBox->Disable();
		salutationComboBox->Disable();
		titleComboBox->Disable();
		firstnameTextCtrl->Disable();
		name1TextCtrl->Disable();
		name2TextCtrl->Disable();
		addressTextCtrl->Disable();
		numberTextCtrl->Disable();
		zipcodeTextCtrl->Disable();
		cityTextCtrl->Disable();
		federalStateComboBox->Disable();
		landComboBox->Disable();
		name3TextCtrl->Disable();
		name4TextCtrl->Disable();
		informationTextCtrl->Disable();

		saveToolbarButton->SetEnabled(false);
	}

	descriptionComboBox->SetFocus();

	if(addressEntity)
		bind.Push();

	changed = false;

	ICON();
}

// Events
void AddressEditDialog::OnClose(wxCloseEvent &event) {
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

void AddressEditDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void AddressEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}


void AddressEditDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void AddressEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void AddressEditDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

// Methods
void AddressEditDialog::Close() {
	wxDialog::Close();
}

bool AddressEditDialog::Save() {
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
		if(name1TextCtrl->GetValue().IsEmpty()) {
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

void AddressEditDialog::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == informationStaticText)
		textCtrl = informationTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}
