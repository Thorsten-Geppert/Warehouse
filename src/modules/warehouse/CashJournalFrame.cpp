#include "CashJournalFrame.h"
#include "resources/images/WarehouseIcon.xpm"
#include "Configuration.h"

BEGIN_EVENT_TABLE(CashJournalFrame, wxFrame)
	EVT_MENU(CLOSE_EVENT, CashJournalFrame::CloseKeyEvent)
	EVT_CLOSE(CashJournalFrame::HideEvent)
END_EVENT_TABLE()

CashJournalFrame::CashJournalFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit
) {
	SetId(id);
	this->rit = rit;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("CashJournalFrame.xml")));
	wxXmlResource::Get()->LoadFrame(this, parent, _N("CashJournalFrame"));

	mainPanel               = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer            = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	closeToolbarButton      = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);

	//monitionsListCtrl->InsertColumn(0, _("Pos."), wxLIST_FORMAT_RIGHT, 50);

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(950, 600));

	CONNECT_TOOLBARBUTTON(closeToolbarButton, CashJournalFrame::CloseEvent);

	wxAcceleratorEntry acceleratorEntries[2];
	acceleratorEntries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,   (int) 'w',  CLOSE_EVENT);
	wxAcceleratorTable acceleratorTable(2, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	ICON();
}

CashJournalFrame::~CashJournalFrame() {
}

void CashJournalFrame::CloseEvent(wxMouseEvent &event) {
	Close();
	SKIP();
}

void CashJournalFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void CashJournalFrame::HideEvent(wxCloseEvent &event) {
	Close();
}

void CashJournalFrame::Close() {
	Hide();
}
