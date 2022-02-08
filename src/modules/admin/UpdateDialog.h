#ifndef UPDATE_DIALOG_H_
#define UPDATE_DIALOG_H_

#include <wx/wx.h>
#include <wx/statline.h>
#include <wx/gauge.h>
#include "../lib/EnterpriseEntity.h"

using namespace DynSoft;

class UpdateDialog : public wxDialog {

	public:
		UpdateDialog(
			wxWindow *parent,
			wxWindowID id,
			EnterpriseEntity *enterpriseEntity,
			wxArrayString *versions,
			const wxString &from
		);
	
	protected:
		enum {
			UPDATE_BUTTON_EVENT
		};

		DECLARE_EVENT_TABLE()

		void UpdateEvent(wxCommandEvent &event);
		void Update();
		void EnableControls(const bool enable);
		void Log(const wxString &log);

		EnterpriseEntity *enterpriseEntity;
		wxArrayString *versions;
		wxString from;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		wxBoxSizer *dataBoxSizer;

		wxStaticText *headlineStaticText;
		wxStaticText *updateStaticText;
		wxStaticLine *headlineStaticLine;
		
		wxBoxSizer *versionBoxSizer;
		wxStaticText *versionStaticText;
		wxStaticLine *versionLeftStaticLine;
		wxButton *updateButton;
		wxStaticLine *versionRightStaticLine;
		wxComboBox *versionsComboBox;

		wxStaticLine *progressStaticLine;

		wxStaticText *progressStaticText;
		wxGauge *progressGauge;

		wxStaticText *informationsStaticText;
		wxTextCtrl *informationsTextCtrl;

};

#endif /* UPDATE_DIALOG_H_ */
