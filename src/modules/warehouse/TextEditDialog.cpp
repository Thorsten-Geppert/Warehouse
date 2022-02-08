#include "TextEditDialog.h"
#include "Configuration.h"
#include "PrintPreviewDialog.h"
#include "YesNoDialog.h"
#include "resources/images/WarehouseIcon.xpm"
#include <DSFile.h>
#include <wx/clipbrd.h>

BEGIN_EVENT_TABLE(TextEditDialog, wxDialog)
	EVT_CLOSE(TextEditDialog::OnClose)
	EVT_MENU(SAVE_EVENT, TextEditDialog::SaveKeyEvent)
	EVT_MENU(PRINT_EVENT, TextEditDialog::PrintKeyEvent)
	EVT_MENU(PRINT_PREVIEW_EVENT, TextEditDialog::PrintPreviewKeyEvent)
	EVT_MENU(COPY_EVENT, TextEditDialog::CopyKeyEvent)
	EVT_MENU(CUT_EVENT, TextEditDialog::CutKeyEvent)
	EVT_MENU(PASTE_EVENT, TextEditDialog::PasteKeyEvent)
	EVT_MENU(IMPORT_EVENT, TextEditDialog::ImportKeyEvent)
	EVT_MENU(EXPORT_EVENT, TextEditDialog::ExportKeyEvent)
	EVT_MENU(CANCEL_EVENT, TextEditDialog::CancelKeyEvent)
END_EVENT_TABLE()

