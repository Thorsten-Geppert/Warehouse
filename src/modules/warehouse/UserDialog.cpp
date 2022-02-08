#include <DSDebug.h>
#include "UserDialog.h"
#include "Configuration.h"
#include "TemplateSelectDialog.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(UserDialog, wxDialog)
	EVT_MENU(SAVE_EVENT, UserDialog::SaveKeyEvent)
	EVT_MENU(CANCEL_EVENT, UserDialog::CancelKeyEvent)
END_EVENT_TABLE()

UserDialog::UserDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	UserEntity *userEntity
) {
	SetId(id);
	this->rit        = rit;
	this->userEntity = userEntity;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("UserDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("UserDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar           = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton   = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	saveToolbarButton     = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	usernameTextCtrl      = XRCCTRL(*this, "usernameTextCtrl", wxTextCtrl);
	passwordTextCtrl      = XRCCTRL(*this, "passwordTextCtrl", wxTextCtrl);
	realNameTextCtrl      = XRCCTRL(*this, "realNameTextCtrl", wxTextCtrl);
	emailTextCtrl         = XRCCTRL(*this, "emailTextCtrl", wxTextCtrl);
	descriptionStaticText = XRCCTRL(*this, "descriptionStaticText", wxStaticText);
	descriptionTextCtrl   = XRCCTRL(*this, "descriptionTextCtrl", wxTextCtrl);
	statusComboBox        = XRCCTRL(*this, "statusComboBox", wxComboBox);
	remindCheckBox        = XRCCTRL(*this, "remindCheckBox", wxCheckBox);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, UserDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, UserDialog::SaveEvent);

	descriptionStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(UserDialog::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	if(userEntity) {
		usernameTextCtrl->SetValue(userEntity->Get(_N("username"))->ToString());
		realNameTextCtrl->SetValue(userEntity->Get(_N("realName"))->ToString());
		emailTextCtrl->SetValue(userEntity->Get(_N("email"))->ToString());
		descriptionTextCtrl->SetValue(userEntity->Get(_N("description"))->ToString());
		statusComboBox->SetSelection(userEntity->Get(_N("status"))->ToUnsignedInt());
		remindCheckBox->SetValue(userEntity->Get(_N("openToRemind"))->ToBool());
		usernameTextCtrl->Disable();
		passwordTextCtrl->SetFocus();
	} else {
		usernameTextCtrl->SetFocus();
	}

	ICON();
}

void UserDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void UserDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void UserDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void UserDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void UserDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

void UserDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void UserDialog::Save() {
	if(!rit->Access(A_ADMINISTRATOR, _("You have no permission to save this user."), this))
		return;

	const wxString username(usernameTextCtrl->GetValue());
	if(username.IsEmpty()) {
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Please fill out the username"),
			_("Save")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		return;
	}

	UserEntity *userEntity = new UserEntity(rit->GetDatabase());
	UserController userController(rit);
	if(userController.Exists(
		userEntity->GetPrimaryKey(),
		username,
		this->userEntity ? this->userEntity->Get(_N("username"))->ToString() : wxString()
	)) {
		rit->Log(true, _N("UserDialog"), _("The user ") + username + _(" is not saved."), _("A user with this username already exists. Please choose another username."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("A user with this username already exists. Please choose another username."),
			_("Save")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();

		delete userEntity;
		return;
	}

	const wxString password(passwordTextCtrl->GetValue());
	userEntity->Set(_N("password"), _N("MD5('") + password + _N("')"), false, false);
	userEntity->SetValue(_N("username"), username);
	userEntity->SetValue(_N("realName"), realNameTextCtrl->GetValue());
	userEntity->SetValue(_N("email"), emailTextCtrl->GetValue());
	userEntity->SetValue(_N("description"), descriptionTextCtrl->GetValue());
	userEntity->SetValue(_N("status"), statusComboBox->GetSelection());
	userEntity->SetValue(_N("openToRemind"), remindCheckBox->GetValue());

	wxArrayString exclude;
	if(password.IsEmpty())
		exclude.Add(_N("password"));

	bool ok = false;
	if(this->userEntity) {
		ok = userController.Update(userEntity, wxEmptyString, true, &exclude);
	} else {
		const wxString pk(userEntity->GetPrimaryKey());
		userEntity->SetPrimaryKey(wxEmptyString);
		ok = userController.Insert(userEntity, false, &exclude);
		userEntity->SetPrimaryKey(pk);
	}

	if(!ok) {
		rit->Log(true, _N("UserDialog"), _("The user ") + username + _(" is not saved."), _("Maybe there is a database error."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("The user could not be saved."),
			_("Save")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
		delete userEntity;
	} else {
		if(this->userEntity) {
			this->userEntity->SetValue(_N("username"), userEntity->Get(_N("username"))->ToString());
			this->userEntity->SetValue(_N("password"), userEntity->Get(_N("password"))->ToString());
			this->userEntity->SetValue(_N("realName"), userEntity->Get(_N("realName"))->ToString());
			this->userEntity->SetValue(_N("email"), userEntity->Get(_N("email"))->ToString());
			this->userEntity->SetValue(_N("description"), userEntity->Get(_N("description"))->ToString());
			this->userEntity->SetValue(_N("status"), userEntity->Get(_N("status"))->ToUnsignedInt());
			this->userEntity->SetValue(_N("openToRemind"), userEntity->Get(_N("openToRemind"))->ToBool());
		} else {
			this->userEntity = userEntity;
		}
		rit->Log(false, _N("UserDialog"), _("The user ") + username + _(" is saved."));
		EndModal(wxID_OK);
	}
}

UserEntity *UserDialog::GetUser() const {
	return userEntity;
}

void UserDialog::TemplateSelect(wxObject *object) {
	wxTextCtrl *textCtrl = NULL;

	if(object == descriptionStaticText)
		textCtrl = descriptionTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}
