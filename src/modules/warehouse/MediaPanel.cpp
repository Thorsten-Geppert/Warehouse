#include "MediaPanel.h"
#include <wx/xrc/xmlres.h>
#include "Configuration.h"
#include "YesNoDialog.h"
#include "FileOpenOrDownloadDialog.h"
#include "../lib/ListEntryController.h"
#include "../lib/CategoryController.h"
#include "../lib/FileEntity.h"
#include "../lib/FileController.h"

BEGIN_EVENT_TABLE(MediaPanel, wxPanel)
	EVT_BUTTON(XRCID("addButton"), MediaPanel::AddEvent)
	EVT_BUTTON(XRCID("removeButton"), MediaPanel::RemoveEvent)
	EVT_BUTTON(XRCID("changeButton"), MediaPanel::ChangeDescriptionEvent)
	EVT_LIST_ITEM_ACTIVATED(XRCID("mediaListCtrl"), MediaPanel::OpenLinkedEvent)
END_EVENT_TABLE()

MediaPanel::MediaPanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &table,
	LinkedFileEntities *linkedFileEntities,
	bool *changed,
	const wxString &defaultDescription
) {
	SetId(id);
	this->rit = rit;
	this->linkedFileEntities = linkedFileEntities;
	this->changed = changed;
	this->editable = true;

	SetTable(table);
	
	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("MediaPanel.xml")));
	wxXmlResource::Get()->LoadPanel(this, parent, _N("MediaPanel"));
	mainBoxSizer              = static_cast<wxBoxSizer *>(GetSizer());
	wxPanel *tmpFilesPanel    = XRCCTRL(*this, "filesPanel", wxPanel);
	filesBoxSizer             = static_cast<wxBoxSizer *>(tmpFilesPanel->GetSizer());
	filesPanel                = new FilesPanel(tmpFilesPanel, -1, rit, _N("file_categories"));
	filesBoxSizer->Add(filesPanel, 1, wxEXPAND);
	addAsComboBox             = XRCCTRL(*this, "addAsComboBox", wxComboBox);
	addButton                 = XRCCTRL(*this, "addButton", wxButton);
	mediaListCtrl             = XRCCTRL(*this, "mediaListCtrl", DynSoft::DSListCtrl);
	removeButton              = XRCCTRL(*this, "removeButton", wxButton);
	changeDescriptionCombobox = XRCCTRL(*this, "changeDescriptionCombobox", wxComboBox);
	changeButton              = XRCCTRL(*this, "changeButton", wxButton);

	filesPanel->GetFilesListCtrl()->Connect(wxEVT_COMMAND_LIST_ITEM_ACTIVATED, wxListEventHandler(MediaPanel::OpenFileEvent), NULL, this);

	mediaListCtrl->InsertColumn(0, _("Description"), wxLIST_FORMAT_LEFT, 100);
	mediaListCtrl->InsertColumn(1, _("File"), wxLIST_FORMAT_LEFT, 400);

	ListEntryController::FillComboBox(rit, addAsComboBox, _N("linked_media_descriptions"), _N("linked_media_description"), ListEntryController::FILL_NEW, true);
	ListEntryController::FillComboBox(rit, changeDescriptionCombobox, _N("linked_media_descriptions"), _N("linked_media_description"), ListEntryController::FILL_NEW, true);

	if(linkedFileEntities) {
		const unsigned int count = linkedFileEntities->GetCount();
		for(unsigned int i = 0; i < count; i++)
			AddToMediaListCtrl(linkedFileEntities->Get(i));
	}

	addAsComboBox->SetValue(defaultDescription);

	filesBoxSizer->Layout();
	mainBoxSizer->Layout();
}

MediaPanel::~MediaPanel() {
}

void MediaPanel::AddEvent(wxCommandEvent &event) {
	Add();
}

void MediaPanel::RemoveEvent(wxCommandEvent &event) {
	Remove();
}

void MediaPanel::ChangeDescriptionEvent(wxCommandEvent &event) {
	ChangeDescription();
}

void MediaPanel::OpenLinkedEvent(wxListEvent &event) {
	OpenLinked();
}

void MediaPanel::OpenFileEvent(wxListEvent &event) {
	OpenFile();
}

void MediaPanel::Add() {
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

	if(!rit->Access(A_WRITE, _("You have no permission to add a file."), this))
		return;

	const unsigned int *fileId = filesPanel->GetSelectedFileId();
	if(!fileId) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the file you want to link."),
			_("Link file")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	LinkedFileEntity *linkedFileEntity = new LinkedFileEntity(rit, GetTable());
	linkedFileEntity->SetValue(_N("fileId"), *fileId);
	linkedFileEntity->SetValue(_N("description"), addAsComboBox->GetValue());

	AddToMediaListCtrl(linkedFileEntity);
	linkedFileEntities->Add(linkedFileEntity);

	if(changed)
		*changed = true;
}

