#include "LogFindDialog.h"
#include <DSLog.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(LogFindDialog, wxDialog)
	EVT_MENU(FIND_EVENT, LogFindDialog::FindKeyEvent)
	EVT_MENU(CANCEL_EVENT, LogFindDialog::CancelKeyEvent)
END_EVENT_TABLE()

LogFindDialog::LogFindDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("LogFindDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("LogFindDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	findToolbarButton   = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	errorComboBox       = XRCCTRL(*this, "errorComboBox", wxComboBox);
	userComboBox        = XRCCTRL(*this, "userComboBox", wxComboBox);
	moduleComboBox      = XRCCTRL(*this, "moduleComboBox", wxComboBox);
	//descriptionTextCtrl = XRCCTRL(*this, "descriptionTextCtrl", wxTextCtrl);
	//messageTextCtrl     = XRCCTRL(*this, "messageTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(findToolbarButton, LogFindDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, LogFindDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	errorComboBox->Append(_("yes"));
	errorComboBox->Append(_("no"));
	errorComboBox->Append(_N(""));
	errorComboBox->SetSelection(2);

	ICON();
}

int LogFindDialog::ShowModal() {
	errorComboBox->SetFocus();

	DSLog *log = rit->GetLog();
	if(log) {
		userComboBox->Clear();
		userComboBox->Append(wxEmptyString);
		log->GetUsers();
		while(log->Run())
			userComboBox->Append(log->GetUsername());

		moduleComboBox->Clear();
		moduleComboBox->Append(wxEmptyString);
		log->GetModules();
		while(log->Run())
			moduleComboBox->Append(log->GetModule());
	}

	return wxDialog::ShowModal();
}

void LogFindDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void LogFindDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void LogFindDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void LogFindDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void LogFindDialog::Find() {
	EndModal(wxID_OK);
}

void LogFindDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

DSLogType *LogFindDialog::GetLogType() const {
	DSLogType *logType = new DSLogType;

	logType->SetNumber(errorComboBox->GetSelection());
	logType->SetModule(moduleComboBox->GetValue());
	logType->SetUsername(userComboBox->GetValue());
	//logType->SetDescription(descriptionTextCtrl->GetValue());
	//logType->SetMessage(messageTextCtrl->GetValue());

	return logType;
}
