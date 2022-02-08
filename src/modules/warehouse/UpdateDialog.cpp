#include "UpdateDialog.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(UpdateDialog, wxDialog)
	EVT_CLOSE(UpdateDialog::OnClose)
END_EVENT_TABLE()

UpdateDialog::UpdateDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	UpdateType *updateType
) {
	SetId(id);
	this->rit        = rit;
	this->updateType = updateType;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("UpdateDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("UpdateDialog"));

	mainPanel                   = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer                = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	customerCenterToolbarButton = XRCCTRL(*this, "customerCenterToolbarButton", ToolbarButton);
	closeToolbarButton          = XRCCTRL(*this, "closeToolbarButton", ToolbarButton);
	versionStaticText           = XRCCTRL(*this, "versionStaticText", wxStaticText);
	informationHtmlWindow       = XRCCTRL(*this, "informationHtmlWindow", wxHtmlWindow);

	versionStaticText->SetLabel(_("New version: ") + updateType->GetVersion());
	informationHtmlWindow->SetPage(updateType->GetInformation());

	mainBoxSizer->SetSizeHints(this);
	SetSize(wxSize(640, 480));

	// Events
	CONNECT_TOOLBARBUTTON(closeToolbarButton, UpdateDialog::CloseEvent);
	CONNECT_TOOLBARBUTTON(customerCenterToolbarButton, UpdateDialog::CustomerCenterEvent);

	ICON();
}

// Events
void UpdateDialog::OnClose(wxCloseEvent &event) {
	Destroy();
	event.Skip();
}

void UpdateDialog::CloseEvent(wxMouseEvent &event) {
	Close();
}

void UpdateDialog::CustomerCenterEvent(wxMouseEvent &event) {
	CustomerCenter();
	SKIP();
}

// Methods
void UpdateDialog::Close() {
	wxWindow::Close();
}

void UpdateDialog::CustomerCenter() {
	wxLaunchDefaultBrowser(CUSTOMERCENTER_URL);
}
