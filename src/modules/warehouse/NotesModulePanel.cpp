#include "NotesModulePanel.h"
#include <wx/xrc/xmlres.h>
#include <wx/filedlg.h>
#include <DSCasts.h>
#include <DSFile.h>
#include "../lib/PermissionLibrary.h"
#include "TextInputDialog.h"
#include "EditedInformationsDialog.h"
#include "PrintPreviewDialog.h"
#include "YesNoDialog.h"
#include "TemplateSelectDialog.h"

BEGIN_EVENT_TABLE(NotesModulePanel, ModulePanel)
	EVT_LISTBOX(XRCID("notesListBox"), NotesModulePanel::LoadNoteEvent)
	EVT_MENU(NEW_EVENT, NotesModulePanel::NewKeyEvent)
	EVT_MENU(EDIT_EVENT, NotesModulePanel::EditKeyEvent)
	EVT_MENU(REMOVE_EVENT, NotesModulePanel::RemoveKeyEvent)
	EVT_MENU(UPDATE_EVENT, NotesModulePanel::UpdateKeyEvent)
	EVT_MENU(EDITED_INFORMATIONS_EVENT, NotesModulePanel::ShowEditedInformationsKeyEvent)
	EVT_MENU(PRINT_EVENT, NotesModulePanel::PrintKeyEvent)
	EVT_MENU(PRINT_PREVIEW_EVENT, NotesModulePanel::PrintPreviewKeyEvent)
	EVT_MENU(EXPORT_EVENT, NotesModulePanel::ExportKeyEvent)
	EVT_MENU(TEMPLATE_SELECT_EVENT, NotesModulePanel::TemplateSelectKeyEvent)
END_EVENT_TABLE()

NotesModulePanel::NotesModulePanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : ModulePanel(
	parent,
	id,
	rit,
	_N("NotesModulePanel")
) {
	noteId            = 0;
	connected         = false;
	tmpNoteEntity     = new NoteEntity(rit);
	tmpNoteController = new NoteController(rit);
	tmpExclude.Add(_N("title"));

	wxArrayString tmpExcludes(*tmpNoteEntity->GetUpdateExcludes());
	const unsigned int count = tmpExcludes.GetCount();
	for(unsigned int i = 0; i < count; i++)
		tmpExclude.Add(tmpExcludes[i]);

	// Toolbar
	newToolbarButton                = XRCCTRL(*this, "newToolbarButton", ToolbarButton);
	editToolbarButton               = XRCCTRL(*this, "editToolbarButton", ToolbarButton);
	removeToolbarButton             = XRCCTRL(*this, "removeToolbarButton", ToolbarButton);
	updateToolbarButton            = XRCCTRL(*this, "updateToolbarButton", ToolbarButton);
	printToolbarButton              = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	templateToolbarButton           = XRCCTRL(*this, "templateToolbarButton", ToolbarButton);
	exportToolbarButton             = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);
	importToolbarButton             = XRCCTRL(*this, "importToolbarButton", ToolbarButton);
	printPreviewToolbarButton       = XRCCTRL(*this, "printPreviewToolbarButton", ToolbarButton);
	editedInformationsToolbarButton = XRCCTRL(*this, "editedInformationsToolbarButton", ToolbarButton);
	notesListBox                    = XRCCTRL(*this, "notesListBox", wxListBox);
	noteTextCtrl                    = XRCCTRL(*this, "noteTextCtrl", wxTextCtrl);

	// Events
	CONNECT_TOOLBARBUTTON(newToolbarButton, NotesModulePanel::NewEvent);
	CONNECT_TOOLBARBUTTON(editToolbarButton, NotesModulePanel::EditEvent);
	CONNECT_TOOLBARBUTTON(removeToolbarButton, NotesModulePanel::RemoveEvent);
	CONNECT_TOOLBARBUTTON(updateToolbarButton, NotesModulePanel::UpdateEvent);
	CONNECT_TOOLBARBUTTON(editedInformationsToolbarButton, NotesModulePanel::ShowEditedInformationsEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, NotesModulePanel::PrintEvent);
	CONNECT_TOOLBARBUTTON(printPreviewToolbarButton, NotesModulePanel::PrintPreviewEvent);
	CONNECT_TOOLBARBUTTON(templateToolbarButton, NotesModulePanel::TemplateSelectEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, NotesModulePanel::ExportEvent);
	CONNECT_TOOLBARBUTTON(importToolbarButton, NotesModulePanel::ImportEvent);

	wxAcceleratorEntry acceleratorEntries[10];
	acceleratorEntries[0].Set(wxACCEL_CTRL,                  (int) 'n',  NEW_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,                  (int) 'e',  EDIT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,                  (int) 'd',  REMOVE_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,                  (int) 'r',  UPDATE_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL,                  (int) 'i',  EDITED_INFORMATIONS_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL,                  (int) 'p',  PRINT_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL,                  (int) 'k',  EXPORT_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL,                  (int) 'j',  IMPORT_EVENT);
	acceleratorEntries[8].Set(wxACCEL_CTRL,                  (int) 't',  TEMPLATE_SELECT_EVENT);
	acceleratorEntries[9].Set(wxACCEL_CTRL | wxACCEL_ALT,    (int) 'p',  PRINT_PREVIEW_EVENT);
	wxAcceleratorTable acceleratorTable(10, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	// Stuff
	Update();

	LoadNote(0);
}

