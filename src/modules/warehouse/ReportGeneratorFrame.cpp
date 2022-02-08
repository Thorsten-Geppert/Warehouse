#include "ReportGeneratorFrame.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"
#include "PrintPreviewDialog.h"
#include "../lib/ReportLibrary.h"

BEGIN_EVENT_TABLE(ReportGeneratorFrame, wxFrame)
	EVT_MENU(CLOSE_EVENT, ReportGeneratorFrame::CloseKeyEvent)
	EVT_MENU(SAVE_EVENT, ReportGeneratorFrame::SaveKeyEvent)
	EVT_LISTBOX(XRCID("areasListBox"), ReportGeneratorFrame::AreasListBoxEvent)
	EVT_LISTBOX(XRCID("elementsListBox"), ReportGeneratorFrame::ElementsListBoxEvent)
	EVT_LISTBOX_DCLICK(XRCID("placeholdersListBox"), ReportGeneratorFrame::PlaceholdersListBoxEvent)
END_EVENT_TABLE()

ReportGeneratorFrame::ReportGeneratorFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : htmlEditPanel(
	NULL
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ReportGeneratorFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("ReportGeneratorFrame"));

	mainPanel                 = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer              = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	saveToolbarButton         = XRCCTRL(*this, "saveToolbarButton", ToolbarButton);
	closeToolbarButton        = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	areasListBox              = XRCCTRL(*this, "areasListBox", wxListBox);
	elementsListBox           = XRCCTRL(*this, "elementsListBox", wxListBox);
	placeholdersListBox       = XRCCTRL(*this, "placeholdersListBox", wxListBox);
	editPanel                 = XRCCTRL(*this, "editPanel", wxPanel);
	editBoxSizer              = static_cast<wxBoxSizer *>(editPanel->GetSizer());

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(saveToolbarButton, ReportGeneratorFrame::SaveEvent);
	CONNECT_TOOLBARBUTTON(closeToolbarButton, ReportGeneratorFrame::CloseEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,   (int) 's',  SAVE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(800, 600));

	ICON();

	// Filling
	ReportLibrary reportLibrary(rit);
	if(!reportLibrary.LoadFromFile(rit->GRP(_N("lists"), _N("Reports.csv")), reportType)) {
		rit->Log(true, _N("ReportGeneratorFrame"), _("Could not load reports file. Please contact the support."));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not load reports file. Please contact the support."),
			_("Error")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		rit->Log(false, _N("ReportGeneratorFrame"), _("Report files loaded, report generator is ready to use."));
		reportLibrary.FillUp(reportType);

		const unsigned int count = reportType.GetCount();
		for(unsigned int i = 0; i < count; i++)
			areasListBox->Append(reportType.Get(i)->GetArea(), reportType.Get(i));
	}
}

ReportGeneratorFrame::~ReportGeneratorFrame() {
}

void ReportGeneratorFrame::CloseEvent(wxMouseEvent &event) {
	Close();
}

void ReportGeneratorFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void ReportGeneratorFrame::SaveEvent(wxMouseEvent &event) {
	Save();
}

void ReportGeneratorFrame::SaveKeyEvent(wxCommandEvent &event) {
	Save();
}

void ReportGeneratorFrame::Close() {
	Destroy();
}

void ReportGeneratorFrame::AreasListBoxEvent(wxCommandEvent &event) {
	AreasListBox();
}

void ReportGeneratorFrame::ElementsListBoxEvent(wxCommandEvent &event) {
	ElementsListBox();
}

void ReportGeneratorFrame::PlaceholdersListBoxEvent(wxCommandEvent &event) {
	PlaceholdersListBox(event.GetString());
}

void ReportGeneratorFrame::AreasListBox() {
	elementsListBox->Clear();

	ReportAreasType *reportAreasType = static_cast<ReportAreasType *>(areasListBox->GetClientData(areasListBox->GetSelection()));
	if(reportAreasType) {
		const unsigned int count = reportAreasType->GetCount();
		for(unsigned int i = 0; i < count; i++)
			elementsListBox->Append(reportAreasType->Get(i)->GetElement(), reportAreasType->Get(i));
	}
}

void ReportGeneratorFrame::ElementsListBox() {
	placeholdersListBox->Clear();

	ReportElementsType *reportElementsType = static_cast<ReportElementsType *>(elementsListBox->GetClientData(elementsListBox->GetSelection()));
	if(reportElementsType) {
		const unsigned int count = reportElementsType->GetCount();
		for(unsigned int i = 0; i < count; i++)
			placeholdersListBox->Append(reportElementsType->Get(i)->GetPlaceholder(), reportElementsType->Get(i));

		if(!reportElementsType->GetHtmlEditPanel()) {
			reportElementsType->CreateHtmlEditPanel(editPanel, -1, rit);
		}

		SetHtmlEditPanel(reportElementsType->GetHtmlEditPanel());
	}
}

void ReportGeneratorFrame::SetHtmlEditPanel(HtmlEditPanel *htmlEditPanel) {
	if(this->htmlEditPanel) {
		editBoxSizer->Detach(this->htmlEditPanel);
		this->htmlEditPanel->Show(false);
	}
	
	this->htmlEditPanel = htmlEditPanel;
	if(this->htmlEditPanel) {
		editBoxSizer->Prepend(this->htmlEditPanel, 1, wxEXPAND);
		this->htmlEditPanel->Show(true);
	}
	
	editBoxSizer->Layout();
}

void ReportGeneratorFrame::Save() {
	ReportLibrary reportLibrary(rit);
	if(!reportLibrary.Save(reportType)) {
		rit->Log(true, _N("ReportGeneratorFrame"), _("Could not save data. Please contact your administrator."));
		wxMessageDialog *errorMessageDialog = new wxMessageDialog(
			this,
			_("Could not save data. Please contact your administrator."),
			_("Error")
		);
		errorMessageDialog->Center();
		errorMessageDialog->ShowModal();
		errorMessageDialog->Destroy();
	} else {
		rit->Log(false, _N("ReportGeneratorFrame"), _("Reports were saved."));
	}
}

void ReportGeneratorFrame::PlaceholdersListBox(const wxString &item) {
	if(htmlEditPanel)
		htmlEditPanel->WriteText(item);
}
