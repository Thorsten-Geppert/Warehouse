#include "ContactDialog.h"
#include <wx/xrc/xmlres.h>
#include "../lib/SendController.h"
#include "../lib/ContactEntity.h"
#include "../lib/UserController.h"
#include "../lib/UserEntity.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ContactDialog, wxDialog)
	EVT_MENU(SEND_EVENT, ContactDialog::SendKeyEvent)
	EVT_MENU(CANCEL_EVENT, ContactDialog::CancelKeyEvent)
END_EVENT_TABLE()

ContactDialog::ContactDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : changed(
	false
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ContactDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ContactDialog"));

	mainPanel            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	sendToolbarButton    = XRCCTRL(*this, "sendToolbarButton", ToolbarButton);
	cancelToolbarButton  = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	yourNameTextCtrl     = XRCCTRL(*this, "yourNameTextCtrl", DynSoft::DSTextCtrl);
	emailTextCtrl        = XRCCTRL(*this, "emailTextCtrl", DynSoft::DSTextCtrl);
	subjectTextCtrl      = XRCCTRL(*this, "subjectTextCtrl", DynSoft::DSTextCtrl);
	messageTextCtrl      = XRCCTRL(*this, "messageTextCtrl", DynSoft::DSTextCtrl);

	SetSize(wxSize(640, 480));

	CONNECT_TOOLBARBUTTON(sendToolbarButton, ContactDialog::SendEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ContactDialog::CancelEvent);

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
	subjectTextCtrl->SetChange(&changed);
	messageTextCtrl->SetChange(&changed);

	changed = false;

	yourNameTextCtrl->SetFocus();

	ICON();
}

void ContactDialog::SendEvent(wxMouseEvent &event) {
	Send();
	SKIP();
}

void ContactDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ContactDialog::SendKeyEvent(wxCommandEvent &event) {
	Send();
}

void ContactDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ContactDialog::Send() {
	ContactEntity contactEntity;
	contactEntity.SetValue(_N("software"), SOFTWARE);
	contactEntity.SetValue(_N("version"), VERSION);
	contactEntity.SetValue(_N("yourName"), yourNameTextCtrl->GetValue());
	contactEntity.SetValue(_N("email"), emailTextCtrl->GetValue());
	contactEntity.SetValue(_N("subject"), subjectTextCtrl->GetValue());
	contactEntity.SetValue(_N("message"), messageTextCtrl->GetValue());

	if(
		contactEntity.Get(_N("yourName"))->ToString().IsEmpty() ||
		contactEntity.Get(_N("email"))->ToString().IsEmpty() ||
		contactEntity.Get(_N("subject"))->ToString().IsEmpty() ||
		contactEntity.Get(_N("message"))->ToString().IsEmpty()
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
	const unsigned int status = sendController.Send(CONTACT_URL, CONTACT_FILE, &contactEntity);
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
			message = _("An unknown error has occured. Please try later or contact us directly. Please tell us the error: ") + DSCasts::ToString(status);
			break;
	}

	wxMessageDialog *messageDialog = new wxMessageDialog(
		this,
		message,
		_("Contact")
	);
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();

	if(status == SendController::SEND) {
		subjectTextCtrl->Clear();
		messageTextCtrl->Clear();

		Close();
	}
}

void ContactDialog::Cancel() {
	Close();
}
