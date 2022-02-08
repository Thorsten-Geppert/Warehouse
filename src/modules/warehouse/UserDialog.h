#ifndef USERDIALOG_H_
#define USERDIALOG_H_

#include <wx/wx.h>
#include <wx/xrc/xmlres.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/UserController.h"
#include "../lib/UserEntity.h"
#include "../lib/Toolbar.h"

class UserDialog : public wxDialog {

	public:
		UserDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			UserEntity *userEntity = NULL
		);

		UserEntity *GetUser() const;
	
	protected:
		enum {
			SAVE_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CancelEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);

		void Cancel();
		void Save();
		void TemplateSelect(wxObject *object);

		RuntimeInformationType *rit;
		UserEntity *userEntity;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		Toolbar *mainToolbar;
		ToolbarButton *cancelToolbarButton;
		ToolbarButton *saveToolbarButton;

		wxTextCtrl   *usernameTextCtrl;
		wxTextCtrl   *passwordTextCtrl;
		wxTextCtrl   *realNameTextCtrl;
		wxTextCtrl   *emailTextCtrl;
		wxStaticText *descriptionStaticText;
		wxTextCtrl   *descriptionTextCtrl;
		wxComboBox   *statusComboBox;
		wxCheckBox   *remindCheckBox;

};

#endif /* USERDIALOG_H_ */
