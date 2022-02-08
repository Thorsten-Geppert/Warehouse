#include "ExportProgressDialog.h"
#include <DSCasts.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ExportProgressDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, ExportProgressDialog::CancelKeyEvent)
END_EVENT_TABLE()

ExportProgressDialog::ExportProgressDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) : windowDisabler(
	NULL
) {
	SetId(id);
	this->rit = rit;
	this->canceled = false;
	this->count = 0;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ExportProgressDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ExportProgressDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	cancelToolbarButton   = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	informationStaticText = XRCCTRL(*this, "informationStaticText", wxStaticText);
	firstProgressGauge    = XRCCTRL(*this, "firstProgressGauge", wxGauge);
	secondProgressGauge   = XRCCTRL(*this, "secondProgressGauge", wxGauge);
	progressGauge         = firstProgressGauge;

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ExportProgressDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[2];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	wxAcceleratorTable acceleratorTable(2, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(350, -1));

	informationStaticText->SetFocus();

	ICON();
}

void ExportProgressDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ExportProgressDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ExportProgressDialog::Cancel() {
	canceled = true;
}

void ExportProgressDialog::SetGauge(const unsigned int index) {
	gauge = index;

	switch(gauge) {
		case SECOND:
			progressGauge = secondProgressGauge;
			break;
		case FIRST:
		default:
			progressGauge = firstProgressGauge;
			break;
	}
}

unsigned int ExportProgressDialog::GetGauge() const {
	return gauge;
}

void ExportProgressDialog::SetCount(const unsigned int count) {
	this->count = count;
	this->countString = DSCasts::ToString(count);
	progressGauge->SetRange(count);
}

unsigned int ExportProgressDialog::GetCount() const {
	return count;
}

void ExportProgressDialog::SetValue(const unsigned int value) {
	progressGauge->SetValue(value);

	informationStaticText->SetLabel(
		(gauge == FIRST ? _("Preparing ") : _("Item ")) +
		DSCasts::ToString(value) +
		_(" of ") +
		countString +
		_N(".")
	);
	wxYield();
}

unsigned int ExportProgressDialog::GetValue() const {
	return value;
}

bool ExportProgressDialog::IsCanceled() const {
	return canceled;
}

void ExportProgressDialog::Start() {
	windowDisabler = new wxWindowDisabler;
	Show();
}

void ExportProgressDialog::Stop() {
	wxMessageDialog *readyMessageDialog = new wxMessageDialog(
		this,
		_("Finished export."),
		_("Export")
	);
	readyMessageDialog->Center();
	readyMessageDialog->ShowModal();
	readyMessageDialog->Destroy();
	delete windowDisabler;
	windowDisabler = NULL;
	Show(false);
	Destroy();
}
