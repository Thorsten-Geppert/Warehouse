#ifndef DEMO_VERSION_DIALOG_H_
#define DEMO_VERSION_DIALOG_H_

#include <wx/wx.h>
#include <wx/timer.h>
#include <wx/html/htmlwin.h>
#include "../lib/RuntimeInformationType.h"

class DemoVersionDialog : public wxDialog {

	public:
		DemoVersionDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const unsigned int counter
		);
	
	private:
		wxPanel *demoVersionPanel;
		wxBoxSizer *demoVersionBoxSizer;

	protected:
		enum {
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelKeyEvent(wxCommandEvent &event);
		void CounterTimerEvent(wxTimerEvent &event);
		void CloseEvent(wxCloseEvent &event);
		void Cancel();
		void CounterTimer();

		wxTimer counterTimer;
		unsigned int counter;
		unsigned int intCounter;

		wxHtmlWindow *gtcHtmlWindow;
		wxStaticText *versionStaticText;
		wxButton     *closeButton;

};

#endif /* DEMO_VERSION_DIALOG_H_ */
