#include "ExportDialog.h"
#include <wx/filedlg.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ExportDialog, wxDialog)
	EVT_MENU(RUN_EVENT, ExportDialog::RunKeyEvent)
	EVT_BUTTON(XRCID("browseButton"), ExportDialog::BrowseEvent)
	EVT_MENU(BROWSE_EVENT, ExportDialog::BrowseKeyEvent)
	EVT_MENU(CANCEL_EVENT, ExportDialog::CancelKeyEvent)
END_EVENT_TABLE()

ExportDialog::ExportDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	ExportType *exportType
) {
	SetId(id);
	this->rit = rit;
	this->exportType = exportType;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ExportDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ExportDialog"));

	mainPanel                   = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	runToolbarButton            = XRCCTRL(*this, "runToolbarButton", ToolbarButton);
	cancelToolbarButton         = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	fileTextCtrl                = XRCCTRL(*this, "fileTextCtrl", wxTextCtrl);
	fieldDelimiterComboBox      = XRCCTRL(*this, "fieldDelimiterComboBox", wxComboBox);
	textDelimiterComboBox       = XRCCTRL(*this, "textDelimiterComboBox", wxComboBox);
	lineFeedComboBox            = XRCCTRL(*this, "lineFeedComboBox", wxComboBox);
	replaceLineFeedCheckBox     = XRCCTRL(*this, "replaceLineFeedCheckBox", wxCheckBox);
	replaceLineFeedWithTextCtrl = XRCCTRL(*this, "replaceLineFeedWithTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(runToolbarButton, ExportDialog::RunEvent);
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ExportDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[4];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 'r',  RUN_EVENT);
	acceleratorEntries[3].Set(wxACCEL_CTRL,   (int) 'b',  BROWSE_EVENT);
	wxAcceleratorTable acceleratorTable(4, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(650, -1));

	fileTextCtrl->SetFocus();

	if(exportType) {
		fileTextCtrl->SetValue(exportType->GetFilename());
		textDelimiterComboBox->SetValue(exportType->GetTextDelimiter());
		fieldDelimiterComboBox->SetValue(exportType->GetFieldDelimiter());
		lineFeedComboBox->SetSelection(exportType->GetLineFeed());
		replaceLineFeedCheckBox->SetValue(exportType->GetReplaceLineFeed());
		replaceLineFeedWithTextCtrl->SetValue(exportType->GetReplaceLineFeedWith());
	}

	ICON();
}

void ExportDialog::RunEvent(wxMouseEvent &event) {
	Run();
	SKIP();
}

void ExportDialog::RunKeyEvent(wxCommandEvent &event) {
	Run();
}

void ExportDialog::BrowseEvent(wxCommandEvent &event) {
	Browse();
}

void ExportDialog::BrowseKeyEvent(wxCommandEvent &event) {
	Browse();
}

void ExportDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ExportDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ExportDialog::Run() {
	EndModal(wxID_OK);
}

void ExportDialog::Browse() {
	wxFileDialog *browseFileDialog = new wxFileDialog(
		this,
		_("Choose a file"),
		fileTextCtrl->GetValue(),
		_N("export.csv"),
		_N("*.csv"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT
	);
	browseFileDialog->Center();
	if(browseFileDialog->ShowModal() == wxID_OK)
		fileTextCtrl->SetValue(browseFileDialog->GetPath());
	browseFileDialog->Destroy();
}

void ExportDialog::Cancel() {
	EndModal(wxCANCEL);
}

ExportType *ExportDialog::GetExportType() const {
	return new ExportType(
		fileTextCtrl->GetValue(),
		fieldDelimiterComboBox->GetValue(),
		textDelimiterComboBox->GetValue(),
		lineFeedComboBox->GetSelection(),
		replaceLineFeedWithTextCtrl->GetValue(),
		replaceLineFeedCheckBox->GetValue()
	);
}

bool ExportDialog::TransferToExportType() const {
	if(!exportType)
		return false;
	
	exportType->SetFilename(fileTextCtrl->GetValue());
	exportType->SetFieldDelimiter(fieldDelimiterComboBox->GetValue());
	exportType->SetTextDelimiter(textDelimiterComboBox->GetValue());
	exportType->SetLineFeed(lineFeedComboBox->GetSelection());
	exportType->SetReplaceLineFeed(replaceLineFeedCheckBox->GetValue());
	exportType->SetReplaceLineFeedWith(replaceLineFeedWithTextCtrl->GetValue());

	return true;
}