void MediaPanel::Remove() {
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

	if(!rit->Access(A_WRITE, _("You have no permission to remove a file."), this))
		return;

	LinkedFileEntity *linkedFileEntity = static_cast<LinkedFileEntity *>(mediaListCtrl->GetSingleSelectionPointer());
	if(!linkedFileEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the file you want to unlink."),
			_("Unlink file")
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
		_("Do you really want to unlink the selected file?"),
		_("Unlink file"),
		_("Unlink file"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		mediaListCtrl->DeleteItem(mediaListCtrl->GetPosition(linkedFileEntity));
		linkedFileEntities->AddToRemove(linkedFileEntity);

		if(changed)
			*changed = true;
	}
	yesNoDialog->Destroy();
}

void MediaPanel::AddToMediaListCtrl(LinkedFileEntity *linkedFileEntity) {
	FileController fileController(rit);
	fileController.Get(linkedFileEntity->Get(_N("fileId"))->ToString());
	if(fileController.Run()) {
		FileEntity fileEntity(rit);
		fileController.Get(&fileEntity);
		fileController.Reset();

		DSListCtrl::DSListCtrlRow *mediaListCtrlRow = new DSListCtrl::DSListCtrlRow(linkedFileEntity);
		mediaListCtrlRow->Add(linkedFileEntity->Get(_N("description"))->ToString());
		mediaListCtrlRow->Add(
			GetPathName(
				fileEntity.Get(_N("name"))->ToString(),
				fileEntity.Get(_N("categoryId"))->ToUnsignedInt()
			)
		);

		mediaListCtrl->Set(mediaListCtrlRow, true);
	} else {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not find file in database. File could not be linked."),
			_("Link file")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	}
}

void MediaPanel::SetTable(const wxString &table) {
	this->table = table;
}

wxString MediaPanel::GetTable() const {
	return table;
}

void MediaPanel::ChangeDescription() {
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

	if(!rit->Access(A_WRITE, _("You have no permission to change the description."), this))
		return;

	LinkedFileEntity *linkedFileEntity = static_cast<LinkedFileEntity *>(mediaListCtrl->GetSingleSelectionPointer());
	if(!linkedFileEntity) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the entry you want to change."),
			_("Change description")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	FileController fileController(rit);
	fileController.Get(linkedFileEntity->Get(_N("fileId"))->ToString());
	if(fileController.Run()) {
		FileEntity fileEntity(rit);
		fileController.Get(&fileEntity);
		fileController.Reset();

		const wxString description(changeDescriptionCombobox->GetValue());
		mediaListCtrl->SetText(mediaListCtrl->GetPosition(linkedFileEntity), 0, description);

		linkedFileEntity->SetValue(_N("description"), description);

		if(changed)
			*changed = true;
	} else {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not find file in database. File could not be linked."),
			_("Link file")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	}
}

wxString MediaPanel::GetPathName(
	const wxString &name,
	const unsigned int categoryId
) {
	CategoryController categoryController(rit, _N("file_categories"));
	wxArrayString pathArrayString;
	categoryController.GetPath(categoryId, &pathArrayString);

	return wxString(categoryController.ConvertPathToString(pathArrayString, _N("/"), _("Files")) + name);
}

void MediaPanel::OpenLinked() {
	LinkedFileEntity *linkedFileEntity = static_cast<LinkedFileEntity *>(mediaListCtrl->GetSingleSelectionPointer());
	if(!linkedFileEntity) {
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select the file you want to open or download."),
			_("Open or download")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();

		return;
	}

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

	unsigned int fileId = linkedFileEntity->Get(_N("fileId"))->ToUnsignedInt();

	switch(todo) {
		case FileOpenOrDownloadDialog::OPEN:
			filesPanel->OpenFile(&fileId);
			break;
		case FileOpenOrDownloadDialog::DOWNLOAD:
			filesPanel->DownloadFile(&fileId);
			break;
	}
}

void MediaPanel::OpenFile() {
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

void MediaPanel::SetEditable(const bool editable) {
	this->editable = editable;

	addAsComboBox->Enable(editable);
	addButton->Enable(editable);
	removeButton->Enable(editable);
	changeDescriptionCombobox->Enable(editable);
	changeButton->Enable(editable);
}

bool MediaPanel::IsEditable() const {
	return editable;
}
