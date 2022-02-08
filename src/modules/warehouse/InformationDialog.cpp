#include "InformationDialog.h"
#include <wx/xrc/xmlres.h>
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(InformationDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, InformationDialog::CancelKeyEvent)
END_EVENT_TABLE()

InformationDialog::InformationDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("InformationDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("InformationDialog"));

	informationPanel    = XRCCTRL(*this, "informationPanel", wxPanel);
	informationBoxSizer = static_cast<wxBoxSizer *>(informationPanel->GetSizer());
	gtcHtmlWindow       = XRCCTRL(*this, "gtcHtmlWindow", wxHtmlWindow);
	versionStaticText   = XRCCTRL(*this, "versionStaticText", wxStaticText);

	gtcHtmlWindow->SetPage(GTC);
	versionStaticText->SetLabel(SOFTWARE + _N(" Version ") + VERSION);

	informationBoxSizer->SetSizeHints(this);
	SetSize(wxSize(-1, 700));

	wxAcceleratorEntry acceleratorEntries[1];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	wxAcceleratorTable acceleratorTable(1, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	informationPanel->SetFocus();

	ICON();
}

void InformationDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void InformationDialog::Cancel() {
	Close();
}
