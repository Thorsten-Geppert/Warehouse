#ifndef FILEEDITDIALOG_H_
#define FILEEDITDIALOG_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include <DSDatePickerCtrl.h>
#include <DSCheckBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/BankConnectionEntity.h"

using namespace DynSoft;

class BankConnectionEditDialog : public wxDialog {

	public:
		BankConnectionEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			BankConnectionEntity *bankConnectionEntity = NULL
		);

	protected:
		enum {
			SAVE_EVENT,
			CANCEL_EVENT,
			FIND_EVENT
		};
		
		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void FindEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void FindKeyEvent(wxCommandEvent &event);

		void Close();
		void Save();
		void Find();

		RuntimeInformationType *rit;
		BankConnectionEntity *bankConnectionEntity;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		ToolbarButton    *saveToolbarButton;
		ToolbarButton    *findToolbarButton;
		ToolbarButton    *closeToolbarButton;
		DSTextCtrl       *descriptionTextCtrl;
		DSTextCtrl       *bankTextCtrl;
		DSTextCtrl       *bankCodeTextCtrl;
		DSTextCtrl       *accountNumberTextCtrl;
		DSTextCtrl       *bicTextCtrl;
		DSTextCtrl       *ibanTextCtrl;
		DSTextCtrl       *bankOwnerTextCtrl;
		DSTextCtrl       *referenceNumberTextCtrl;
		DSDatePickerCtrl *referenceDateDatePickerCtrl;
		DSComboBox       *referenceStateComboBox;
		DSComboBox       *referenceAccountComboBox;
		DSTextCtrl       *notesTextCtrl;
		DSCheckBox       *activeCheckBox;

};

#endif /* FILEEDITDIALOG_H_ */
