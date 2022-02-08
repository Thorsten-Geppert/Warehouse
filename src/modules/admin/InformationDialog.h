#ifndef INFORMATION_DIALOG_H_
#define INFORMATION_DIALOG_H_

#include <wx/wx.h>
#include <wx/html/htmlwin.h>

class InformationDialog : public wxDialog {

	public:
		InformationDialog(
			wxWindow *parent,
			wxWindowID id
		);
	
	private:
		wxPanel *informationPanel;
		wxBoxSizer *informationBoxSizer;
		wxStaticBitmap *logoStaticBitmap;
		wxHtmlWindow *informationHtmlWindow;

	protected:
		enum {
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelKeyEvent(wxCommandEvent &event);
		void Cancel();

};

#endif /* INFORMATION_DIALOG_H_ */
