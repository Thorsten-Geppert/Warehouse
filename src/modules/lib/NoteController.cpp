#include "NoteController.h"
#include <DSCgi.h>

NoteController::NoteController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("notes"), // Table
	_N("noteId"),  // Primary key
	_N("title")
) {
}

HtmlPrintoutLibrary *NoteController::Print(NoteEntity *noteEntity) {
	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Note"), rit);

	DSTemplate noteTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("Note"));
	wxString note(HtmlSpecialChars(noteEntity->Get(_N("note"))->ToString()));
	note.Replace(_N("\t"), _N("&nbsp;&nbsp;&nbsp;&nbsp;"));
	noteTemplate.SetVariable(_N("note"), nl2br(note));
	noteTemplate.Parse();

	htmlPrintoutLibrary->SetWithContainer(noteTemplate.Get());

	// Header
	DSTemplate noteHeaderTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("NoteHeader"));
	noteHeaderTemplate.SetVariable(_N("title"), noteEntity->Get(_N("title"))->ToString(), true);
	noteHeaderTemplate.Parse();
	htmlPrintoutLibrary->SetHeader(noteHeaderTemplate.Get());

	// Footer
	DSTemplate noteFooterTemplate = htmlPrintoutLibrary->GetTemplateByDatabase(_N("NoteFooter"));
	noteFooterTemplate.Parse();
	htmlPrintoutLibrary->SetFooter(noteFooterTemplate.Get());

	return htmlPrintoutLibrary;
}
