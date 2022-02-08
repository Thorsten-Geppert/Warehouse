#ifndef CLIENTEDITFRAME_H_
#define CLIENTEDITFRAME_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSBind.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include <DSCheckBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ClientEntity.h"
#include "GuiCallbackInterface.h"
#include "EditedInformationsDialog.h"
#include "TextEditDialog.h"
#include "ClientOrdersDialog.h"
#include "MediaPanel.h"
#include "MoreDataPanel.h"

using namespace DynSoft;

class ClientEditFrame : public wxFrame {

	public:
		ClientEditFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			ClientEntity *clientEntity,
			GuiCallbackInterface *callbackInterface = NULL
		);
		~ClientEditFrame();

	protected:
		enum {
			CLOSE_EVENT,
			SAVE_EVENT,
			PRINT_EVENT,
			PRINT_PREVIEW_EVENT,
			HIDDEN_INFORMATIONS_EVENT,
			EDITED_INFORMATIONS_EVENT,
			ORDERS_EVENT,
			REMINDERS_EVENT,
			ADDRESS_ADD_EVENT,
			ADDRESS_REMOVE_EVENT,
			ADDRESS_CHANGE_EVENT,
			CONTACT_PERSON_ADD_EVENT,
			CONTACT_PERSON_REMOVE_EVENT,
			BANK_CONNECTION_ADD_EVENT,
			BANK_CONNECTION_REMOVE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void PrintPreviewEvent(wxMouseEvent &event);
		void OrdersEvent(wxMouseEvent &event);
		void RemindersEvent(wxMouseEvent &event);
		void ChangeFormOfOrganizationEvent(wxCommandEvent &event);
		void ShowEditedInformationsEvent(wxMouseEvent &event);
		void ShowHiddenInformationsEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void ShowEditedInformationsKeyEvent(wxCommandEvent &event);
		void ShowHiddenInformationsKeyEvent(wxCommandEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void PrintPreviewKeyEvent(wxCommandEvent &event);
		void OrdersKeyEvent(wxCommandEvent &event);
		void RemindersKeyEvent(wxCommandEvent &event);
		void AddressAddEvent(wxCommandEvent &event);
		void AddressAddKeyEvent(wxCommandEvent &event);
		void AddressRemoveEvent(wxCommandEvent &event);
		void AddressRemoveKeyEvent(wxCommandEvent &event);
		void AddressChangeEvent(wxCommandEvent &event);
		void AddressChangeKeyEvent(wxCommandEvent &event);
		void AddressEditEvent(wxListEvent &event);
		void ContactPersonAddEvent(wxCommandEvent &event);
		void ContactPersonAddKeyEvent(wxCommandEvent &event);
		void ContactPersonRemoveEvent(wxCommandEvent &event);
		void ContactPersonRemoveKeyEvent(wxCommandEvent &event);
		void ContactPersonEditEvent(wxListEvent &event);
		void BankConnectionEditEvent(wxListEvent &event);
		void BankConnectionAddEvent(wxCommandEvent &event);
		void BankConnectionRemoveEvent(wxCommandEvent &event);
		void BankConnectionAddKeyEvent(wxCommandEvent &event);
		void BankConnectionRemoveKeyEvent(wxCommandEvent &event);

		void Close();
		bool Save();
		void ChangeFormOfOrganization();
		void ShowEditedInformations();
		void ShowHiddenInformations();
		void Print(const bool preview = false);
		void Orders();
		void Reminders();
		void Callback(void *data);

		void AddressAdd();
		void AddressRemove();
		void AddressChange();
		void AddressEdit();
		void ContactPersonAdd();
		void ContactPersonRemove();
		void ContactPersonEdit();
		void BankConnectionEdit();
		void BankConnectionAdd();
		void BankConnectionRemove();
		void AddOrChangeToAddressesListCtrl(AddressEntity *addressEntity);
		void AddOrChangeToContactPersonsListCtrl(ContactPersonEntity *contactPersonEntity);
		void AddOrChangeToBankConnectionsListCtrl(BankConnectionEntity *bankConnectionEntity);

		RuntimeInformationType *rit;
		DSBind bind;
		unsigned int clientId;
		GuiCallbackInterface *callbackInterface;
		wxString firstname;
		bool changed;
		bool mandatoryOwnClientId;
		wxString *hiddenValue;
		EditedInformationsDialog *editedInformationsDialog;
		TextEditDialog *hiddenInformationsDialog;
		ClientOrdersDialog *clientOrdersDialog;
		wxArrayString removedAddresses, removedContactPersons;
		wxString oldOwnClientId;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *saveToolbarButton;
		ToolbarButton *closeToolbarButton;
		ToolbarButton *editedInformationsToolbarButton;
		ToolbarButton *hiddenInformationsToolbarButton;
		ToolbarButton *printToolbarButton;
		ToolbarButton *printPreviewToolbarButton;
		ToolbarButton *ordersToolbarButton;
		ToolbarButton *remindersToolbarButton;
		DSTextCtrl    *clientIdTextCtrl;
		DSComboBox    *formOfOrganizationComboBox;
		DSTextCtrl    *circularLetterTextCtrl;
		DSComboBox    *salutationComboBox;
		DSComboBox    *titleComboBox;
		wxStaticText  *firstnameStaticText;
		DSTextCtrl    *firstnameTextCtrl;
		DSTextCtrl    *name1TextCtrl;
		DSTextCtrl    *name2TextCtrl;
		DSTextCtrl    *shortNameTextCtrl;
		DSTextCtrl    *addressTextCtrl;
		DSTextCtrl    *numberTextCtrl;
		DSTextCtrl    *zipcodeTextCtrl;
		DSTextCtrl    *cityTextCtrl;
		DSComboBox    *federalStateComboBox;
		DSComboBox    *landComboBox;
		DSTextCtrl    *name3TextCtrl;
		DSTextCtrl    *name4TextCtrl;
		DSTextCtrl    *telephone1CountryTextCtrl;
		DSTextCtrl    *telephone1PrefixTextCtrl;
		DSTextCtrl    *telephone1NumberTextCtrl;
		DSTextCtrl    *telephone2CountryTextCtrl;
		DSTextCtrl    *telephone2PrefixTextCtrl;
		DSTextCtrl    *telephone2NumberTextCtrl;
		DSTextCtrl    *telephone3CountryTextCtrl;
		DSTextCtrl    *telephone3PrefixTextCtrl;
		DSTextCtrl    *telephone3NumberTextCtrl;
		DSTextCtrl    *fax1CountryTextCtrl;
		DSTextCtrl    *fax1PrefixTextCtrl;
		DSTextCtrl    *fax1NumberTextCtrl;
		DSTextCtrl    *fax2CountryTextCtrl;
		DSTextCtrl    *fax2PrefixTextCtrl;
		DSTextCtrl    *fax2NumberTextCtrl;
		DSTextCtrl    *fax3CountryTextCtrl;
		DSTextCtrl    *fax3PrefixTextCtrl;
		DSTextCtrl    *fax3NumberTextCtrl;
		DSTextCtrl    *mobile1CountryTextCtrl;
		DSTextCtrl    *mobile1PrefixTextCtrl;
		DSTextCtrl    *mobile1NumberTextCtrl;
		DSTextCtrl    *mobile2CountryTextCtrl;
		DSTextCtrl    *mobile2PrefixTextCtrl;
		DSTextCtrl    *mobile2NumberTextCtrl;
		DSTextCtrl    *email1TextCtrl;
		DSTextCtrl    *email2TextCtrl;
		DSTextCtrl    *internet1TextCtrl;
		DSTextCtrl    *internet2TextCtrl;
		DSTextCtrl    *informationsTextCtrl;
		DSListCtrl    *addressesListCtrl;
		DSListCtrl    *contactPersonsListCtrl;
		DSTextCtrl    *taxNumberTextCtrl;
		DSTextCtrl    *salesTaxIdentificationNumberTextCtrl;
		DSTextCtrl    *discountTextCtrl;
		DSCheckBox    *liableToTaxCheckBox;
		wxBoxSizer    *mediaBoxSizer;
		MediaPanel    *mediaPanel;
		wxBoxSizer    *moreDataBoxSizer;
		MoreDataPanel *moreDataPanel;
		DSListCtrl    *bankConnectionsListCtrl;

		wxStaticText  *ownClientIdStaticText;
		DSTextCtrl    *ownClientIdTextCtrl;
		DSComboBox    *kindComboBox;
		DSComboBox    *groupComboBox;
		DSComboBox    *sectorComboBox;
		DSComboBox    *birthdayDayComboBox;
		DSComboBox    *birthdayMonthComboBox;
		DSComboBox    *birthdayYearComboBox;
		DSCheckBox    *activeCheckBox;
		DSComboBox    *conditionComboBox;
		DSCheckBox    *monitionableCheckBox;

};

#endif /* CLIENTEDITFRAME_H_ */
