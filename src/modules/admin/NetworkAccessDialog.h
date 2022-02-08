#ifndef NETWORK_ACCESS_DIALOG_H_
#define NETWORK_ACCESS_DIALOG_H_

#include <wx/wx.h>
#include <wx/statline.h>
#include <DSListCtrl.h>
#include "../lib/PgHbaHostsType.h"

using namespace DynSoft;

class NetworkAccessDialog : public wxDialog {

	public:
		NetworkAccessDialog(wxWindow *parent, wxWindowID id);
		~NetworkAccessDialog();
	
	protected:
		enum {
			BROWSE_EVENT,
			SAVE_EVENT,
			ADD_EVENT,
			EDIT_EVENT,
			EDIT_LIST_EVENT,
			REMOVE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void SaveEvent(wxCommandEvent &event);
		void BrowseEvent(wxCommandEvent &event);
		void RemoveEvent(wxCommandEvent &event);
		void AddEvent(wxCommandEvent &event);
		void EditEvent(wxCommandEvent &event);
		void EditListEvent(wxListEvent &event);

		void Save(const wxString &filename);
		void Browse();
		void Remove();
		void Edit();
		void Add();
		unsigned int LoadHosts(const wxString &filename);
		unsigned int UpdateHosts();
		void AddInList(DSListCtrl *hostsListCtrl, PgHbaHostType *hostEntry);

		wxArrayString lines;
		PgHbaHostsType pgHbaHostsType;
		wxString choosenFile;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		
		wxBoxSizer *browseBoxSizer;
		wxStaticText *fileStaticText;
		wxTextCtrl *fileTextCtrl;
		wxButton *fileButton;
		wxButton *saveButton;

		DSListCtrl *hostsListCtrl;
		wxBoxSizer *listBoxSizer;
		wxBoxSizer *optionsBoxSizer;
		wxButton *addHostButton;
		wxButton *editHostButton;
		wxButton *removeHostButton;

};

#endif /* NETWORK_ACCESS_DIALOG_H_ */
