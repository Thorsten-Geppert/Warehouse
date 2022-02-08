#ifndef INFORMATION_DIALOG_H_
#define INFORMATION_DIALOG_H_

#include <wx/wx.h>
#include <wx/html/htmlwin.h>
#include "../lib/RuntimeInformationType.h"

class InformationDialog : public wxDialog {

	public:
		InformationDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
	
	private:
		wxPanel *informationPanel;
		wxBoxSizer *informationBoxSizer;

	protected:
		enum {
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelKeyEvent(wxCommandEvent &event);
		void Cancel();

		wxHtmlWindow *gtcHtmlWindow;
		wxStaticText *versionStaticText;

};

#endif /* INFORMATION_DIALOG_H_ */