TextEditDialog::TextEditDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	bool *changed,
	wxString *value
) {
	this->rit          = rit;
	this->changed      = changed;
	this->value        = value;
	this->innerChanged = false;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("TextEditDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("TextEditDialog"));

	mainPanel                 = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer              = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar               = XRCCTRL(*this, "mainToolbar", Toolbar);	
	saveToolbarButton         = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	printToolbarButton        = XRCCTRL(*this, "printToolbarButton", ToolbarButton);
	printPreviewToolbarButton = XRCCTRL(*this, "printPreviewToolbarButton", ToolbarButton);
	copyToolbarButton         = XRCCTRL(*this, "copyToolbarButton", ToolbarButton);
	cutToolbarButton          = XRCCTRL(*this, "cutToolbarButton", ToolbarButton);
	pasteToolbarButton        = XRCCTRL(*this, "pasteToolbarButton", ToolbarButton);
	importToolbarButton       = XRCCTRL(*this, "importToolbarButton", ToolbarButton);
	exportToolbarButton       = XRCCTRL(*this, "exportToolbarButton", ToolbarButton);
	cancelToolbarButton       = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	informationTextCtrl       = XRCCTRL(*this, "informationTextCtrl", DSTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(saveToolbarButton, TextEditDialog::SaveEvent);
	CONNECT_TOOLBARBUTTON(printToolbarButton, TextEditDialog::PrintEvent);
	CONNECT_TOOLBARBUTTON(printPreviewToolbarButton, TextEditDialog::PrintPreviewEvent);
	CONNECT_TOOLBARBUTTON(copyToolbarButton, TextEditDialog::CopyEvent);
	CONNECT_TOOLBARBUTTON(cutToolbarButton, TextEditDialog::CutEvent);
	CONNECT_TOOLBARBUTTON(pasteToolbarButton, TextEditDialog::PasteEvent);
	CONNECT_TOOLBARBUTTON(importToolbarButton, TextEditDialog::ImportEvent);
	CONNECT_TOOLBARBUTTON(exportToolbarButton, TextEditDialog::ExportEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, TextEditDialog::CancelEvent);
	
	wxAcceleratorEntry acceleratorEntries[10];
	acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 's', SAVE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, (int) 'p', PRINT_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL | wxACCEL_SHIFT, (int) 'p', PRINT_PREVIEW_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL, (int) 'c', COPY_EVENT);
	acceleratorEntries[4].Set(wxACCEL_CTRL, (int) 'x', CUT_EVENT);
	acceleratorEntries[5].Set(wxACCEL_CTRL, (int) 'v', PASTE_EVENT);
	acceleratorEntries[6].Set(wxACCEL_CTRL, (int) 'i', IMPORT_EVENT);
	acceleratorEntries[7].Set(wxACCEL_CTRL, (int) 'e', EXPORT_EVENT);
	acceleratorEntries[8].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[9].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	wxAcceleratorTable acceleratorTable(10, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(1000, 600));

	if(value)
		informationTextCtrl->SetValue(*value);

	informationTextCtrl->SetFocus();
	informationTextCtrl->SetChange(&innerChanged);

	ICON();
}

// Events
void TextEditDialog::SaveEvent(wxMouseEvent &event) {
	Save();
	SKIP();
}

void TextEditDialog::CopyEvent(wxMouseEvent &event) {
	Copy();
	SKIP();
}

void TextEditDialog::CutEvent(wxMouseEvent &event) {
	Cut();
	SKIP();
}

void TextEditDialog::PasteEvent(wxMouseEvent &event) {
	Paste();
	SKIP();
}

void TextEditDialog::ImportEvent(wxMouseEvent &event) {
	Import();
	SKIP();
}

void TextEditDialog::ExportEvent(wxMouseEvent &event) {
	Export();
	SKIP();
}

void TextEditDialog::PrintPreviewEvent(wxMouseEvent &event) {
	Print(true);
	SKIP();
}

void TextEditDialog::PrintEvent(wxMouseEvent &event) {
	Print();
	SKIP();
}

void TextEditDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

// Key Events
void TextEditDialog::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void TextEditDialog::CopyKeyEvent(wxCommandEvent &event) {
	Copy();
}

void TextEditDialog::CutKeyEvent(wxCommandEvent &event) {
	Cut();
}

void TextEditDialog::PasteKeyEvent(wxCommandEvent &event) {
	Paste();
}

void TextEditDialog::ImportKeyEvent(wxCommandEvent &event) {
	Import();
}

void TextEditDialog::ExportKeyEvent(wxCommandEvent &event) {
	Export();
}

void TextEditDialog::PrintPreviewKeyEvent(wxCommandEvent &event) {
	Print(true);
}

void TextEditDialog::PrintKeyEvent(wxCommandEvent &event) {
	Print();
}

void TextEditDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

// Methods
void TextEditDialog::OnClose(wxCloseEvent &event) {
	bool ok = true;
	if(innerChanged) {
		YesNoDialog *yesNoDialog = new YesNoDialog(
			this,
			-1,
			rit,
			_("You have changed the data. Do you want to use the changes?"),
			_("Close"),
			_("Save"),
			_("Cancel")
		);
		yesNoDialog->Center();
		if(yesNoDialog->ShowModal() == wxID_YES)
			ok = Save();
		yesNoDialog->Destroy();
	}

	if(ok) {
		event.Skip();
	} else {
		event.Veto();
	}

	innerChanged = false;
}

bool TextEditDialog::Save() {
	if(!rit->Access(A_WRITE, _("You have no permission to save this text."), this))
		return false;

	if(value)
		*value = informationTextCtrl->GetValue();

	informationTextCtrl->SetFocus();
	
	if(changed)
		*changed = true;
	else
		return false;

	return true;
}

void TextEditDialog::Copy() {
	long selectionFrom = 0, selectionTo = 0;
	informationTextCtrl->GetSelection(&selectionFrom, &selectionTo);
	informationTextCtrl->Copy();
	informationTextCtrl->SetFocus();
	informationTextCtrl->SetSelection(selectionFrom, selectionTo);
}

void TextEditDialog::Cut() {
	long selectionFrom = 0, selectionTo = 0;
	informationTextCtrl->GetSelection(&selectionFrom, &selectionTo);
	informationTextCtrl->Cut();
	informationTextCtrl->SetFocus();
	informationTextCtrl->SetSelection(0, 0);
	informationTextCtrl->SetInsertionPoint(selectionFrom);
}

void TextEditDialog::Paste() {
	wxString clipboard;

	if(wxTheClipboard->Open()) {
		if(wxTheClipboard->IsSupported(wxDF_TEXT)) {
			wxTextDataObject data;
			wxTheClipboard->GetData(data);
			clipboard = data.GetText();
		}

		wxTheClipboard->Close();
	}

	if(clipboard.IsEmpty())
		return;

	long selectionFrom = 0, selectionTo = 0;
	informationTextCtrl->GetSelection(&selectionFrom, &selectionTo);
	if(selectionFrom != selectionTo)
		informationTextCtrl->Remove(selectionFrom, selectionTo);
	const long insertionPoint = informationTextCtrl->GetInsertionPoint();
	informationTextCtrl->Paste();
	informationTextCtrl->SetFocus();
	informationTextCtrl->SetInsertionPoint(insertionPoint + clipboard.Length());
}

void TextEditDialog::Import() {
	informationTextCtrl->SetFocus();

	if(!rit->Access(A_WRITE, _("You have no permission to import text."), this))
		return;

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
					_("Text edit")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			} else {
				informationTextCtrl->SetValue(tmp);
			}
			
			if(!file.Close()) {
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not close file."),
					_("Text exit")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			}
		}
	}
	browseFileDialog->Destroy();
	informationTextCtrl->SetSelection(0, 0);
}

