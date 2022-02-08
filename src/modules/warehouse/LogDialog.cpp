#include "LogDialog.h"
#include <wx/xrc/xmlres.h>
#include <DSLog.h>
#include "../lib/LogEntity.h"
#include "../lib/ExportType.h"
#include "../lib/ExportLibrary.h"
#include "ExportDialog.h"
#include "ProgressDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(LogDialog, wxDialog)
	EVT_MENU(RELOAD_EVENT, LogDialog::ReloadKeyEvent)
	EVT_MENU(FIND_EVENT, LogDialog::FindKeyEvent)
	EVT_MENU(CLOSE_EVENT, LogDialog::CloseKeyEvent)
END_EVENT_TABLE()

LogDialog::LogDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("LogDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("LogDialog"));

	mainPanel            = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer         = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	reloadToolbarButton  = XRCCTRL(*this, "reloadToolbarButton", ToolbarButton);
	findToolbarButton    = XRCCTRL(*this, "findToolbarButton", ToolbarButton);
	closeToolbarButton   = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	exportToolbarButton  = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);
	logListCtrl          = XRCCTRL(*this, "logListCtrl", DynSoft::DSListCtrl);

	logListCtrl->InsertColumn(0, _("Date"), wxLIST_FORMAT_LEFT, 150);
	logListCtrl->InsertColumn(1, _("Error"), wxLIST_FORMAT_LEFT, 80);
	logListCtrl->InsertColumn(2, _("Module"), wxLIST_FORMAT_LEFT, 150);
	logListCtrl->InsertColumn(3, _("Username"), wxLIST_FORMAT_LEFT, 150);
	logListCtrl->InsertColumn(4, _("Description"), wxLIST_FORMAT_LEFT, 400);
	logListCtrl->InsertColumn(5, _("Message"), wxLIST_FORMAT_LEFT, 400);

	SetSize(wxSize(800, 600));

	CONNECT_TOOLBARBUTTON(reloadToolbarButton, LogDialog::ReloadEvent);
	CONNECT_TOOLBARBUTTON(findToolbarButton, LogDialog::FindEvent);
	CONNECT_TOOLBARBUTTON(closeToolbarButton, LogDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, LogDialog::ExportEvent);

	wxAcceleratorEntry acceleratorEntries[5];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'r',  RELOAD_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'f',  FIND_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,    (int) 'k',  EXPORT_EVENT);
	wxAcceleratorTable acceleratorTable(5, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	Reload();

	logListCtrl->SetFocus();

	logFindDialog = new LogFindDialog(this, -1, rit);
	logFindDialog->CenterOnScreen();

	ICON();
}

LogDialog::~LogDialog() {
	logListCtrl->CleanUp();
}

void LogDialog::ReloadEvent(wxMouseEvent &event) {
	Reload();
	SKIP();
}

void LogDialog::FindEvent(wxMouseEvent &event) {
	Find();
	SKIP();
}

void LogDialog::ReloadKeyEvent(wxCommandEvent &event) {
	Reload();
}

void LogDialog::FindKeyEvent(wxCommandEvent &event) {
	Find();
}

void LogDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void LogDialog::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void LogDialog::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void LogDialog::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void LogDialog::Reload(DSLogType *logType) {
	logListCtrl->CleanUp();

	DSLog *log = rit->GetLog();
	if(log) {
		DSListCtrl::DSListCtrlRow *row = NULL;
		ProgressDialog *progressDialog = NULL;
		if(logType) {
			progressDialog = new ProgressDialog(this, -1, rit, 0);
			progressDialog->Center();
			progressDialog->Start();

			log->Search(
				wxEmptyString,
				wxEmptyString,
				logType->GetUsername(),
				logType->GetNumber() == 2 ? -1 : logType->GetNumber(),
				logType->GetModule(),
				logType->GetDescription(),
				logType->GetMessage(),
				0 // Limit
			);
			progressDialog->SetCount(log->GetCount());
			delete logType;
			logType = NULL;
		} else {
			log->GetLimited(500);
		}
		unsigned int i = 0;
		DSLogType *entry = NULL;
		while(log->Run()) {
			entry = new DSLogType;
			log->Get(entry);
			row = new DSListCtrl::DSListCtrlRow(reinterpret_cast<wxUIntPtr *>(entry));
			row->Add(entry->GetDate().ToDatabaseDateTime());
			row->Add(entry->GetNumber() == 0 ? _("yes") : _("no"));
			row->Add(entry->GetModule());
			row->Add(entry->GetUsername());
			row->Add(entry->GetDescription());
			row->Add(entry->GetMessage());
			logListCtrl->Append(row);
			if(progressDialog) {
				if(progressDialog->IsCanceled())
					break;
				progressDialog->SetValue(++i);
			}
		}

		if(progressDialog)
			progressDialog->Stop();
	}
}

void LogDialog::Close() {
	wxDialog::Close();
}

void LogDialog::Find() {
	if(logFindDialog->ShowModal() == wxID_OK)
		Reload(logFindDialog->GetLogType());
}

void LogDialog::Export() {
	if(!rit->Access(A_READ, _("You have no permission to export."), this))
		return;

	ExportType exportType;
	ExportDialog *exportDialog = new ExportDialog(this, -1, rit, &exportType);
	exportDialog->Center();

	DSFile file;
	while(exportDialog->ShowModal() == wxID_OK) {
		exportDialog->TransferToExportType();

		if(file.Open(exportType.GetFilename(), _N("w"))) {
			file.Close();

			const unsigned int count = logListCtrl->GetItemCount();
			ExportProgressDialog *exportProgressDialog = new ExportProgressDialog(this, -1, rit);
			ExportLibrary exportLibrary(exportProgressDialog, &exportType, count, false);

			for(unsigned int i = 0; i < count; i++)
				exportLibrary.ExportAdd(LogEntity::ConvertLogType(static_cast<DSLogType *>(logListCtrl->GetPointer(i, 0))));

			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Error writing file. Aborting."),
				_("Export")
			);
			errorMessageDialog->Center();

			if(exportLibrary.ExportStart()) {
				if(exportLibrary.ExportRun()) {
					if(!exportLibrary.ExportStop())
						errorMessageDialog->ShowModal();
				} else {
					errorMessageDialog->ShowModal();
				}
			} else {
				errorMessageDialog->ShowModal();
			}

			errorMessageDialog->Destroy();

			break;
		} else {
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("File is not writeable. Please check file permissions."),
				_("Export")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		}
	}
	exportDialog->Destroy();
}
