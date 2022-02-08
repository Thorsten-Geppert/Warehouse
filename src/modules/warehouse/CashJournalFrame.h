#ifndef CASHJOURNALSFRAME_H_
#define CASHJOURNALSFRAME_H_

#include <wx/wx.h>
#include "../lib/Toolbar.h"
#include "../lib/RuntimeInformationType.h"

using namespace DynSoft;

class CashJournalFrame : public wxFrame {

	public:
		CashJournalFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~CashJournalFrame();

	protected:
		enum {
			CLOSE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CloseEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void HideEvent(wxCloseEvent &event);

		void Close();

		RuntimeInformationType *rit;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		ToolbarButton    *closeToolbarButton;

};

#endif /* CASHJOURNALSFRAME_H_ */