NotesModulePanel::~NotesModulePanel() {
	ClearNotesListBox();
	delete tmpNoteController;
	delete tmpNoteEntity;
}

void NotesModulePanel::Load() {
	notesListBox->SetFocus();
}

void NotesModulePanel::Unload() {
}

void NotesModulePanel::NewEvent(wxMouseEvent &event) {
	New();
	SKIP();
}

void NotesModulePanel::EditEvent(wxMouseEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Edit(noteId ? *noteId : 0);
	SKIP();
}

void NotesModulePanel::RemoveEvent(wxMouseEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Remove(noteId ? *noteId : 0);
	SKIP();
}

void NotesModulePanel::PrintEvent(wxMouseEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Print(noteId ? *noteId : 0);
	SKIP();
}

void NotesModulePanel::PrintPreviewEvent(wxMouseEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Print(noteId ? *noteId : 0, true);
	SKIP();
}

void NotesModulePanel::UpdateEvent(wxMouseEvent &event) {
	Update();
	SKIP();
}

void NotesModulePanel::LoadNoteEvent(wxCommandEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(event.GetClientData());
	LoadNote(noteId ? *noteId : 0);
}

void NotesModulePanel::EditNoteEvent(wxCommandEvent &event) {
	EditNote();
}

void NotesModulePanel::ShowEditedInformationsEvent(wxMouseEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	ShowEditedInformations(noteId ? *noteId : 0);
	SKIP();
}

void NotesModulePanel::NewKeyEvent(wxCommandEvent &event) {
	New();
}

void NotesModulePanel::EditKeyEvent(wxCommandEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Edit(noteId ? *noteId : 0);
}

void NotesModulePanel::RemoveKeyEvent(wxCommandEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Remove(noteId ? *noteId : 0);
}

void NotesModulePanel::PrintKeyEvent(wxCommandEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Print(noteId ? *noteId : 0);
}

void NotesModulePanel::PrintPreviewKeyEvent(wxCommandEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Print(noteId ? *noteId : 0, true);
}

void NotesModulePanel::UpdateKeyEvent(wxCommandEvent &event) {
	Update();
}

void NotesModulePanel::ShowEditedInformationsKeyEvent(wxCommandEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	ShowEditedInformations(noteId ? *noteId : 0);
}

void NotesModulePanel::ExportEvent(wxMouseEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Export(noteId ? *noteId : 0);
	SKIP();
}

void NotesModulePanel::TemplateSelectEvent(wxMouseEvent &event) {
	TemplateSelect();
	SKIP();
}

void NotesModulePanel::TemplateSelectKeyEvent(wxCommandEvent &event) {
	TemplateSelect();
}

void NotesModulePanel::ExportKeyEvent(wxCommandEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Export(noteId ? *noteId : 0);
}

void NotesModulePanel::ImportEvent(wxMouseEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Import(noteId ? *noteId : 0);
	SKIP();
}

