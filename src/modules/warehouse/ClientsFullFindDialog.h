#ifndef CLIENTSFULLFINDDIALOG_H_
#define CLIENTSFULLFINDDIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ClientEntity.h"

using namespace DynSoft;

class ClientsFullFindDialog : public wxDialog {

	public:
		enum {
			KIND_ALL,
			KIND_CLIENT,
			KIND_DISTRIBUTOR,
			KIND_CLIENT_AND_DISTRIBUTOR
		};

		ClientsFullFindDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const int kind = KIND_ALL
		);
		ClientEntity *GetClientEntity() const;
		int ShowModal();
	
	protected:
		enum {
			CLOSE_EVENT,
			FIND_EVENT,
			USE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void FindEvent(wxMouseEvent &event);
		void UseEvent(wxMouseEvent &event);
		void UseDirectEvent(wxListEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void FindKeyEvent(wxCommandEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Find();
		void Use();
		void Cancel();
		void ClearClientsListCtrl();
		void AddToClientsListCtrl(ClientEntity *clientEntity);

		RuntimeInformationType *rit;
		unsigned int clientId;
		int kind;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *findToolbarButton;
		ToolbarButton *useToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSTextCtrl *clientIdTextCtrl;
		wxComboBox *salutationComboBox;
		wxComboBox *titleComboBox;
		wxTextCtrl *firstnameTextCtrl;
		wxTextCtrl *nameTextCtrl;
		wxTextCtrl *addressTextCtrl;
		wxTextCtrl *zipcodeTextCtrl;
		wxTextCtrl *cityTextCtrl;
		wxComboBox *landComboBox;
		wxTextCtrl *emailTextCtrl;
		DSListCtrl *clientsListCtrl;

};

#endif /* CLIENTSFULLDIALOG_H_ */
