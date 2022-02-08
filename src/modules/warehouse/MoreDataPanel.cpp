#include "MoreDataPanel.h"
#include <wx/xrc/xmlres.h>
#include "YesNoDialog.h"
#include "../lib/ListEntryController.h"

BEGIN_EVENT_TABLE(MoreDataPanel, wxPanel)
	EVT_BUTTON(XRCID("saveButton"), MoreDataPanel::SaveEvent)
	EVT_BUTTON(XRCID("abortButton"), MoreDataPanel::AbortEvent)
	EVT_BUTTON(XRCID("removeButton"), MoreDataPanel::RemoveEvent)
	EVT_TEXT_ENTER(XRCID("valueTextCtrl"), MoreDataPanel::SaveTextEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("dataListCtrl"), MoreDataPanel::EditEvent)
END_EVENT_TABLE()

MoreDataPanel::MoreDataPanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	KeysValuesType *keysValuesType,
	bool *changed
) : editingKeyValueType(
	NULL
) {
	this->rit = rit;
	this->keysValuesType = keysValuesType;
	this->changed = changed;
	this->editable = true;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("MoreDataPanel.xml")));
	wxXmlResource::Get()->LoadPanel(this, parent, _N("MoreDataPanel"));
	mainBoxSizer        = static_cast<wxBoxSizer *>(GetSizer());
	descriptionComboBox = XRCCTRL(*this, "descriptionComboBox", DynSoft::DSComboBox);
	valueTextCtrl       = XRCCTRL(*this, "valueTextCtrl", DynSoft::DSTextCtrl);
	saveButton          = XRCCTRL(*this, "saveButton", wxButton);
	abortButton         = XRCCTRL(*this, "abortButton", wxButton);
	dataListCtrl        = XRCCTRL(*this, "dataListCtrl", DynSoft::DSListCtrl);
	removeButton        = XRCCTRL(*this, "removeButton", wxButton);

	dataListCtrl->InsertColumn(0, _("Description"), wxLIST_FORMAT_LEFT, 200);
	dataListCtrl->InsertColumn(1, _("Value"), wxLIST_FORMAT_LEFT, 200);

	ListEntryController::FillComboBox(rit, descriptionComboBox, _N("descriptions"), _N("description"), ListEntryController::FILL_NEW, true);

	if(keysValuesType) {
		const unsigned int count = keysValuesType->GetCount();
		for(unsigned int i = 0; i < count; i++)
			AddOrChangeToDataListCtrl(keysValuesType->Get(i), true);
	}
}

MoreDataPanel::~MoreDataPanel() {
}

void MoreDataPanel::SaveEvent(wxCommandEvent &event) {
	Save();
}

void MoreDataPanel::SaveTextEvent(wxCommandEvent &event) {
	Save();
}

void MoreDataPanel::RemoveEvent(wxCommandEvent &event) {
	Remove();
}

void MoreDataPanel::EditEvent(wxListEvent &event) {
	Edit();
}

void MoreDataPanel::AbortEvent(wxCommandEvent &event) {
	Abort();
}

void MoreDataPanel::AddOrChangeToDataListCtrl(KeyValueType *keyValueType, const bool justAppend) {
	if(!keyValueType)
		return;

	if(!justAppend) {
		const unsigned int count = dataListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			if(static_cast<KeyValueType *>(dataListCtrl->GetPointer(i, 0)) == keyValueType)
				break;
		}
	}

	DSListCtrl::DSListCtrlRow *dataListCtrlRow = new DSListCtrl::DSListCtrlRow(keyValueType);
	dataListCtrlRow->Add(keyValueType->GetKey());
	dataListCtrlRow->Add(keyValueType->GetValue());

	dataListCtrl->Set(dataListCtrlRow, true);
}

void MoreDataPanel::Save() {
	if(!IsEditable()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("You cannot do changes anymore."),
			_("Closed")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	if(!rit->Access(A_WRITE, _("You have no permission to edit more data."), this))
		return;

	if(!editingKeyValueType) {
		KeyValueType *keyValueType = new KeyValueType(descriptionComboBox->GetValue(), valueTextCtrl->GetValue());
		if(keysValuesType->Exists(keyValueType->GetKey())) {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("An entry with this description allready exists. Could not add this entry."),
				_("Add")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
			delete keyValueType;
			return;
		}

		AddOrChangeToDataListCtrl(keyValueType, true);
		keysValuesType->Add(keyValueType);

		if(changed)
			*changed = true;
	} else {
		if(editingKeyValueType->GetKey() != descriptionComboBox->GetValue() && keysValuesType->Exists(descriptionComboBox->GetValue())) {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("An entry with this description allready exists. Could not add this entry."),
				_("Edit")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
			return;
		}

		editingKeyValueType->SetKey(descriptionComboBox->GetValue());
		editingKeyValueType->SetValue(valueTextCtrl->GetValue());
		AddOrChangeToDataListCtrl(editingKeyValueType, false);
		Abort();

		if(changed)
			*changed = true;
	}
}

void MoreDataPanel::Edit() {
	if(!IsEditable()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("You cannot do changes anymore."),
			_("Closed")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	if(!rit->Access(A_WRITE, _("You have no permission to edit more data."), this))
		return;

	editingKeyValueType = static_cast<KeyValueType *>(dataListCtrl->GetSingleSelectionPointer());

	descriptionComboBox->SetValue(editingKeyValueType->GetKey());
	valueTextCtrl->SetValue(editingKeyValueType->GetValue());

	removeButton->Disable();
	dataListCtrl->Disable();

	saveButton->SetLabel(_("Save"));
}

void MoreDataPanel::Abort() {
	descriptionComboBox->SetValue(wxEmptyString);
	valueTextCtrl->Clear();
	editingKeyValueType = NULL;
	removeButton->Enable();
	dataListCtrl->Enable();
	saveButton->SetLabel(_("Add"));
}

void MoreDataPanel::Remove() {
	if(!IsEditable()) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("You cannot do changes anymore."),
			_("Closed")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	if(!rit->Access(A_WRITE, _("You have no permission to remove more data."), this))
		return;

	KeyValueType *keyValueType = static_cast<KeyValueType *>(dataListCtrl->GetSingleSelectionPointer());
	if(!keyValueType) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to remove."),
			_("Remove")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to remove the selected entry?"),
		_("Remove"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		dataListCtrl->DeleteItem(dataListCtrl->GetPosition(keyValueType));
		keysValuesType->Remove(keyValueType, true);

		if(changed)
			*changed = true;
	}
	yesNoDialog->Destroy();
}

void MoreDataPanel::SetEditable(const bool editable) {
	this->editable = editable;

	descriptionComboBox->Enable(editable);
	valueTextCtrl->Enable(editable);
	saveButton->Enable(editable);
	abortButton->Enable(editable);
	removeButton->Enable(editable);
}

bool MoreDataPanel::IsEditable() const {
	return editable;
}