void NotesModulePanel::ImportKeyEvent(wxCommandEvent &event) {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	Import(noteId ? *noteId : 0);
}

void NotesModulePanel::New() {
	if(!rit->Access(A_WRITE, _("You have no permission to create notes."), this))
		return;

	TextInputDialog *nameInputDialog = new TextInputDialog(
		this,
		-1,
		rit,
		_("New note"),
		_("Note name:")
	);
	nameInputDialog->Center();
	if(nameInputDialog->ShowModal() == wxID_OK) {
		NoteEntity noteEntity(rit);
		noteEntity.SetValue(_N("title"), nameInputDialog->GetValue());
		noteEntity.SetValue(_N("note"), wxString(_("New note")));

		NoteController noteController(rit);
		if(noteController.Insert(&noteEntity)) {
			AddToNotesListBox(&noteEntity);
			rit->Log(false, _N("NotesModulePanel"), _("The note is not created."), _("The note with title \"") + noteEntity.Get(_N("title"))->ToString() + _N("\" is created."));
		} else {
			rit->Log(true, _N("NotesModulePanel"), _("The note is not saved."), _("The note with title \"") + noteEntity.Get(_N("title"))->ToString() + _N("\" is not created. Database error."));

			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not save note. Database error."),
				_("New note")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		}
	}
	nameInputDialog->Destroy();

	notesListBox->SetFocus();
}

void NotesModulePanel::Edit(const unsigned int noteId) {
	notesListBox->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to edit notes."), this))
		return;

	NoteController noteController(rit);
	noteController.Get(DSCasts::ToString(noteId));
	if(noteController.Run()) {
		NoteEntity noteEntity(rit);
		noteController.Get(&noteEntity);
		noteController.Reset();

		TextInputDialog *nameInputDialog = new TextInputDialog(
			this,
			-1,
			rit,
			_("Edit note"),
			_("Note name:"),
			false,
			noteEntity.Get(_N("title"))->ToString()
		);
		nameInputDialog->Center();
		if(nameInputDialog->ShowModal() == wxID_OK) {
			noteEntity.SetValue(_N("title"), nameInputDialog->GetValue());
			noteEntity.SetValue(_N("changedAt"), _N("NOW()"));
			wxArrayString exclude;
			exclude.Add(_N("note"));
			exclude.Add(_N("createdAt"));
			if(noteController.Update(&noteEntity, wxEmptyString, true, &exclude)) {
				unsigned int *tmp = NULL;
				const unsigned int count = notesListBox->GetCount();
				for(unsigned int i = 0; i < count; i++) {
					tmp = static_cast<unsigned int *>(notesListBox->GetClientData(i));
					if(*tmp == noteId) {
						notesListBox->SetString(i, noteEntity.Get(_N("title"))->ToString());
						break;
					}
				}

				rit->Log(false, _N("NotesModulePanel"), _("The note is renamed."), _("The note with title \"") + noteEntity.Get(_N("title"))->ToString() + _N("\" is renamed."));
			} else {
				rit->Log(true, _N("NotesModulePanel"), _("The note is not renamed."), _("The note with title \"") + noteEntity.Get(_N("title"))->ToString() + _N("\" is not renamed."));

				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not edit note. Database error."),
					_("New note")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			}
		}
		nameInputDialog->Destroy();
	} else {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the note you want to edit."),
			_("Remove")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}
}

void NotesModulePanel::Remove(const unsigned int noteId) {
	notesListBox->SetFocus();

	if(!rit->Access(A_DELETE, _("You have no permission to remove notes."), this))
		return;

	if(noteId == 0) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the note you want to remove."),
			_("Remove")
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
		_("Do you really want to remove the selected note?"),
		_("Remove"),
		_("Remove"),
		_("Cancel")
	);
	yesNoDialog->Center();
	if(yesNoDialog->ShowModal() == wxID_YES) {
		unsigned int *tmp = NULL;
		NoteEntity noteEntity(rit);
		noteEntity.SetValue(_N("noteId"), noteId);
		NoteController noteController(rit);
		if(noteController.Delete(&noteEntity)) {
			const unsigned int count = notesListBox->GetCount();
			for(unsigned int i = 0; i < count; i++) {
				tmp = static_cast<unsigned int *>(notesListBox->GetClientData(i));
				if(*tmp == noteId) {
					notesListBox->Delete(i);
					break;
				}
			}

			delete tmp;
			LoadNote(0);

			rit->Log(false, _N("NotesModulePanel"), _("The note is removed."), _("The note with title \"") + noteEntity.Get(_N("title"))->ToString() + _N("\" is removed."));
		} else {
			rit->Log(true, _N("NotesModulePanel"), _("The note is not removed."), _("The note with title \"") + noteEntity.Get(_N("title"))->ToString() + _N("\" is not removed."));

			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not remove selected note."),
				_("Remove")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
			return;
		}
	}
	yesNoDialog->Destroy();
}

