#include "FilesModulePanel.h"
#include <wx/xrc/xmlres.h>
#include <wx/imaglist.h>
#include "FileOpenOrDownloadDialog.h"
#include "../lib/PermissionLibrary.h"
#include "../lib/CategoryEntity.h"
#include "../lib/CategoryEntities.h"
#include "../lib/CategoryController.h"

BEGIN_EVENT_TABLE(FilesModulePanel, ModulePanel)
	EVT_MENU(NEW_CATEGORY_EVENT, FilesModulePanel::NewCategoryKeyEvent)
	EVT_MENU(EDIT_CATEGORY_EVENT, FilesModulePanel::EditCategoryKeyEvent)
	EVT_MENU(REMOVE_CATEGORY_EVENT, FilesModulePanel::RemoveCategoryKeyEvent)
	EVT_MENU(NEW_FILE_EVENT, FilesModulePanel::NewFileKeyEvent)
	EVT_MENU(EDIT_FILE_EVENT, FilesModulePanel::EditFileKeyEvent)
	EVT_MENU(REMOVE_FILE_EVENT, FilesModulePanel::RemoveFileKeyEvent)
	EVT_MENU(MOVE_FILE_EVENT, FilesModulePanel::MoveFileKeyEvent)
	EVT_MENU(DOWNLOAD_FILE_EVENT, FilesModulePanel::DownloadFileKeyEvent)
	EVT_MENU(UPDATE_FILE_EVENT, FilesModulePanel::UpdateKeyEvent)
END_EVENT_TABLE()

FilesModulePanel::FilesModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("FilesModulePanel")
) {
	// Toolbar
	newCategoryToolbarButton     = XRCCTRL(*this, "newCategoryToolbarButton", ToolbarButton);
	editCategoryToolbarButton    = XRCCTRL(*this, "editCategoryToolbarButton", ToolbarButton);
	removeCategoryToolbarButton  = XRCCTRL(*this, "removeCategoryToolbarButton", ToolbarButton);
	newFileToolbarButton         = XRCCTRL(*this, "newFileToolbarButton", ToolbarButton);
	editFileToolbarButton        = XRCCTRL(*this, "editFileToolbarButton", ToolbarButton);
	removeFileToolbarButton      = XRCCTRL(*this, "removeFileToolbarButton", ToolbarButton);
	moveFileToolbarButton        = XRCCTRL(*this, "moveFileToolbarButton", ToolbarButton);
	downloadFileToolbarButton    = XRCCTRL(*this, "downloadFileToolbarButton", ToolbarButton);
	updateToolbarButton         = XRCCTRL(*this, "updateToolbarButton", ToolbarButton);
	mainPanel                    = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                 = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	filesPanel                   = new FilesPanel(mainPanel, -1, rit, _N("file_categories"));
	mainBoxSizer->Add(filesPanel, 1, wxEXPAND);
	filesListCtrl                = filesPanel->GetFilesListCtrl();
	
	// Events
	CONNECT_TOOLBARBUTTON(newCategoryToolbarButton, FilesModulePanel::NewCategoryEvent);
	CONNECT_TOOLBARBUTTON(editCategoryToolbarButton, FilesModulePanel::EditCategoryEvent);
	CONNECT_TOOLBARBUTTON(removeCategoryToolbarButton, FilesModulePanel::RemoveCategoryEvent);
	CONNECT_TOOLBARBUTTON(newFileToolbarButton, FilesModulePanel::NewFileEvent);
	CONNECT_TOOLBARBUTTON(editFileToolbarButton, FilesModulePanel::EditFileEvent);
	CONNECT_TOOLBARBUTTON(removeFileToolbarButton, FilesModulePanel::RemoveFileEvent);
	CONNECT_TOOLBARBUTTON(moveFileToolbarButton, FilesModulePanel::MoveFileEvent);
	CONNECT_TOOLBARBUTTON(downloadFileToolbarButton, FilesModulePanel::DownloadFileEvent);
	CONNECT_TOOLBARBUTTON(updateToolbarButton, FilesModulePanel::UpdateEvent);
	filesListCtrl->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(FilesModulePanel::OpenFileEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[9];
	acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 'k', NEW_CATEGORY_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, (int) 'm', EDIT_CATEGORY_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL, (int) 'b', REMOVE_CATEGORY_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL, (int) 'n', NEW_FILE_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL, (int) 'e', EDIT_FILE_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL, (int) 'd', REMOVE_FILE_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL, (int) 'x', MOVE_FILE_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL, (int) 'i', DOWNLOAD_FILE_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL, (int) 'r', UPDATE_FILE_EVENT);
	wxAcceleratorTable acceleratorTable(9, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);
}

