#include "FilesPanel.h"
#include <wx/xrc/xmlres.h>
#include "../lib/FileController.h"
#include "../lib/CategoryController.h"
#include "../lib/CategoryEntities.h"
#include "FileEditDialog.h"
#include "CategoryMoveDialog.h"
#include "YesNoDialog.h"
#include "BusyInfo.h"

FilesPanel::FilesPanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &table
) {
	this->rit = rit;
	this->actualCategoryId = 0;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("FilesPanel.xml")));
	wxXmlResource::Get()->LoadPanel(this, parent, _N("FilesPanel"));

	contentSplitterWindow = XRCCTRL(*this, "contentSplitterWindow", wxSplitterWindow);
	categoriesPanel       = XRCCTRL(*contentSplitterWindow, "categoriesPanel", wxPanel);
	categoriesBoxSizer    = static_cast<wxBoxSizer *>(categoriesPanel->GetSizer());
	filesListCtrl         = XRCCTRL(*contentSplitterWindow, "filesListCtrl", DynSoft::DSListCtrl);

	treeCategoriesPanel   = new CategoriesPanel(categoriesPanel, -1, rit, table);
	categoriesBoxSizer->Add(treeCategoriesPanel, 1, wxEXPAND);

	treeCtrl              = treeCategoriesPanel->GetCategoriesTreeCtrl();

	// Files
	filesListCtrl->InsertColumn(0, _("File name"), wxLIST_FORMAT_LEFT, 150);
	filesListCtrl->InsertColumn(1, _("Extension"), wxLIST_FORMAT_LEFT, 80);
	filesListCtrl->InsertColumn(2, _("Size"), wxLIST_FORMAT_RIGHT, 120);
	filesListCtrl->InsertColumn(3, _("Origin"), wxLIST_FORMAT_LEFT, 150);

	// Events
	treeCtrl->Connect(wxEVT_COMMAND_TREE_SEL_CHANGED, wxTreeEventHandler(FilesPanel::SelectCategoryEvent), NULL, this);

	LoadFiles(actualCategoryId);
}

FilesPanel::~FilesPanel() {
	UnloadFiles();
}

void FilesPanel::SelectCategoryEvent(wxTreeEvent &event) {
	CategoryEntity *categoryEntity = reinterpret_cast<CategoryEntity *>(treeCtrl->GetItemData(treeCtrl->GetSelection()));
	SelectCategory(categoryEntity);
}

void FilesPanel::SelectCategory(CategoryEntity *categoryEntity) {
	actualCategoryId = categoryEntity ? categoryEntity->Get(_N("categoryId"))->ToUnsignedInt() : 0U;
	LoadFiles(actualCategoryId);
}

unsigned int FilesPanel::LoadFiles(const unsigned int categoryId) {
	unsigned int count = 0;

	UnloadFiles();

	FileEntity *fileEntity = NULL;

	FileController fileController(rit);
	fileController.GetByCategory(categoryId);
	while(fileController.Run()) {
		fileEntity = new FileEntity(rit);
		fileController.Get(fileEntity);
		AddOrChangeToFilesListCtrl(fileEntity, true);

		count++;
	}

	return count;
}

void FilesPanel::UnloadFiles() {
	filesListCtrl->CleanUp();
}

void FilesPanel::NewCategory() {
	treeCategoriesPanel->NewCategory();
}

void FilesPanel::EditCategory() {
	treeCategoriesPanel->EditCategory();
}

void FilesPanel::RemoveCategory() {
	FileController fileController(rit);
	if(fileController.RemoveByCategory(actualCategoryId)) {
		treeCategoriesPanel->RemoveCategory();
	} else {
		rit->Log(true, _N("FilesPanel"), _("Could not remove category."), _("Could not remove category because could not remove files in this category."));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not remove category because could not remove files in this category."),
			_("Remove category")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	}
}

void FilesPanel::NewFile() {
	filesListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to add a new file."), this))
		return;

	FileEntity fileEntity(rit);
	fileEntity.SetValue(_N("categoryId"), actualCategoryId);

	FileEditDialog *fileEditDialog = new FileEditDialog(
		this,
		-1,
		rit,
		&fileEntity
	);
	fileEditDialog->Center();
	if(fileEditDialog->ShowModal() == wxID_OK)
		AddOrChangeToFilesListCtrl(&fileEntity, true);
}

