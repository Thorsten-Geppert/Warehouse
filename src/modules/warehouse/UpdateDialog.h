#ifndef UPDATEEDITDIALOG_H_
#define UPDATEEDITDIALOG_H_

#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/UpdateType.h"

class UpdateDialog : public wxDialog {

	public:
		UpdateDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			UpdateType *updateType
		);

	protected:
		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CustomerCenterEvent(wxMouseEvent &event);
		void CloseEvent(wxMouseEvent &event);

		void CustomerCenter();
		void Close();

		RuntimeInformationType *rit;
		UpdateType *updateType;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *customerCenterToolbarButton;
		ToolbarButton *closeToolbarButton;
		wxStaticText  *versionStaticText;
		wxHtmlWindow  *informationHtmlWindow;

};

#endif /* UPDATEEDITDIALOG_H_ */
