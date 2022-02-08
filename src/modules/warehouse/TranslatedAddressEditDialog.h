#ifndef TRANSLATEDADDRESSEDITDIALOG_H_
#define TRANSLATEDADDRESSEDITDIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"

using namespace DynSoft;

class TranslatedAddressEditDialog : public wxDialog {

	public:
		TranslatedAddressEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			wxString *address,
			const unsigned int clientId = 0,
			const bool readOnly = false
		);

	protected:
		enum {
			USE_EVENT,
			FIND_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void UseEvent(wxMouseEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void FindEvent(wxMouseEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Use();
		void Find();
		void Cancel();

		RuntimeInformationType *rit;
		wxString *address;
		unsigned int clientId;
		bool readOnly;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *useToolbarButton;
		ToolbarButton *findToolbarButton;
		ToolbarButton *cancelToolbarButton;
		wxTextCtrl    *addressTextCtrl;

};

#endif /* TRANSLATEDADDRESSEDITDIALOG_H_ */
