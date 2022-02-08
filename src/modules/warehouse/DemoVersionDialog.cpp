#include "DemoVersionDialog.h"
#include <wx/xrc/xmlres.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(DemoVersionDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, DemoVersionDialog::CancelKeyEvent)
	EVT_BUTTON(XRCID("closeButton"), DemoVersionDialog::CancelKeyEvent)
	EVT_TIMER(-1, DemoVersionDialog::CounterTimerEvent)
	EVT_CLOSE(DemoVersionDialog::CloseEvent)
END_EVENT_TABLE()

DemoVersionDialog::DemoVersionDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const unsigned int counter
) : counterTimer(
	this
), intCounter(
	counter * 5
) {
	this->counter = counter;

	SetId(id);
	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("DemoVersionDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("DemoVersionDialog"));

	demoVersionPanel    = XRCCTRL(*this, "demoVersionPanel", wxPanel);
	demoVersionBoxSizer = static_cast<wxBoxSizer *>(demoVersionPanel->GetSizer());
	versionStaticText   = XRCCTRL(*this, "versionStaticText", wxStaticText);
	closeButton         = XRCCTRL(*this, "closeButton", wxButton);

	if(intCounter > 0) {
		closeButton->SetLabel(_N("(") + DSCasts::ToString(intCounter) + _N(")"));
		closeButton->Disable();
	}

	versionStaticText->SetLabel(SOFTWARE + _N(" Version ") + VERSION);

	demoVersionBoxSizer->SetSizeHints(this);
	SetSize(wxSize(-1, 650));

	wxAcceleratorEntry acceleratorEntries[1];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	wxAcceleratorTable acceleratorTable(1, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	demoVersionPanel->SetFocus();

	counterTimer.Start(1000);

	ICON();
}

void DemoVersionDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void DemoVersionDialog::CounterTimerEvent(wxTimerEvent &event) {
	CounterTimer();
}

void DemoVersionDialog::CloseEvent(wxCloseEvent &event) {
	if(intCounter > 0)
		event.Veto();
	else
		event.Skip();
}

void DemoVersionDialog::Cancel() {
	Close();
}

void DemoVersionDialog::CounterTimer() {
	if(intCounter > 0) {
		closeButton->SetLabel(_N("(") + DSCasts::ToString(intCounter) + _N(")"));
		intCounter--;
	} else {
		closeButton->SetLabel(_("Close"));
		closeButton->Enable();
	}
}
