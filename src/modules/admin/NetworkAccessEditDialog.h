#ifndef NETWORK_ACCESS_EDIT_DIALOG_H_
#define NETWORK_ACCESS_EDIT_DIALOG_H_

#include <wx/wx.h>
#include <DSLib.h>
#include "../lib/PgHbaHostType.h"

using namespace DynSoft;

class NetworkAccessEditDialog : public wxDialog {

	public:
		NetworkAccessEditDialog(wxWindow *parent, wxWindowID id, PgHbaHostType *hostEntity = NULL);

	protected:
		enum {
			SAVE_EVENT,
			ABORT_EVENT
		};

		DECLARE_EVENT_TABLE()

		void SaveEvent(wxCommandEvent &event);
		void AbortEvent(wxCommandEvent &event);

		void Save();
		void Abort();
		void LoadType();
		
		PgHbaHostType *hostEntity;


		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		wxFlexGridSizer *dataFlexGridSizer;

		wxStaticText *typeStaticText;
		wxComboBox *typeComboBox;
		wxStaticText *databaseStaticText;
		wxComboBox *databaseComboBox;
		wxStaticText *userStaticText;
		wxComboBox *userComboBox;
		wxStaticText *addressStaticText;
		wxComboBox *addresseComboBox;
		wxStaticText *subnetStaticText;
		wxComboBox *subnetComboBox;
		wxStaticText *methodStaticText;
		wxComboBox *methodComboBox;
		
		wxBoxSizer *optionsBoxSizer;
		wxButton *abortButton;
		wxButton *saveButton;

};

#endif /* NETWORK_ACCESS_EDIT_DIALOG_H_ */
