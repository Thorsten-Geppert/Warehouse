#ifndef EDITEDINFORMATIONSDIALOG_H_
#define EDITEDINFORMATIONSDIALOG_H_

#include <wx/wx.h>
#include <DSBind.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/InformationEntity.h"

using namespace DynSoft;

class EditedInformationsDialog : public wxDialog {

	public:
		EditedInformationsDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			InformationEntity *informationEntity
		);
		void Get(bool show);
		void Update();
	
	protected:
		DECLARE_EVENT_TABLE()

		RuntimeInformationType *rit;
		DSBind bind;
		InformationEntity *informationEntity;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		wxTextCtrl *createdByTextCtrl;
		wxTextCtrl *createdAtTextCtrl;
		wxTextCtrl *changedByTextCtrl;
		wxTextCtrl *changedAtTextCtrl;

};


#endif /* EDITEDINFORMATIONSDIALOG_H_ */