void TextEditDialog::Export() {
	long selectionFrom = 0, selectionTo = 0;
	informationTextCtrl->GetSelection(&selectionFrom, &selectionTo);

	if(!rit->Access(A_READ, _("You have no permission to export text."), this))
		return;

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
			if(!file.Write(informationTextCtrl->GetValue())) {
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not write file."),
					_("Text edit")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			}
			if(!file.Close()) {
				wxMessageDialog *errorMessage = new wxMessageDialog(
					this,
					_("Could not close file."),
					_("Text edit")
				);
				errorMessage->Center();
				errorMessage->ShowModal();
				errorMessage->Destroy();
			}
		} else {
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not open file."),
				_("Text edit")
			);
			errorMessage->Center();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		}
	}
	browseFileDialog->Destroy();
	informationTextCtrl->SetFocus();
	informationTextCtrl->SetSelection(selectionFrom, selectionTo);
}

void TextEditDialog::Print(const bool preview) {
	informationTextCtrl->SetFocus();

	if(!rit->Access(A_READ, _("You have no permission to print."), this))
		return;
	
	//changed = false;
	if(*changed) {
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Save first this entry and press print again. Only saved entries can be printed."),
			_("Print")
		);
		errorMessage->Center();
		errorMessage->ShowModal();
		errorMessage->Destroy();
		return;
	}

	HtmlPrintoutLibrary *htmlPrintoutLibrary = new HtmlPrintoutLibrary(_("Informationen"), rit);

	DSTemplate informationTemplate = htmlPrintoutLibrary->GetTemplateByFile(_N("Informationen.tpl"));

	wxString note(DSLib::HtmlSpecialChars(informationTextCtrl->GetValue()));
	note.Replace(_N("\t"), _N("&nbsp;&nbsp;&nbsp;&nbsp;"));
	informationTemplate.SetVariable(
		_N("text"),
		DSLib::nl2br(note)
	);
	informationTemplate.Parse();
	htmlPrintoutLibrary->SetWithContainer(informationTemplate.Get());


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

void TextEditDialog::Cancel() {
	Close();
}

wxString TextEditDialog::GetValue() const {
	return informationTextCtrl->GetValue();
}

void TextEditDialog::SetValue(const wxString &value) {
	informationTextCtrl->SetValue(value);
}

void TextEditDialog::ResetValue() {
	informationTextCtrl->ChangeValue(*this->value);
	informationTextCtrl->SetInsertionPoint(0);
}

DSTextCtrl *TextEditDialog::GetTextCtrl() const {
	return informationTextCtrl;
}