void FilesPanel::EditFile() {
	filesListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to edit a file."), this))
		return;

	const unsigned int *fileId = static_cast<const unsigned int *>(filesListCtrl->GetSingleSelectionPointer());
	if(!IsFilePointerNotNull(fileId, _("Please select the file you want to edit.")))
		return;

	FileController fileController(rit);
	fileController.Get(DSCasts::ToString(*fileId));
	if(fileController.Run()) {
		FileEntity fileEntity(rit);
		fileController.Get(&fileEntity);
		fileController.Reset();

		FileEditDialog *fileEditDialog = new FileEditDialog(
			this,
			-1,
			rit,
			&fileEntity
		);
		fileEditDialog->Center();
		if(fileEditDialog->ShowModal() == wxID_OK)
			AddOrChangeToFilesListCtrl(&fileEntity, false);
	}
}

void FilesPanel::RemoveFile() {
	filesListCtrl->SetFocus();

	if(!rit->Access(A_DELETE, _("You have no permission to remove a file."), this))
		return;

	const unsigned int *fileId = static_cast<const unsigned int *>(filesListCtrl->GetSingleSelectionPointer());
	if(!IsFilePointerNotNull(fileId, _("Please select the file you want to remove.")))
		return;

	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you want to remove the selected file?"),
		_("Remove file"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		FileController fileController(rit);
		if(fileController.Remove(*fileId)) {
			rit->Log(false, _N("FilesPanel"), _("The file was removed."), _("File id:") + DSCasts::ToString(*fileId));
			filesListCtrl->DeleteItem(filesListCtrl->GetSingleSelectionPosition());
			delete fileId;
		} else {
			rit->Log(true, _N("FilesPanel"), _("The file was not removed."), _("File id:") + DSCasts::ToString(*fileId) + _N(". Could not find file in database."));
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not find file in database."),
				_("Edit file")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
			return;
		}
	}
	yesNoDialog->Destroy();
}

void FilesPanel::AddOrChangeToFilesListCtrl(FileEntity *fileEntity, const bool justAppend) {
	if(!fileEntity)
		return;

	unsigned int *foundFileId = NULL;
	const unsigned int fileId = fileEntity->Get(_N("fileId"))->ToUnsignedInt();

	if(!justAppend) {
		const unsigned int count = filesListCtrl->GetItemCount();
		for(unsigned int i = 0; i < count; i++) {
			foundFileId = static_cast<unsigned int *>(filesListCtrl->GetPointer(i, 0));
			if(foundFileId && *foundFileId == fileId)
				break;
			foundFileId = NULL;
		}
	}

	if(!foundFileId && !justAppend)
		return;

	if(!foundFileId)
		foundFileId = new unsigned int(fileId);

	DSListCtrl::DSListCtrlRow *filesListCtrlRow = new DSListCtrl::DSListCtrlRow(foundFileId);

	filesListCtrlRow->Add(fileEntity->Get(_N("name"))->ToString());
	filesListCtrlRow->Add(fileEntity->Get(_N("extension"))->ToString());
	filesListCtrlRow->Add(fileEntity->Get(_N("humanSize"))->ToString());
	filesListCtrlRow->Add(fileEntity->Get(_N("origin"))->ToString());

	filesListCtrl->Set(filesListCtrlRow, true);
}

DSListCtrl *FilesPanel::GetFilesListCtrl() const {
	return filesListCtrl;
}

wxTreeCtrl *FilesPanel::GetCategoriesTreeCtrl() const {
	return treeCtrl;
}

void FilesPanel::OpenFile(const unsigned int *fileId) {
	if(!rit->Access(A_READ, _("You have no permission to open a file."), this))
		return;

	wxString filename;
	if(!fileId)
		fileId = static_cast<const unsigned int *>(filesListCtrl->GetSingleSelectionPointer());
	if(!IsFilePointerNotNull(fileId, _("Please select the file you want to open.")))
		return;

	BusyInfo *busyInfo = new BusyInfo(this);
	FileController fileController(rit);
	if(fileController.OpenFile(*fileId, &filename) == -1) {
		wxString message(_("Could not find file."));

		if(!filename.IsEmpty())
			message = _("Could not open file. It is saved under: ") + filename;

		rit->Log(true, _N("FilesPanel"), _("Could not open file."), _("File id:") + DSCasts::ToString(*fileId) + _N(".") + message);
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			message,
			_("Open")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		rit->Log(false, _N("FilesPanel"), _("File was opened."), _("File id:") + DSCasts::ToString(*fileId));
	}
	delete busyInfo;
}

