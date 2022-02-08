#ifndef CLIENTSFINDDIALOG_H_
#define CLIENTSFINDDIALOG_H_

#include <wx/wx.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ClientEntity.h"

using namespace DynSoft;

class ClientsFindDialog : public wxDialog {

	public:
		ClientsFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		ClientEntity *GetClientEntity() const;
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
		DSTextCtrl    *clientIdTextCtrl;
		wxComboBox    *salutationComboBox;
		wxComboBox    *titleComboBox;
		wxTextCtrl    *firstnameTextCtrl;
		wxTextCtrl    *nameTextCtrl;
		wxTextCtrl    *addressTextCtrl;
		wxTextCtrl    *zipcodeTextCtrl;
		wxTextCtrl    *cityTextCtrl;
		wxComboBox    *landComboBox;
		wxTextCtrl    *emailTextCtrl;

};

#endif /* CLIENTSDIALOG_H_ */
