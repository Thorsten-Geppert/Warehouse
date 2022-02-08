#include "JasperReportsSelectionDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(JasperReportsSelectionDialog, wxDialog)
	EVT_MENU(CLOSE_EVENT, JasperReportsSelectionDialog::CancelKeyEvent)
	EVT_MENU(USE_EVENT, JasperReportsSelectionDialog::UseKeyEvent)
	EVT_COMBOBOX(XRCID("reportComboBox"), JasperReportsSelectionDialog::ChangeReportEvent)
END_EVENT_TABLE()

JasperReportsSelectionDialog::JasperReportsSelectionDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	JasperReportsEntities &jasperReportsEntities
) {
	SetId(id);
	this->rit                   = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("JasperReportsSelectionDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("JasperReportsSelectionDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	mainToolbar           = XRCCTRL(*this, "mainToolbar", Toolbar);
	cancelToolbarButton   = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	useToolbarButton     = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	reportStaticText      = XRCCTRL(*this, "reportStaticText", wxStaticText);
	reportComboBox        = XRCCTRL(*this, "reportComboBox", wxComboBox);
	descriptionStaticText = XRCCTRL(*this, "descriptionStaticText", wxStaticText);
	descriptionTextCtrl   = XRCCTRL(*this, "descriptionTextCtrl", wxTextCtrl);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(300, -1));

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, JasperReportsSelectionDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(useToolbarButton, JasperReportsSelectionDialog::UseEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CLOSE_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 's',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();

	JasperReportsEntity *jasperReportsEntity = NULL;
	const unsigned int count = jasperReportsEntities.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		jasperReportsEntity = jasperReportsEntities.Get(i);
		reportComboBox->Append(
			jasperReportsEntity->Get(_N("report"))->ToString(),
			jasperReportsEntity
		);
	}

	reportComboBox->SetSelection(0);
	ChangeReport();
}

void JasperReportsSelectionDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void JasperReportsSelectionDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

void JasperReportsSelectionDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void JasperReportsSelectionDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void JasperReportsSelectionDialog::ChangeReportEvent(wxCommandEvent &event) {
	ChangeReport();
}

void JasperReportsSelectionDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

void JasperReportsSelectionDialog::Use() {
	EndModal(wxID_OK);
}

JasperReportsEntity *JasperReportsSelectionDialog::GetSelectedEntry() {
	return static_cast<JasperReportsEntity *>(
		reportComboBox->GetClientData(reportComboBox->GetSelection())
	);
}

void JasperReportsSelectionDialog::ChangeReport() {
	descriptionTextCtrl->SetValue(
		static_cast<JasperReportsEntity *>(reportComboBox->GetClientData(reportComboBox->GetSelection()))->Get(_N("description"))->ToString()
	);
}