void NotesModulePanel::Update() {
	long insertionPoint     = 0;
	unsigned int *tmpNoteId = NULL;
	unsigned int noteId     = 0;
	if(notesListBox->GetSelection() != wxNOT_FOUND) {
		tmpNoteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
		if(tmpNoteId) {
			noteId = *tmpNoteId;
			insertionPoint = noteTextCtrl->GetInsertionPoint();
		}
	}

	ClearNotesListBox();

	NoteEntity noteEntity(rit);
	NoteController noteController(rit);
	noteController.Get();
	while(noteController.Run()) {
		noteController.Get(&noteEntity);
		AddToNotesListBox(&noteEntity);
	}

	if(noteId > 0) {
		const unsigned int count = notesListBox->GetCount();
		for(unsigned int i = 0; i < count; i++) {
			tmpNoteId = static_cast<unsigned int *>(notesListBox->GetClientData(i));
			if(tmpNoteId && *tmpNoteId == noteId) {
				notesListBox->SetSelection(i);
				LoadNote(noteId);
				noteTextCtrl->SetInsertionPoint(insertionPoint);
				noteTextCtrl->SetFocus();
				break;
			}
		}
	}
}

void NotesModulePanel::ClearNotesListBox() {
	const unsigned int count = notesListBox->GetCount();
	for(unsigned int i = 0; i < count; i++)
		delete static_cast<unsigned int *>(notesListBox->GetClientData(i));
	notesListBox->Clear();
}

void NotesModulePanel::AddToNotesListBox(NoteEntity *noteEntity) {
	if(!noteEntity)
		return;
	
	notesListBox->Append(
		noteEntity->Get(_N("title"))->ToString(),
		new unsigned int (noteEntity->Get(_N("noteId"))->ToUnsignedInt())
	);
}

bool NotesModulePanel::LoadNote(const unsigned int noteId) {
	if(connected)
		noteTextCtrl->Disconnect(wxEVT_COMMAND_TEXT_UPDATED, wxTextEventHandler(NotesModulePanel::EditNoteEvent), NULL, this);

	if(noteId != 0) {
		NoteController noteController(rit);
		noteController.Get(DSCasts::ToString(noteId));
		if(noteController.Run()) {
			NoteEntity noteEntity(rit);
			noteController.Get(&noteEntity);
			noteController.Reset();

			noteTextCtrl->SetValue(noteEntity.Get(_N("note"))->ToString());

			this->noteId = noteId;

			noteTextCtrl->Connect(wxEVT_COMMAND_TEXT_UPDATED, wxTextEventHandler(NotesModulePanel::EditNoteEvent), NULL, this);
			connected = true;
			noteTextCtrl->Enable();
			noteTextCtrl->SetFocus();

			tmpNoteEntity->SetValue(_N("noteId"), noteId);
			tmpNoteEntity->SetValue(_N("title"), noteEntity.Get(_N("title"))->ToString());
			tmpNoteEntity->SetValue(_N("note"), noteEntity.Get(_N("note"))->ToString());

			return true;
		}
	}

	this->noteId = 0;
	noteTextCtrl->Clear();
	noteTextCtrl->Disable();

	return false;
}

