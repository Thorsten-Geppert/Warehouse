#include "BugDialog.h"
#include <wx/xrc/xmlres.h>
#include "../lib/SendController.h"
#include "../lib/BugEntity.h"
#include "../lib/UserController.h"
#include "../lib/UserEntity.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(BugDialog, wxDialog)
	EVT_MENU(SEND_EVENT, BugDialog::SendKeyEvent)
	EVT_MENU(CANCEL_EVENT, BugDialog::CancelKeyEvent)
END_EVENT_TABLE()

BugDialog::BugDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : changed(
	false
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("BugDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("BugDialog"));

	mainPanel            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	sendToolbarButton    = XRCCTRL(*this, "sendToolbarButton", ToolbarButton);
	cancelToolbarButton  = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	yourNameTextCtrl     = XRCCTRL(*this, "yourNameTextCtrl", DynSoft::DSTextCtrl);
	emailTextCtrl        = XRCCTRL(*this, "emailTextCtrl", DynSoft::DSTextCtrl);
	bugTextCtrl          = XRCCTRL(*this, "bugTextCtrl", DynSoft::DSTextCtrl);
	moduleComboBox       = XRCCTRL(*this, "moduleComboBox", DynSoft::DSComboBox);
	priorityComboBox     = XRCCTRL(*this, "priorityComboBox", DynSoft::DSComboBox);
	descriptionTextCtrl  = XRCCTRL(*this, "descriptionTextCtrl", DynSoft::DSTextCtrl);

	SetSize(wxSize(640, 480));

	CONNECT_TOOLBARBUTTON(sendToolbarButton, BugDialog::SendEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, BugDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SEND_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	UserController userController(rit);
	userController.Get(rit->GetUserConfigurationFileEntity()->GetLastUsername(), true);
	if(userController.Run()) {
		UserEntity userEntity(rit->GetDatabase());
		userController.Get(&userEntity);
		
		yourNameTextCtrl->SetValue(userEntity.Get(_N("realName"))->ToString());
		emailTextCtrl->SetValue(userEntity.Get(_N("email"))->ToString());

		userController.Reset();
	}

	yourNameTextCtrl->SetChange(&changed);
	emailTextCtrl->SetChange(&changed);
	bugTextCtrl->SetChange(&changed);
	moduleComboBox->SetChange(&changed);
	priorityComboBox->SetChange(&changed);
	descriptionTextCtrl->SetChange(&changed);

	changed = false;

	yourNameTextCtrl->SetFocus();

	ICON();
}

void BugDialog::SendEvent(wxMouseEvent &event) {
	Send();
	SKIP();
}

void BugDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void BugDialog::SendKeyEvent(wxCommandEvent &event) {
	Send();
}

void BugDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void BugDialog::Send() {
	BugEntity bugEntity;
	bugEntity.SetValue(_N("software"), SOFTWARE);
	bugEntity.SetValue(_N("version"), VERSION);
	bugEntity.SetValue(_N("yourName"), yourNameTextCtrl->GetValue());
	bugEntity.SetValue(_N("email"), emailTextCtrl->GetValue());
	bugEntity.SetValue(_N("bug"), bugTextCtrl->GetValue());
	bugEntity.SetValue(_N("module"), moduleComboBox->GetValue());
	bugEntity.SetValue(_N("priority"), priorityComboBox->GetSelection());
	bugEntity.SetValue(_N("description"), descriptionTextCtrl->GetValue());

	if(
		bugEntity.Get(_N("yourName"))->ToString().IsEmpty() ||
		bugEntity.Get(_N("email"))->ToString().IsEmpty() ||
		bugEntity.Get(_N("bug"))->ToString().IsEmpty() ||
		bugEntity.Get(_N("module"))->ToString().IsEmpty() ||
		bugEntity.Get(_N("description"))->ToString().IsEmpty()
	) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please fill out all fields."),
			_("Send")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	}

	wxString message;
	SendController sendController;
	const unsigned int status = sendController.Send(CONTACT_URL, CONTACT_FILE, &bugEntity);
	switch(status) {
		case SendController::SEND:
			message = _("Message was send successfully. Thank you. We will reply as soon as possible.");
			break;
		case SendController::NO_CONNECTION:
			message = _("Could not connect to our server. Maybe your computer is not connected to the internet or our servers are down. Please try again later.");
			break;
		case SendController::ENTITY_IS_NULL:
		case SendController::UNKNOWN_ERROR:
		case SendController::INCORRECT_RESPONSE:
		case SendController::SERVER_ERROR:
		default:
			message = _("An unknown error has occured. Please try later or bug us directly. Please tell us the error: ") + DSCasts::ToString(status);
			break;
	}

	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		_("Bug")
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();

	if(status == SendController::SEND) {
		bugTextCtrl->Clear();
		moduleComboBox->SetSelection(0);
		priorityComboBox->SetSelection(1);
		descriptionTextCtrl->Clear();

		Close();
	}
}

void BugDialog::Cancel() {
	Close();
}
