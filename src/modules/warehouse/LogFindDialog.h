#ifndef LOGFINDDIALOG_H_
#define LOGFINDDIALOG_H_

#include <wx/wx.h>
#include <DSLogType.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class LogFindDialog : public wxDialog {

	public:
		LogFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		DSLogType *GetLogType() const;
		int ShowModal();
	
	protected:
		enum {
			FIND_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void FindEvent(wxMouseEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Find();
		void Cancel();

		RuntimeInformationType *rit;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *findToolbarButton;
		ToolbarButton *cancelToolbarButton;
		wxComboBox    *errorComboBox;
		wxComboBox    *userComboBox;
		wxComboBox    *moduleComboBox;
		wxTextCtrl    *descriptionTextCtrl;
		wxTextCtrl    *messageTextCtrl;

};

#endif /* LOGDIALOG_H_ */
