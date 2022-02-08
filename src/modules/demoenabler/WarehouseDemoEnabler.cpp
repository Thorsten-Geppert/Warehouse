#include "WarehouseDemoEnabler.h"
#include <wx/dirdlg.h>
#include <wx/filename.h>
#include <wx/platinfo.h>
#include <wx/config.h>
#include <wx/aboutdlg.h>

IMPLEMENT_APP(WarehouseDemoEnabler)

bool WarehouseDemoEnabler::OnInit() {
	wxInitAllImageHandlers();

	WarehouseDemoEnablerFrame *warehouseDemoEnablerFrame = new WarehouseDemoEnablerFrame;
	warehouseDemoEnablerFrame->Center();
	warehouseDemoEnablerFrame->Show();
	SetTopWindow(warehouseDemoEnablerFrame);

	return true;
}

BEGIN_EVENT_TABLE(WarehouseDemoEnablerFrame, wxFrame)
	EVT_BUTTON(BROWSE_EVENT, WarehouseDemoEnablerFrame::BrowseEvent)
	EVT_BUTTON(ENABLE_EVENT, WarehouseDemoEnablerFrame::EnableEvent)
	EVT_BUTTON(INFO_EVENT, WarehouseDemoEnablerFrame::InfoEvent)
END_EVENT_TABLE()