void FilesPanel::DownloadFile(const unsigned int *fileId) {
	filesListCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to download a file."), this))
		return;

	if(!fileId)
		fileId = static_cast<const unsigned int *>(filesListCtrl->GetSingleSelectionPointer());
	if(!IsFilePointerNotNull(fileId, _("Please select the file you want to download.")))
		return;

	FileController fileController(rit);
	wxString filename(fileController.GetFilename(*fileId));
	
	wxFileDialog *fileDialog = new wxFileDialog(
		this,
		_("Download file"),
		wxEmptyString,
		filename,
		_N("*.*"),
		wxFD_SAVE
	);
	fileDialog->CenterOnScreen();
	if(fileDialog->ShowModal() == wxID_OK) {
		BusyInfo *busyInfo = new BusyInfo(this);
		filename = fileDialog->GetPath();
		if(!fileController.ExportFile(*fileId, filename)) {
			rit->Log(true, _N("FilesPanel"), _("Could not download file."), _("File id:") + DSCasts::ToString(*fileId));
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not download file."),
				_("Download")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
		} else {
			rit->Log(true, _N("FilesPanel"), _("File was downloaded."), _("File id:") + DSCasts::ToString(*fileId));
		}
		delete busyInfo;
	}
	fileDialog->Destroy();
}

void FilesPanel::MoveFile() {
	filesListCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to move a file."), this))
		return;

	const unsigned int *fileId = static_cast<const unsigned int *>(filesListCtrl->GetSingleSelectionPointer());
	if(!IsFilePointerNotNull(fileId, _("Please select the file you want to move.")))
		return;

	CategoryMoveDialog *categoryMoveDialog = new CategoryMoveDialog(
		this,
		-1,
		rit,
		_N("file_categories")
	);
	categoryMoveDialog->Center();
	if(categoryMoveDialog->ShowModal() == wxID_OK) {
		FileController fileController(rit);
		const unsigned int categoryId = categoryMoveDialog->GetSelectedCategoryId();
		if(fileController.MoveFile(*fileId, categoryId)) {
			rit->Log(false, _N("FilesPanel"), _("File was moved."), _("File id:") + DSCasts::ToString(*fileId));
			if(actualCategoryId != categoryId) {
				filesListCtrl->DeleteItem(filesListCtrl->GetSingleSelectionPosition());
				delete fileId;
			}
		} else {
			rit->Log(true, _N("FilesPanel"), _("Could not move file."), _("File id:") + DSCasts::ToString(*fileId) + _N(". Database error."));
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("Could not move file. Database error."),
				_("Open")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();

			return;
		}
	}
	categoryMoveDialog->Destroy();
}

bool FilesPanel::IsFilePointerNotNull(const unsigned int *filePointer, wxString message) {
	if(filePointer)
		return true;
	
	if(message.IsEmpty())
		message = _("Please select a file.");
	
	wxMessageDialog *errorMessageDialog = new wxMessageDialog(
		this,
		message,
		_("Error")
	);
	errorMessageDialog->CenterOnScreen();
	errorMessageDialog->ShowModal();
	errorMessageDialog->Destroy();
	
	return false;
}

void FilesPanel::Update() {
	filesListCtrl->SetFocus();

	UnloadFiles();

	CategoryController categoryController(rit, _N("file_categories"));
	CategoryEntities *categoryEntities = categoryController.GetAll();
	categoryEntities->FillTreeCtrl(*treeCtrl, treeCtrl->GetRootItem(), actualCategoryId, true);
	delete categoryEntities;

	CategoryEntity *categoryEntity = reinterpret_cast<CategoryEntity *>(treeCtrl->GetItemData(treeCtrl->GetSelection()));
	SelectCategory(categoryEntity);
}

unsigned int *FilesPanel::GetSelectedFileId() {
	return static_cast<unsigned int *>(GetFilesListCtrl()->GetSingleSelectionPointer());
}
