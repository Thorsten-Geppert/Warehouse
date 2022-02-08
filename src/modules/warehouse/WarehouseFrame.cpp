#include "WarehouseFrame.h"
#include "../lib/ModulePanel.h"
#include "../lib/UserConfigurationFileController.h"
#include "../lib/UserConfigurationFileEntity.h"
//#include "../lib/RpcThread.h"

#include "PreferencesModulePanel.h"
#include "ClientsModulePanel.h"
#include "ProductsModulePanel.h"
#include "OrdersModulePanel.h"
#include "PurchasesModulePanel.h"
#include "VouchersModulePanel.h"
#include "StatisticsModulePanel.h"
#include "FilesModulePanel.h"
#include "NotesModulePanel.h"
#include "YesNoDialog.h"
#include "DemoVersionDialog.h"

#include "Configuration.h"

#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(WarehouseFrame, wxFrame)
	EVT_CLOSE(WarehouseFrame::OnClose)
	EVT_MENU(CLOSE_EVENT, WarehouseFrame::CloseKeyEvent)
	EVT_TIMER(-1, WarehouseFrame::DemoVersionEvent)
	EVT_MENU(PAGEUP_EVENT, WarehouseFrame::PageUpEvent)
	EVT_MENU(PAGEDOWN_EVENT, WarehouseFrame::PageDownEvent)
END_EVENT_TABLE()

WarehouseFrame::WarehouseFrame(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxPoint &pos,
	const wxSize &size
) : wxFrame(
	parent,
	id,
	TITLE,
	pos,
	size
), demoVersionTimer(
	this
), demoCounter(
	0
) {
	/*RpcThread *rpc = new RpcThread(rit);
	if(rpc->Start() != RpcThread::ERROR_NO) {
		printf("ohoh\n");
	} else {
		printf("started\n");
		rpc->Run();
	}*/

	SetTitle(TITLE);
	SetMinSize(wxSize(800, 500));
	this->rit = rit;

	if(rit->IsDemoVersion())
		demoVersionTimer.Start(DEMO_SHOW, true);

	mainPanel    = new wxPanel(this, -1);

	mainBoxSizer = new wxBoxSizer(wxVERTICAL);

	contentPanel = new ContentPanel(mainPanel, -1);
	mainBoxSizer->Add(contentPanel, 1, wxEXPAND);

	// Dock
	ClientsModulePanel *clientsModulePanel         = new ClientsModulePanel(contentPanel, -1, rit);
	ProductsModulePanel *productsModulePanel       = new ProductsModulePanel(contentPanel, -1, rit);
	OrdersModulePanel *ordersModulePanel           = new OrdersModulePanel(contentPanel, -1, rit);
	PurchasesModulePanel *purchasesModulePanel     = new PurchasesModulePanel(contentPanel, -1, rit);
	VouchersModulePanel *vouchersModulePanel       = new VouchersModulePanel(contentPanel, -1, rit);
	StatisticsModulePanel *statisticsModulePanel   = new StatisticsModulePanel(contentPanel, -1, rit);
	NotesModulePanel *notesModulePanel             = new NotesModulePanel(contentPanel, -1, rit);
	FilesModulePanel *filesModulePanel             = new FilesModulePanel(contentPanel, -1, rit);
	PreferencesModulePanel *preferencesModulePanel = new PreferencesModulePanel(contentPanel, -1, rit);

	rit->SetObject(_N("clientsModulePanel"), clientsModulePanel);
	rit->SetObject(_N("productsModulePanel"), productsModulePanel);
	rit->SetObject(_N("ordersModulePanel"), ordersModulePanel);
	rit->SetObject(_N("purchasesModulePanel"), purchasesModulePanel);
	rit->SetObject(_N("vouchersModulePanel"), vouchersModulePanel);
	rit->SetObject(_N("statisticsModulePanel"), statisticsModulePanel);
	rit->SetObject(_N("notesModulePanel"), notesModulePanel);
	rit->SetObject(_N("filesModulePanel"), filesModulePanel);
	rit->SetObject(_N("preferencesModulePanel"), preferencesModulePanel);

	dock = new Dock(mainPanel, contentPanel, -1);

	reminderDockToggleButton = new DockToggleButton(
		dock,
		-1,
		_("Reminders"),
		wxBitmap(rit->GRP(_N("images"), _N("Reminder.png")), wxBITMAP_TYPE_PNG)
	);
	reminderDockToggleButton->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(WarehouseFrame::ReminderEvent), NULL, this);

	dock->Add(reminderDockToggleButton, 0, 50);
	dock->Add(new DockButton(dock, -1, _("Clients"),     wxBitmap(rit->GRP(_N("images"), _N("Clients.png")),      wxBITMAP_TYPE_PNG), clientsModulePanel));
	dock->Add(new DockButton(dock, -1, _("Products"),    wxBitmap(rit->GRP(_N("images"), _N("Products.png")),     wxBITMAP_TYPE_PNG), productsModulePanel));
	dock->Add(new DockButton(dock, -1, _("Orders"),      wxBitmap(rit->GRP(_N("images"), _N("Orders.png")),       wxBITMAP_TYPE_PNG), ordersModulePanel));
	dock->Add(new DockButton(dock, -1, _("Purchases"),   wxBitmap(rit->GRP(_N("images"), _N("Purchases.png")),    wxBITMAP_TYPE_PNG), purchasesModulePanel));
	dock->Add(new DockButton(dock, -1, _("Vouchers"),    wxBitmap(rit->GRP(_N("images"), _N("Vouchers.png")),     wxBITMAP_TYPE_PNG), vouchersModulePanel));
	dock->Add(new DockButton(dock, -1, _("Statistics"),  wxBitmap(rit->GRP(_N("images"), _N("Statistics.png")),   wxBITMAP_TYPE_PNG), statisticsModulePanel));
	dock->Add(new DockButton(dock, -1, _("Notes"),       wxBitmap(rit->GRP(_N("images"), _N("Notes.png")),        wxBITMAP_TYPE_PNG), notesModulePanel));
	dock->Add(new DockButton(dock, -1, _("Files"),       wxBitmap(rit->GRP(_N("images"), _N("Files.png")),        wxBITMAP_TYPE_PNG), filesModulePanel));
	dock->Add(new DockButton(dock, -1, _("Preferences"), wxBitmap(rit->GRP(_N("images"), _N("Preferences.png")),  wxBITMAP_TYPE_PNG), preferencesModulePanel));
	mainBoxSizer->Add(dock, 0, wxEXPAND);
	dock->Change(1);

	mainPanel->SetSizer(mainBoxSizer);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_CTRL, (int) 'q', CLOSE_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL, WXK_PAGEUP, PAGEUP_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL, WXK_PAGEDOWN, PAGEDOWN_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	reminderDialog = NULL;

	ICON();
}

