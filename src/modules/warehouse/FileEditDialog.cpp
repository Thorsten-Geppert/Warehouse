#include "FileEditDialog.h"
#include <wx/filedlg.h>
#include <wx/filename.h>
#include <wx/mimetype.h>
#include "../lib/FileController.h"
#include "Configuration.h"
#include "TemplateSelectDialog.h"
#include "BusyInfo.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(FileEditDialog, wxDialog)
	EVT_CLOSE(FileEditDialog::OnClose)
	EVT_MENU(SAVE_EVENT, FileEditDialog::SaveKeyEvent)
	EVT_MENU(BROWSE_EVENT, FileEditDialog::BrowseKeyEvent)
	EVT_MENU(CANCEL_EVENT, FileEditDialog::CloseKeyEvent)
	EVT_TEXT(XRCID("fileTextCtrl"), FileEditDialog::FileInputEvent)
END_EVENT_TABLE()

FileEditDialog::FileEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	FileEntity *fileEntity
) : changed(
	false
) {
	SetId(id);
	this->rit        = rit;
	this->fileEntity = fileEntity;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("FileEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("FileEditDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton     = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	browseToolbarButton   = XRCCTRL(*this, "browseToolbarButton", ToolbarButton);
	closeToolbarButton    = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	fileTextCtrl          = XRCCTRL(*this, "fileTextCtrl", DynSoft::DSTextCtrl);
	nameTextCtrl          = XRCCTRL(*this, "nameTextCtrl", DynSoft::DSTextCtrl);
	descriptionStaticText = XRCCTRL(*this, "descriptionStaticText", wxStaticText);
	descriptionTextCtrl   = XRCCTRL(*this, "descriptionTextCtrl", DynSoft::DSTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, FileEditDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(browseToolbarButton, FileEditDialog::BrowseEvent);
	CONNECT_TOOLBARBUTTON(saveToolbarButton, FileEditDialog::SaveEvent);

	descriptionStaticText->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(FileEditDialog::TemplateSelectEvent), NULL, this);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  SAVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,    (int) 'b',  BROWSE_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(450, -1));

	fileTextCtrl->SetChange(&changed);
	nameTextCtrl->SetChange(&changed);
	descriptionTextCtrl->SetChange(&changed);

	if(fileEntity) {
		nameTextCtrl->SetValue(fileEntity->Get(_N("name"))->ToString());
		descriptionTextCtrl->SetValue(fileEntity->Get(_N("description"))->ToString());
	}

	changed = false;

	fileTextCtrl->SetFocus();

	ICON();
}

// Events
void FileEditDialog::OnClose(wxCloseEvent &event) {
	Destroy();
	event.Skip();
}

void FileEditDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void FileEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void FileEditDialog::BrowseEvent(wxMouseEvent &event) {
	Browse();
	SKIP();
}

void FileEditDialog::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void FileEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void FileEditDialog::BrowseKeyEvent(wxCommandEvent &event) {
	Browse();
}

void FileEditDialog::FileInputEvent(wxCommandEvent &event) {
	FileInput();
}

void FileEditDialog::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect(event.GetEventObject());
}

// Methods
void FileEditDialog::Close() {
	wxWindow::Close();
}

void FileEditDialog::Save() {
	const bool edit = fileEntity->Get(_N("fileId"))->ToUnsignedInt() != 0;
	const wxString filename(fileTextCtrl->GetValue());

	if(nameTextCtrl->GetValue().IsEmpty() || (!edit && filename.IsEmpty())) {
		rit->Log(true, _N("FileEditDialog"), _("Could not save file."), _("Filename was empty."));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Please select a file and fill out name."),
			_("Save")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
		return;
	}

	fileEntity->SetValue(_N("name"), nameTextCtrl->GetValue());
	fileEntity->SetValue(_N("description"), descriptionTextCtrl->GetValue());

	if(!filename.IsEmpty() || !edit) {
		wxFileName fileName(filename);
		if(!fileName.FileExists()) {
			rit->Log(true, _N("FileEditDialog"), _("Could not save file."), _("The selected file does not exist."));
			wxMessageDialog *errorMessageDialog = new wxMessageDialog(
				this,
				_("The selected file does not exist."),
				_("Save")
			);
			errorMessageDialog->Center();
			errorMessageDialog->ShowModal();
			errorMessageDialog->Destroy();
			return;
		}

		const wxString extension(fileName.GetExt());
		
		wxString mimetype;
		/*wxMimeTypesManager mimeTypeManager;
		wxFileType *fileType = mimeTypeManager.GetFileTypeFromExtension(extension);
		if(fileType) {
			fileType->GetMimeType(&mimetype);
			delete fileType;
		}*/

		fileEntity->SetValue(_N("origin"), filename);
		fileEntity->SetValue(_N("mimetype"), mimetype);
		fileEntity->SetValue(_N("extension"), extension);
		fileEntity->SetValue(_N("size"), fileName.GetSize().ToULong());
		fileEntity->SetValue(_N("humanSize"), fileName.GetHumanReadableSize(_("n/a")));
	}

	BusyInfo *busyInfo = new BusyInfo(this);
	bool ok = false;
	FileController fileController(rit);
	if(!edit)
		ok = fileController.Add(*fileEntity);
	else
		ok = fileController.Edit(*fileEntity, !filename.IsEmpty());
	
	if(ok) {
		rit->Log(false, _N("FileEditDialog"), _("File was saved."), _("File was saved to database."));
		EndModal(wxID_OK);
	} else {
		rit->Log(true, _N("FileEditDialog"), _("Could not save file."), _("Unkown error (maybe database error)."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not save file."),
			_("Save file")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	}
	delete busyInfo;
}

void FileEditDialog::Browse() {
	wxFileDialog *fileDialog = new wxFileDialog(
		this,
		_("Please choose a file"),
		path,
		wxEmptyString,
		_N("*.*"),
		wxFD_OPEN | wxFD_FILE_MUST_EXIST
	);
	fileDialog->Center();
	if(fileDialog->ShowModal() == wxID_OK)
		fileTextCtrl->SetValue(fileDialog->GetPath());
	path = fileDialog->GetDirectory();
	fileDialog->Destroy();
}

void FileEditDialog::FileInput() {
	wxFileName filename(fileTextCtrl->GetValue());
	if(filename.FileExists())
		nameTextCtrl->SetValue(filename.GetName());
}

void FileEditDialog::TemplateSelect(wxObject *object) {
	DSTextCtrl *textCtrl = NULL;

	if(object == descriptionStaticText)
		textCtrl = descriptionTextCtrl;
	
	TemplateSelectDialog::ShowAndSet(this, rit, textCtrl);
}
