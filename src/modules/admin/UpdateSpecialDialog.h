#ifndef UPDATE_SPECIAL_DIALOG_H_
#define UPDATE_SPECIAL_DIALOG_H_

#include <wx/wx.h>
#include <wx/statline.h>
#include "../lib/EnterpriseEntity.h"
#include "../lib/SqlUpdateType.h"

using namespace DynSoft;

class UpdateSpecialDialog : public wxDialog {

	public:
		UpdateSpecialDialog(
			wxWindow *parent,
			wxWindowID id,
			EnterpriseEntity *enterpriseEntity,
			const wxString &version
		);
		~UpdateSpecialDialog();
	
	protected:
		enum {
			FILE_CHOOSE_BUTTON,
			UPDATE_BUTTON
		};

		DECLARE_EVENT_TABLE()

		void FileChooseEvent(wxCommandEvent &event);
		void UpdateEvent(wxCommandEvent &event);

		void FileChoose();
		void Update();
		void SetInformations(SqlUpdateType *sqlUpdateType);

		EnterpriseEntity *enterpriseEntity;
		wxString version;
		wxString path;
		SqlUpdateType *sqlUpdateType;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		wxBoxSizer *dataBoxSizer;

		wxStaticText *headlineStaticText;
		wxStaticLine *headlineStaticLine;

		wxBoxSizer *fileBoxSizer;
		wxStaticText *filenameStaticText;
		wxTextCtrl *filenameTextCtrl;
		wxButton *fileChooseButton;

		wxStaticLine *fileStaticLine;

		wxTextCtrl *logTextCtrl;
		
		wxStaticLine *logStaticLine;

		wxButton *updateButton;
		
};

#endif /* UPDATE_SPECIAL_H_ */
