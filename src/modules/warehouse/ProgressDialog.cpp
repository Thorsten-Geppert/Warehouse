#include "ProgressDialog.h"
#include <DSCasts.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(ProgressDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, ProgressDialog::CancelKeyEvent)
END_EVENT_TABLE()

ProgressDialog::ProgressDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const unsigned int count
) : windowDisabler(
	NULL
) {
	SetId(id);
	this->rit = rit;
	this->canceled = false;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("ProgressDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("ProgressDialog"));

	mainPanel             = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer          = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	cancelToolbarButton   = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	informationStaticText = XRCCTRL(*this, "informationStaticText", wxStaticText);
	progressGauge         = XRCCTRL(*this, "progressGauge", wxGauge);
	SetCount(count);

	mainBoxSizer->SetSizeHints(this);

	CONNECT_TOOLBARBUTTON(cancelToolbarButton, ProgressDialog::CancelEvent);

	wxAcceleratorEntry acceleratorEntries[2];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CANCEL_EVENT);
	wxAcceleratorTable acceleratorTable(2, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(350, -1));

	informationStaticText->SetFocus();

	ICON();
}

void ProgressDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
	SKIP();
}

void ProgressDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void ProgressDialog::Cancel() {
	canceled = true;
}

void ProgressDialog::SetCount(const unsigned int count) {
	this->count = count;
	this->countString = DSCasts::ToString(count);
	progressGauge->SetRange(count);
}

unsigned int ProgressDialog::GetCount() const {
	return count;
}

void ProgressDialog::SetValue(const unsigned int value) {
	this->value = value;
	progressGauge->SetValue(value);
	informationStaticText->SetLabel(
		_("Item ") +
		DSCasts::ToString(value) +
		_(" of ") +
		countString +
		_N(".")
	);
	wxYield();
}

unsigned int ProgressDialog::GetValue() const {
	return value;
}

bool ProgressDialog::IsCanceled() const {
	return canceled;
}

void ProgressDialog::Start() {
	windowDisabler = new wxWindowDisabler;
	Show();
}

void ProgressDialog::Stop() {
	delete windowDisabler;
	windowDisabler = NULL;
	Show(false);
	Destroy();
}