void FilesModulePanel::Load() {
	filesPanel->SetFocus();
}

void FilesModulePanel::Unload() {
}

void FilesModulePanel::NewCategoryEvent(wxMouseEvent &event) {
	NewCategory();
	SKIP();
}

void FilesModulePanel::EditCategoryEvent(wxMouseEvent &event) {
	EditCategory();
	SKIP();
}

void FilesModulePanel::RemoveCategoryEvent(wxMouseEvent &event) {
	RemoveCategory();
	SKIP();
}

void FilesModulePanel::NewFileEvent(wxMouseEvent &event) {
	NewFile();
	SKIP();
}

void FilesModulePanel::EditFileEvent(wxMouseEvent &event) {
	EditFile();
	SKIP();
}

void FilesModulePanel::RemoveFileEvent(wxMouseEvent &event) {
	RemoveFile();
	SKIP();
}

void FilesModulePanel::MoveFileEvent(wxMouseEvent &event) {
	MoveFile();
	SKIP();
}

void FilesModulePanel::DownloadFileEvent(wxMouseEvent &event) {
	DownloadFile();
	SKIP();
}

void FilesModulePanel::UpdateEvent(wxMouseEvent &event) {
	Update();
	SKIP();
}

void FilesModulePanel::NewCategoryKeyEvent(wxCommandEvent &event) {
	NewCategory();
}

void FilesModulePanel::EditCategoryKeyEvent(wxCommandEvent &event) {
	EditCategory();
}

void FilesModulePanel::RemoveCategoryKeyEvent(wxCommandEvent &event) {
	RemoveCategory();
}

void FilesModulePanel::NewFileKeyEvent(wxCommandEvent &event) {
	NewFile();
}

void FilesModulePanel::EditFileKeyEvent(wxCommandEvent &event) {
	EditFile();
}

void FilesModulePanel::RemoveFileKeyEvent(wxCommandEvent &event) {
	RemoveFile();
}

void FilesModulePanel::MoveFileKeyEvent(wxCommandEvent &event) {
	MoveFile();
}

void FilesModulePanel::DownloadFileKeyEvent(wxCommandEvent &event) {
	DownloadFile();
}

void FilesModulePanel::UpdateKeyEvent(wxCommandEvent &event) {
	Update();
}

void FilesModulePanel::OpenFileEvent(wxListEvent &event) {
	OpenFile();
}

void FilesModulePanel::OpenFile() {
	FileOpenOrDownloadDialog *fileOpenOrDownloadDialog = new FileOpenOrDownloadDialog(
		this,
		-1,
		rit
	);

	unsigned int todo = FileOpenOrDownloadDialog::NOTHING;
	fileOpenOrDownloadDialog->Center();
	if(fileOpenOrDownloadDialog->ShowModal() == wxID_YES)
		todo = fileOpenOrDownloadDialog->GetTodo();
	fileOpenOrDownloadDialog->Destroy();

	switch(todo) {
		case FileOpenOrDownloadDialog::OPEN:
			filesPanel->OpenFile();
			break;
		case FileOpenOrDownloadDialog::DOWNLOAD:
			filesPanel->DownloadFile();
			break;
	}
}

void FilesModulePanel::NewCategory() {
	filesPanel->NewCategory();
}

void FilesModulePanel::EditCategory() {
	filesPanel->EditCategory();
}

void FilesModulePanel::RemoveCategory() {
	filesPanel->RemoveCategory();
}

void FilesModulePanel::NewFile() {
	filesPanel->NewFile();
}

void FilesModulePanel::EditFile() {
	filesPanel->EditFile();
}

void FilesModulePanel::RemoveFile() {
	filesPanel->RemoveFile();
}

void FilesModulePanel::MoveFile() {
	filesPanel->MoveFile();
}

void FilesModulePanel::DownloadFile() {
	filesPanel->DownloadFile();
}

void FilesModulePanel::Update() {
	filesPanel->Update();
}
