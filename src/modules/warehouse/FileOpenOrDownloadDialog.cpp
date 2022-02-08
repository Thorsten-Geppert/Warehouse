#include "FileOpenOrDownloadDialog.h"
#include <wx/xrc/xmlres.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(FileOpenOrDownloadDialog, wxDialog)
	EVT_BUTTON(XRCID("openButton"), FileOpenOrDownloadDialog::OpenEvent)
	EVT_BUTTON(XRCID("downloadButton"), FileOpenOrDownloadDialog::DownloadEvent)
	EVT_MENU(CANCEL_EVENT, FileOpenOrDownloadDialog::CancelEvent)
END_EVENT_TABLE()

FileOpenOrDownloadDialog::FileOpenOrDownloadDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;
	this->todo = NOTHING;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("FileOpenOrDownloadDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("FileOpenOrDownloadDialog"));

	mainPanel      = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer   = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	openButton     = XRCCTRL(*this, "openButton", wxButton);
	downloadButton = XRCCTRL(*this, "downloadButton", wxButton);

	mainBoxSizer->SetSizeHints(this);

	wxAcceleratorEntry acceleratorEntries[1];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	wxAcceleratorTable acceleratorTable(1, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	openButton->SetFocus();

	ICON();
}

void FileOpenOrDownloadDialog::OpenEvent(wxCommandEvent &event) {
	Open();
}

void FileOpenOrDownloadDialog::DownloadEvent(wxCommandEvent &event) {
	Download();
}

void FileOpenOrDownloadDialog::CancelEvent(wxCommandEvent &event) {
	Cancel();
}

void FileOpenOrDownloadDialog::Open() {
	this->todo = OPEN;
	EndModal(wxID_YES);
}

void FileOpenOrDownloadDialog::Download() {
	this->todo = DOWNLOAD;
	EndModal(wxID_YES);
}

void FileOpenOrDownloadDialog::Cancel() {
	this->todo = NOTHING;
	EndModal(wxID_NO);
}

unsigned int FileOpenOrDownloadDialog::GetTodo() const {
	return todo;
}