WarehouseDemoEnablerFrame::WarehouseDemoEnablerFrame(
) : wxFrame(
	NULL,
	-1,
	_("DynSoft.com - Warehouse Demo Enabler"),
	wxDefaultPosition,
	wxDefaultSize,
	wxCAPTION | wxCLOSE_BOX | wxMINIMIZE_BOX | wxSYSTEM_MENU
) {
	pathSeparator = wxFileName::GetPathSeparator();

	mainPanel = new wxPanel(this);
	mainBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	logoStaticBitmap = new wxStaticBitmap(
		mainPanel,
		-1,
		wxBitmap(
#ifdef __WXMAC__
			_("WarehouseDemoEnabler.app/Contents/Resources/Logo.png"),
#else
			_("Logo.png"),
#endif
			wxBITMAP_TYPE_PNG
		)
	);
	mainBoxSizer->Add(logoStaticBitmap);

	borderBoxSizer = new wxBoxSizer(wxVERTICAL);
	dataBoxSizer = new wxBoxSizer(wxVERTICAL);
	
	headlineStaticText = new wxStaticText(mainPanel, -1, _("DynSoft.com - Warehouse Demo Enabler"));
	headlineStaticText->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	dataBoxSizer->Add(headlineStaticText);

	headlineStaticLine = new wxStaticLine(mainPanel, -1);
	dataBoxSizer->Add(headlineStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	infoStaticText = new wxStaticText(
		mainPanel,
		-1,
		_(
			"Nutzen Sie die Software, um die Demoversion von DynSoft.com Warehouse\n"
			"ab heute zu beginnen. Diese Aktion funktioniert nur einmal.\n"
			"Sie nutzen die Software auf eigenes Risiko. Wir schliessen die Haftung\n"
			"jeglicher Art aus."
		)
	);
	dataBoxSizer->Add(infoStaticText);

	infoStaticLine = new wxStaticLine(mainPanel, -1);
	dataBoxSizer->Add(infoStaticLine, 0, wxEXPAND | wxTOP | wxBOTTOM, 5);

	pathStaticText = new wxStaticText(mainPanel, -1, _("Bitte tragen Sie den Pfad ein, in dem Sie DynSoft.com Warehouse installiert haben:"));
	dataBoxSizer->Add(pathStaticText, 0, wxBOTTOM, 5);

	pathBoxSizer = new wxBoxSizer(wxHORIZONTAL);

	pathTextCtrl = new wxTextCtrl(mainPanel, -1, wxEmptyString);
	pathBoxSizer->Add(pathTextCtrl, 1, wxALIGN_CENTER_VERTICAL | wxRIGHT, 5);

	pathButton = new wxButton(mainPanel, BROWSE_EVENT, _("Durchsuchen ..."));
	pathBoxSizer->Add(pathButton, 0, wxALIGN_CENTER_VERTICAL);

	dataBoxSizer->Add(pathBoxSizer, 0, wxEXPAND);

	enableButton = new wxButton(mainPanel, ENABLE_EVENT, _("Demoversion freischalten"));
	enableButton->SetFont(wxFont(enableButton->GetFont().GetPointSize(), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD));
	dataBoxSizer->Add(enableButton, 0, wxALIGN_CENTER | wxTOP, 5);

	infoButton = new wxButton(mainPanel, INFO_EVENT, _("?"));
	dataBoxSizer->Add(infoButton, 0, wxALIGN_RIGHT);

	borderBoxSizer->Add(dataBoxSizer, 1, wxEXPAND | wxALL, 15);
	mainBoxSizer->Add(borderBoxSizer, 1, wxEXPAND);

	mainPanel->SetSizer(mainBoxSizer);
	mainBoxSizer->SetSizeHints(this);

	wxString path;
	wxPlatformInfo platformInfo;
	switch(platformInfo.GetOperatingSystemId()) {
		case wxOS_MAC_OSX_DARWIN:
			path = _("/Applications/");
			break;
		case wxOS_WINDOWS_NT:
		{
			switch(platformInfo.GetArchitecture()) {
				case wxARCH_32:
					path = _("C:\\Program Files\\DynSoft.com Warehouse\\");
					break;
				case wxARCH_64:
					path = _("C:\\Program Files (x86)\\DynSoft.com Warehouse\\");
					break;
			}
			break;
		}
		case wxOS_UNIX_FREEBSD:
			path = _("/usr/local/");
			break;
		case wxOS_UNIX_LINUX:
			path = _("/usr/share/warehouse/");
			break;
	}

	pathTextCtrl->SetValue(path);

#ifdef __WINDOWS__
	SetIcon(wxIcon(_("WarehouseIcon.ico"), wxBITMAP_TYPE_ICO));
#else
	#ifdef __WXMAC__
		SetIcon(wxIcon(_("WarehouseDemoEnabler.app/Contents/Resources/WarehouseIcon.png"), wxBITMAP_TYPE_PNG));
	#else
		SetIcon(wxIcon(_("WarehouseIcon.png"), wxBITMAP_TYPE_PNG));
	#endif
#endif
}

void WarehouseDemoEnablerFrame::BrowseEvent(wxCommandEvent &event) {
	Browse();
}

void WarehouseDemoEnablerFrame::EnableEvent(wxCommandEvent &event) {
	Enable();
}

void WarehouseDemoEnablerFrame::InfoEvent(wxCommandEvent &event) {
	Info();
}

void WarehouseDemoEnablerFrame::Browse() {
	wxString path(pathTextCtrl->GetValue());

	wxDirDialog *warehouseDirDialog = new wxDirDialog(
		this,
		_("Pfad selektieren, in dem DynSoft.com Warehouse installiert ist"),
		path,
		wxDD_DIR_MUST_EXIST
	);
	warehouseDirDialog->Center();
	if(warehouseDirDialog->ShowModal() == wxID_OK) {
		pathTextCtrl->SetValue(warehouseDirDialog->GetPath());
	}
	warehouseDirDialog->Destroy();
}

void WarehouseDemoEnablerFrame::Enable() {
	bool ok = false;
	wxString message(_("Die Demoversion wurde aktiviert. Das Programm wird jetzt beendet. Starten Sie danach DynSoft.com Warehouse."));

	wxString path(pathTextCtrl->GetValue());
	if(path.IsEmpty()) {
		message = _("Bitte tragen Sie den Pfad ein, in dem DynSoft.com Warehouse installiert ist.");
	} else {
		if(!ExistsExecutableFile(path, &path)) {
			message = _("Die zur Demoversion heraufzustufende Datei existiert nicht in dem angegebenen Pfad.");
		} else {
			wxConfig *config = new wxConfig(_("DynSoftCom.Warehouse"));
			
			long version = 0;
			config->Read(_("version"), &version);
			if(version != 0) {
				message = _("Sie haben die Demoversion von DynSoft.com Warehouse bereits einmal aktiviert. Die Aktivierung funktioniert nur einmal.");
			} else {
				wxDateTime now(wxDateTime::Now());
				
				wxFileName fileName(path);
				if(!fileName.SetTimes(&now, &now, &now)) {
					message = _("Die Demoversion von DynSoft.com Warehouse konnte nicht aktiviert werden. Haben Sie Administratorrechte?");
				} else {
					config->Write(_("version"), 1);
					ok = true;
				}
			}

			delete config;
		}
	}

	wxMessageDialog *messageDialog = new wxMessageDialog(this, message, _("Hinweis"));
	messageDialog->Center();
	messageDialog->ShowModal();
	messageDialog->Destroy();

	if(ok)
		Close();
}

bool WarehouseDemoEnablerFrame::ExistsExecutableFile(wxString path, wxString *fullPath) {
	if(!path.EndsWith(pathSeparator))
		path.Append(pathSeparator);

	wxPlatformInfo platformInfo;
	switch(platformInfo.GetOperatingSystemId()) {
		case wxOS_MAC_OSX_DARWIN:
			path.Append(_("Warehouse.app/Contents/MacOS/Warehouse"));
			break;
		case wxOS_WINDOWS_NT:
			path.Append(_("Warehouse.exe"));
			break;
		case wxOS_UNIX_FREEBSD:
			path.Append(_("Warehouse"));
			break;
		case wxOS_UNIX_LINUX:
			path.Append(_("warehouse"));
			break;
	}

	if(fullPath)
		*fullPath = path;
	
	return wxFileName::FileExists(path);
}

void WarehouseDemoEnablerFrame::Info() {
	wxAboutDialogInfo aboutDialogInfo;

	aboutDialogInfo.AddDeveloper(_("Thorsten Geppert (thorsten@dynsoft.com)"));
	aboutDialogInfo.SetCopyright(_("DynSoft.com - Thorsten Geppert"));
	aboutDialogInfo.SetDescription(_("Programm zur Einleitung der Demophase in DynSoft.com Warehouse"));
	aboutDialogInfo.SetName(_("DynSoft.com Warehouse Demo Enabler"));
	aboutDialogInfo.SetVersion(_("1.0.0"));
	aboutDialogInfo.SetWebSite(_("http://www.dynsoft.com"));

	wxAboutBox(aboutDialogInfo);
}
