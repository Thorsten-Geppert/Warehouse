#ifndef REMOVEDIALOG_H_
#define REMOVEDIALOG_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ReferencesType.h"

using namespace DynSoft;

class RemoveDialog : public wxDialog {

	public:
		RemoveDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			ReferencesType *referencesType
		);
	
	protected:
		enum {
			CLOSE_EVENT,
			OPEN_EVENT
		};

		DECLARE_EVENT_TABLE()

		void FindEvent(wxMouseEvent &event);
		void OpenEvent(wxMouseEvent &event);
		void OpenDirectEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void OpenKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Open();
		void Cancel();

		RuntimeInformationType *rit;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *findToolbarButton;
		ToolbarButton *openToolbarButton;
		ToolbarButton *cancelToolbarButton;
		wxListBox     *itemsListBox;

};

#endif /* REMOVEDIALOG_H_ */