void NotesModulePanel::EditNote() {
	if(!rit->Access(A_WRITE, _("You have no permission to edit notes."), this))
		return;

	tmpNoteEntity->SetValue(_N("noteId"), noteId);
	tmpNoteEntity->SetValue(_N("note"), noteTextCtrl->GetValue());
	if(!tmpNoteController->Update(tmpNoteEntity, wxEmptyString, true, &tmpExclude)) {
		rit->Log(true, _N("NotesModulePanel"), _("The note is not saved."), _("The note with title \"") + tmpNoteEntity->Get(_N("title"))->ToString() + _N("\" is not saved. Database error."));

		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not save note. Database error. Please press update."),
			_("Save note")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return;
	}
}

void NotesModulePanel::ShowEditedInformations(const unsigned int noteId) {
	if(noteId == 0) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a note."),
			_("Note informations")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	NoteController noteController(rit);
	noteController.Get(DSCasts::ToString(noteId));
	if(noteController.Run()) {
		NoteEntity noteEntity(rit);
		noteController.Get(&noteEntity);
		noteController.Reset();

		EditedInformationsDialog *editedInformationsDialog = new EditedInformationsDialog(
			this,
			-1,
			rit,
			&noteEntity
		);
		editedInformationsDialog->Center();
		editedInformationsDialog->ShowModal();
		editedInformationsDialog->Destroy();
	} else {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not find note. Please press update."),
			_("Note informations")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	noteTextCtrl->SetFocus();
}

void NotesModulePanel::Print(const unsigned int noteId, const bool preview) {
	noteTextCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to print."), this))
		return;

	if(noteId == 0) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a note."),
			_("Print")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	NoteController noteController(rit);
	HtmlPrintoutLibrary *htmlPrintoutLibrary = noteController.Print(tmpNoteEntity);

	if(preview) {
		PrintPreviewDialog *printPreviewDialog = new PrintPreviewDialog(this, -1, rit, htmlPrintoutLibrary);
		printPreviewDialog->Center();
		printPreviewDialog->ShowModal();
		printPreviewDialog->Destroy();
	} else {
		wxPrinter printer;
		printer.Print(this, htmlPrintoutLibrary, true);
	}

	delete htmlPrintoutLibrary;
}

void NotesModulePanel::Export(const unsigned int noteId) {
	noteTextCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to export notes."), this))
		return;

	if(noteId == 0) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a note."),
			_("Print")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	wxFileDialog *browseFileDialog = new wxFileDialog(
		this,
		_("Choose a file"),
		lastExportFile,
		_N("export.txt"),
		_N("*.*"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);
	browseFileDialog->Center();
	if(browseFileDialog->ShowModal() == wxID_OK) {
		lastExportFile = browseFileDialog->GetPath();
		DSFile file(lastExportFile, _N("w"));
		if(file.IsOpened()) {
			if(!file.Write(tmpNoteEntity->Get(_N("title"))->ToString() + _N("\n\n") + tmpNoteEntity->Get(_N("note"))->ToString())) {
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not write file."),
					_("Export")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			}

			if(!file.Close()) {
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not close file."),
					_("Export")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			}
		} else {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not open file."),
				_("Export")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		}
	}
	browseFileDialog->Destroy();
}

void NotesModulePanel::Import(const unsigned int noteId) {
	noteTextCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to import notes."), this))
		return;

	if(noteId == 0) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a note."),
			_("Import")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	wxFileDialog *browseFileDialog = new wxFileDialog(
		this,
		_("Choose a file"),
		lastImportFile,
		wxEmptyString,
		_N("*.*"),
		wxFD_OPEN
	);
	browseFileDialog->Center();
	if(browseFileDialog->ShowModal() == wxID_OK) {
		lastImportFile = browseFileDialog->GetPath();

		DSFile file(lastImportFile, _N("r"));
		if(file.IsOpened()) {
			wxString tmp;
			if(!file.ReadAll(&tmp)) {
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not write file."),
					_("Import")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			} else {
				noteTextCtrl->SetValue(tmp);
			}
			
			if(!file.Close()) {
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not close file."),
					_("Import")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			}
		}
	}
	browseFileDialog->Destroy();
}

void NotesModulePanel::TemplateSelect() {
	const unsigned int *noteId = static_cast<unsigned int *>(notesListBox->GetClientData(notesListBox->GetSelection()));
	if(noteId) {
		TemplateSelectDialog::ShowAndSet(this, rit, noteTextCtrl);
	} else {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select a note."),
			_("Templates")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	}
}
