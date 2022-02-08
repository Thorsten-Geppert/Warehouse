#include "InformationDialog.h"
#include <DSLib.h>
#include "Configuration.h"
#include "AdminIcon.xpm"
#include "LoginLogo.xpm"

BEGIN_EVENT_TABLE(InformationDialog, wxDialog)
	EVT_MENU(CANCEL_EVENT, InformationDialog::CancelKeyEvent)
END_EVENT_TABLE()

InformationDialog::InformationDialog(
	wxWindow *parent,
	wxWindowID id
) : wxDialog(
	parent,
	id,
	NAME,
	wxDefaultPosition,
	wxDefaultSize
) {
	SetId(id);

	informationPanel = new wxPanel(this, -1);
	informationBoxSizer = new wxBoxSizer(wxVERTICAL);

	logoStaticBitmap = new wxStaticBitmap(informationPanel, -1, wxBitmap(LoginLogo_xpm));
	informationBoxSizer->Add(logoStaticBitmap);

	informationHtmlWindow = new wxHtmlWindow(informationPanel, -1);
	informationBoxSizer->Add(informationHtmlWindow, 1, wxEXPAND | wxALL, 15);

	informationPanel->SetSizer(informationBoxSizer);
	informationBoxSizer->SetSizeHints(this);

	SetSize(wxSize(-1, 750));

	wxAcceleratorEntry acceleratorEntries[1];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	wxAcceleratorTable acceleratorTable(1, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	informationHtmlWindow->SetPage(_N(
		"<html>"
		"<head>"
		"	<title>Information</title>"
		"</head>"
		""
		"<body>"
		"	<h2>DynSoft.com - Thorsten Geppert</h2>"
		"	<p />"
		"	Steinkaul 24<br />"
		"	56651 Oberd&uuml;renbach"
		"	<p />"
		"	<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">"
		"		<tr>"
		"			<td align=\"right\"><b>Telefon:</b></td>"
		"			<td>&nbsp;</td>"
		"			<td>+49 (0) 2646 / 8453007</td>"
		"		</tr>"
		"		<tr>"
		"			<td align=\"right\"><b>Mobil:</b></td>"
		"			<td>&nbsp;</td>"
		"			<td>+49 (0) 151 / 22666988</td>"
		"		</tr>"
		"		<tr>"
		"			<td align=\"right\"><b>E-Mail:</b></td>"
		"			<td>&nbsp;</td>"
		"			<td>info@dynsoft.com</td>"
		"		</tr>"
		"		<tr>"
		"			<td align=\"right\"><b>Web:</b></td>"
		"			<td>&nbsp;</td>"
		"			<td>https://www.dynsoft.com</td>"
		"		</tr>"
		"	</table>"
		""
		"<p />"
		""
		"<h2>Danksagungen</h2>"
		""
		"Wir wollen uns bei folgenden Projekten bedanken:"
		""
		"<p />"
		""
		"<table border=\"0\" cellspacing=\"0\" cellpadding=\"0\">"
		"	<tr>"
		"		<td align=\"right\"><b>wxWidgets:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>GUI Toolkit</td>"
		"		<td>&nbsp;</td>"
		"		<td>wxwidgets.org</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>PostgreSQL:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Database</td>"
		"		<td>&nbsp;</td>"
		"		<td>postgresql.org</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>EnterpriseDB:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Database-Installer</td>"
		"		<td>&nbsp;</td>"
		"		<td>enterprisedb.com</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>Oxygen:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Icon sets</td>"
		"		<td>&nbsp;</td>"
		"		<td>deviantdark.com</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>DSLib:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Toolkit</td>"
		"		<td>&nbsp;</td>"
		"		<td>dynsoft.com</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>FreeBSD:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Betriebssystem</td>"
		"		<td>&nbsp;</td>"
		"		<td>freebsd.org</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>Ubuntu:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Betriebssystem</td>"
		"		<td>&nbsp;</td>"
		"		<td>ubuntu.com</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>Inkscape:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Vektorgrafikprogramm</td>"
		"		<td>&nbsp;</td>"
		"		<td>Inkscape.org</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>Gimp:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Bildbearbeitung</td>"
		"		<td>&nbsp;</td>"
		"		<td>gimp.org</td>"
		"	</tr>"
		"	<tr>"
		"		<td align=\"right\"><b>GIT:</b></td>"
		"		<td>&nbsp;</td>"
		"		<td>Versionskontrolle</td>"
		"		<td>&nbsp;</td>"
		"		<td>git-scm.com</td>"
		"	</tr>"
		"</table>"
		""
		"<p />"
		""
		"Und vielen mehr. Vielen Dank an Euch alle."
		"</body>"
		"</html>"
	));

	ICON();
}

void InformationDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void InformationDialog::Cancel() {
	Close();
}
