#ifndef ADDRESSEDITDIALOG_H_
#define ADDRESSEDITDIALOG_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSBind.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/AddressEntity.h"

using namespace DynSoft;

class AddressEditDialog : public wxDialog {

	public:
		AddressEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			AddressEntity *addressEntity,
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
		DSComboBox    *descriptionComboBox;
		DSComboBox    *salutationComboBox;
		DSComboBox    *titleComboBox;
		DSTextCtrl    *firstnameTextCtrl;
		DSTextCtrl    *name1TextCtrl;
		DSTextCtrl    *name2TextCtrl;
		DSTextCtrl    *addressTextCtrl;
		DSTextCtrl    *numberTextCtrl;
		DSTextCtrl    *zipcodeTextCtrl;
		DSTextCtrl    *cityTextCtrl;
		DSComboBox    *federalStateComboBox;
		DSComboBox    *landComboBox;
		DSTextCtrl    *name3TextCtrl;
		DSTextCtrl    *name4TextCtrl;
		wxStaticText  *informationStaticText;
		DSTextCtrl    *informationTextCtrl;

};

#endif /* ADDRESSEDITDIALOG_H_ */