void WarehouseFrame::CloseKeyEvent(wxCommandEvent &event) {
	Close();
}

void WarehouseFrame::OnClose(wxCloseEvent &event) {
	YesNoDialog *yesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to quit Warehouse?"),
		_("Quit"),
		_("Quit"),
		_("Cancel")
	);
	yesNoDialog->Center();
	const bool ok = yesNoDialog->ShowModal() == wxID_YES;
	yesNoDialog->Destroy();
	if(!ok)
		return;

	rit->GetUserConfigurationFileEntity()->GetMainWindowPreferences()->SetSize(GetSize());
	rit->GetUserConfigurationFileEntity()->GetMainWindowPreferences()->SetPosition(GetPosition());

	if(!UserConfigurationFileController::Save(
		rit->GetUserConfigurationFileEntity(),
		rit->GetUserConfigurationFileWithPath(),
		UserConfigurationFileController::ALL
	)) {
		rit->Log(true, _N("WarehouseFrame"), _("Could not save configuration file. Please correct the permissions."));
		wxMessageDialog *messageDialog = new wxMessageDialog(
			this,
			_("Could not save configuration file. Please correct the permissions. You will be logged in now."),
			_("Save configuration file")
		);
		messageDialog->Center();
		messageDialog->ShowModal();
		messageDialog->Destroy();
	}
	event.Skip();
}

void WarehouseFrame::ReminderEvent(wxMouseEvent &event) {
	Reminder();
	event.Skip();
}

void WarehouseFrame::DemoVersionEvent(wxTimerEvent &event) {
	DemoVersion();
}

void WarehouseFrame::PageUpEvent(wxCommandEvent &event) {
	Page(false);
}

void WarehouseFrame::PageDownEvent(wxCommandEvent &event) {
	Page(true);
}

void WarehouseFrame::Reminder(const bool showAndRaise) {
	if(!rit->Access(A_WRITE, _("You have no permission to open reminders."), this))
		return;

	if(!reminderDialog) {
		reminderDialog = new ReminderDialog(this, -1, rit);
		reminderDialog->Center();
	}

	if(showAndRaise) {
		if(!reminderDialog->IsShown())
			reminderDialog->Show();
		else
			reminderDialog->Raise();
	} else {
		reminderDialog->Show(!reminderDialog->IsShown());
	}
	reminderDockToggleButton->SetButton(reminderDialog->IsShown());
}

void WarehouseFrame::DemoVersion() {
	DemoVersionDialog *demoVersionDialog = new DemoVersionDialog(
		this,
		-1,
		rit,
		//++demoCounter
		demoCounter
	);
	demoVersionDialog->Center();
	demoVersionDialog->ShowModal();
	demoVersionDialog->Destroy();

	demoVersionTimer.Start(DEMO_SHOW, true);
}

void WarehouseFrame::Page(const bool next) {
	const int index = dock->GetDockButtons()->Find(dock->GetPressedButton());

	if(index != wxNOT_FOUND) {
		if(next && static_cast<unsigned int>(index + 1) <= dock->GetDockButtons()->GetCount()) 
			dock->Change(index + 1);
		else if(index - 1 > 0)
			dock->Change(index - 1);
	}
}
