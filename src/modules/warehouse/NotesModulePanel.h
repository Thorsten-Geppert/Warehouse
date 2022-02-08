#ifndef NOTESMODULEPANEL_H_
#define NOTESMODULEPANEL_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/Toolbar.h"
#include "../lib/NoteEntity.h"
#include "../lib/NoteController.h"

using namespace DynSoft;

class NotesModulePanel : public ModulePanel {

	public:
		NotesModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~NotesModulePanel();

		virtual void Load();
		virtual void Unload();
	
	protected:
		enum {
			NEW_EVENT,
			EDIT_EVENT,
			REMOVE_EVENT,
			UPDATE_EVENT,
			PRINT_EVENT,
			PRINT_PREVIEW_EVENT,
			EDITED_INFORMATIONS_EVENT,
			EXPORT_EVENT,
			IMPORT_EVENT,
			TEMPLATE_SELECT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void NewEvent(wxMouseEvent &event);
		void EditEvent(wxMouseEvent &event);
		void RemoveEvent(wxMouseEvent &event);
		void UpdateEvent(wxMouseEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void PrintPreviewEvent(wxMouseEvent &event);
		void LoadNoteEvent(wxCommandEvent &event);
		void EditNoteEvent(wxCommandEvent &event);
		void ShowEditedInformationsEvent(wxMouseEvent &event);
		void NewKeyEvent(wxCommandEvent &event);
		void EditKeyEvent(wxCommandEvent &event);
		void RemoveKeyEvent(wxCommandEvent &event);
		void UpdateKeyEvent(wxCommandEvent &event);
		void ShowEditedInformationsKeyEvent(wxCommandEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void PrintPreviewKeyEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);
		void TemplateSelectKeyEvent(wxCommandEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);
		void ImportEvent(wxMouseEvent &event);
		void ImportKeyEvent(wxCommandEvent &event);

		void New();
		void Edit(const unsigned int noteId);
		void Remove(const unsigned int noteId);
		void Update();
		void ClearNotesListBox();
		void AddToNotesListBox(NoteEntity *noteEntity);
		bool LoadNote(const unsigned int noteId);
		void EditNote();
		void ShowEditedInformations(const unsigned int noteId);
		void Print(const unsigned int noteId, const bool preview = false);
		void Export(const unsigned int noteId);
		void Import(const unsigned int noteId);
		void TemplateSelect();

		unsigned int noteId;
		bool connected;
		NoteEntity *tmpNoteEntity; // Speed
		NoteController *tmpNoteController; // Speed
		wxArrayString tmpExclude;
		wxString lastExportFile, lastImportFile;

		// tmp
		void ni(wxString module = wxEmptyString);

		ToolbarButton *newToolbarButton;
		ToolbarButton *editToolbarButton;
		ToolbarButton *removeToolbarButton;
		ToolbarButton *updateToolbarButton;
		ToolbarButton *editedInformationsToolbarButton;
		ToolbarButton *printToolbarButton;
		ToolbarButton *printPreviewToolbarButton;
		ToolbarButton *templateToolbarButton;
		ToolbarButton *exportToolbarButton;
		ToolbarButton *importToolbarButton;
		wxListBox *notesListBox;
		wxTextCtrl *noteTextCtrl;

};

#endif /* NOTESMODULEPANEL_H_ */
