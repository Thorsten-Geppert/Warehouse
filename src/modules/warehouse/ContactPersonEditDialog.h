#ifndef CONTACTPERSONEDITDIALOG_H_
#define CONTACTPERSONEDITDIALOG_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSBind.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ContactPersonEntity.h"

using namespace DynSoft;

class ContactPersonEditDialog : public wxDialog {

	public:
		ContactPersonEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			ContactPersonEntity *contactPersonEntity,
			const bool writeable = true
		);

	protected:
		enum {
			CLOSE_EVENT,
			SAVE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);

		void Close();
		bool Save();
		void TemplateSelect(wxObject *object);

		RuntimeInformationType *rit;
		DSBind bind;
		bool changed;
		bool writeable;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *saveToolbarButton;
		ToolbarButton *closeToolbarButton;
		DSComboBox    *functionComboBox;
		DSComboBox    *departmentComboBox;
		DSComboBox    *salutationComboBox;
		DSComboBox    *titleComboBox;
		DSTextCtrl    *firstnameTextCtrl;
		DSTextCtrl    *lastnameTextCtrl;
		wxStaticText  *informationStaticText;
		DSTextCtrl    *informationTextCtrl;
		DSTextCtrl    *telephoneCountryTextCtrl;
		DSTextCtrl    *telephonePrefixTextCtrl;
		DSTextCtrl    *telephoneNumberTextCtrl;
		DSTextCtrl    *mobileCountryTextCtrl;
		DSTextCtrl    *mobilePrefixTextCtrl;
		DSTextCtrl    *mobileNumberTextCtrl;
		DSTextCtrl    *faxCountryTextCtrl;
		DSTextCtrl    *faxPrefixTextCtrl;
		DSTextCtrl    *faxNumberTextCtrl;
		DSTextCtrl    *emailTextCtrl;
		DSTextCtrl    *internetTextCtrl;

};

#endif /* CONTACTPERSONEDITDIALOG_H_ */
