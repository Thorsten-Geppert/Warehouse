#include "MonitionsTextsDialog.h"
#include "TemplateSelectDialog.h"
#include "Configuration.h"
#include "../lib/CommonPreferencesController.h"
#include "../lib/MonitionTextController.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(MonitionsTextsDialog, wxDialog)
	EVT_MENU(SAVE_EVENT, MonitionsTextsDialog::SaveKeyEvent)
	EVT_MENU(CANCEL_EVENT, MonitionsTextsDialog::CancelKeyEvent)
	EVT_LISTBOX(XRCID("statesListBox"), MonitionsTextsDialog::StateSelectEvent)
	EVT_TEXT(XRCID("prependTextTextCtrl"), MonitionsTextsDialog::TextEvent)
	EVT_TEXT(XRCID("appendTextTextCtrl"), MonitionsTextsDialog::TextEvent)
END_EVENT_TABLE()

MonitionsTextsDialog::MonitionsTextsDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;
	this->monitionTextEntity = NULL;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("MonitionsTextsDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("MonitionsTextsDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton     = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	cancelToolbarButton   = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	statesListBox         = XRCCTRL(*this, "statesListBox", wxListBox);
	prependTextStaticText = XRCCTRL(*this, "prependTextStaticText", wxStaticText);
	prependTextTextCtrl   = XRCCTRL(*this, "prependTextTextCtrl", wxTextCtrl);
	appendTextStaticText  = XRCCTRL(*this, "appendTextStaticText", wxStaticText);
	appendTextTextCtrl    = XRCCTRL(*this, "appendTextTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(800, 600));

	CONNECT_TOOLBARBUTTON(saveToolbarButton, MonitionsTextsDialog::SaveEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, MonitionsTextsDialog::CancelEvent);
	prependTextStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(MonitionsTextsDialog::TemplateSelectEvent), NULL, this);
	appendTextStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(MonitionsTextsDialog::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Fill stuff
	MonitionTextController monitionTextController(rit);
	const int states = CommonPreferencesController::GetMonitionStates(rit, 3);
	for(int number = 1; number <= states; number++) {
		statesListBox->Append(
			_("Monition text ") + DSCasts::ToString(number),
			monitionTextController.GetDirect(number, true)
		);
	}

	ICON();

	StateSelect(NULL);
}

MonitionsTextsDialog::~MonitionsTextsDialog() {
	CleanUp();
}

void MonitionsTextsDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void MonitionsTextsDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void MonitionsTextsDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void MonitionsTextsDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void MonitionsTextsDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

void MonitionsTextsDialog::StateSelectEvent(wxCommandEvent &event) {
	StateSelect(static_cast<MonitionTextEntity *>(event.GetClientData()));
}

void MonitionsTextsDialog::TextEvent(wxCommandEvent &event) {
	Text(static_cast<wxTextCtrl *>(event.GetEventObject()));
}

void MonitionsTextsDialog::Save() {
	if(!rit->Access(A_WRITE, _("You have no permission to change monition texts."), this))
		return;

	MonitionTextEntity *monitionTextEntity = NULL;
	MonitionTextController monitionTextController(rit);
	
	bool close = true;
	bool ok = false;
	const unsigned int count = statesListBox->GetCount();
	for(unsigned int i = 0; i < count; i++) {
		monitionTextEntity = static_cast<MonitionTextEntity *>(statesListBox->GetClientData(i));

		if(!monitionTextEntity) {
			rit->Log(true, _N("MonitionsTextsDialog"), _("Could not save number ") + DSCasts::ToString(i + 1) + _N(". Referencing error."));
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not save number ") + DSCasts::ToString(i + 1) + _N(". Referencing error."),
				_("Error")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		} else {
			ok = false;
			if(monitionTextController.Exists(_N("number"), monitionTextEntity->Get(_N("number"))->ToString()))
				ok = monitionTextController.Update(monitionTextEntity);
			else
				ok = monitionTextController.Insert(monitionTextEntity, false);
			if(!ok) {
				rit->Log(true, _N("MonitionsTextsDialog"), _("Could not save number ") + DSCasts::ToString(i + 1) + _N(". Database error."));
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not save number ") + DSCasts::ToString(i + 1) + _N(". Database error."),
					_("Error")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();

				close = false;
			} else {
				rit->Log(false, _N("MonitionsTextsDialog"), _("Saved number ") + DSCasts::ToString(i + 1) + _N("."));
			}
		}
	}

	if(close)
		Cancel();
}

void MonitionsTextsDialog::Cancel() {
	Close();
}

void MonitionsTextsDialog::TemplateSelect(wxObject *object) {
	if(!rit->Access(A_WRITE, _("You have no permission to change monition texts."), this))
		return;

	wxTextCtrl *textCtrl = NULL;

	if(object == prependTextStaticText)
		textCtrl = prependTextTextCtrl;
	else if(object == appendTextStaticText)
		textCtrl = appendTextTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}

void MonitionsTextsDialog::CleanUp() {
	const unsigned int count = statesListBox->GetCount();
	for(unsigned int i = 0; i < count; i++)
		delete static_cast<MonitionTextEntity *>(statesListBox->GetClientData(i));
	statesListBox->Clear();
}

void MonitionsTextsDialog::StateSelect(MonitionTextEntity *monitionTextEntity) {
	fireEvent = false;

	this->monitionTextEntity = monitionTextEntity;

	if(!monitionTextEntity) {
		prependTextTextCtrl->Disable();
		prependTextStaticText->Disable();
		appendTextTextCtrl->Disable();
		appendTextStaticText->Disable();

		prependTextTextCtrl->Clear();
		appendTextTextCtrl->Clear();
	} else {
		prependTextTextCtrl->Enable();
		prependTextStaticText->Enable();
		appendTextTextCtrl->Enable();
		appendTextStaticText->Enable();

		prependTextTextCtrl->SetValue(monitionTextEntity->Get(_N("prepend"))->ToString());
		appendTextTextCtrl->SetValue(monitionTextEntity->Get(_N("append"))->ToString());
	}

	fireEvent = true;
}

void MonitionsTextsDialog::Text(wxTextCtrl *textCtrl) {
	if(!rit->Access(A_WRITE, _("You have no permission to change monition texts."), this))
		return;

	if(!fireEvent)
		return;

	if(!monitionTextEntity) {
		ErrorText();
	} else {
		wxString entity;
		if(textCtrl == prependTextTextCtrl)
			entity = _N("prepend");
		else if(textCtrl == appendTextTextCtrl)
			entity = _N("append");

		if(!entity.IsEmpty())
			monitionTextEntity->SetValue(entity, textCtrl->GetValue());
	}
}

void MonitionsTextsDialog::ErrorText() {
	wxMessageDialog *errorMessage = new wxMessageDialog(
		this,
		_("Please select the monition state you want to edit."),
		_("Error")
	);
	errorMessage->Center();
	errorMessage->ShowModal();
	errorMessage->Destroy();
}
