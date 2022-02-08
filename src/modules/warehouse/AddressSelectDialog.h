#ifndef ADDRESSSELECTDIALOG_H_
#define ADDRESSSELECTDIALOG_H_

#include <wx/wx.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/AddressEntities.h"
#include "../lib/AddressEntity.h"

using namespace DynSoft;

class AddressSelectDialog : public wxDialog {

	public:
		AddressSelectDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			AddressEntities &addressEntities
		);

		AddressEntity *GetSelectedAddress() const;
	
	protected:
		enum {
			USE_EVENT,
			CANCEL_EVENT
		};

		DECLARE_EVENT_TABLE()

		void UseEvent(wxMouseEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void UseSelectEvent(wxListEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Use();
		void Cancel();

		RuntimeInformationType *rit;
		AddressEntity *resultAddressEntity;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;

		ToolbarButton *useToolbarButton;
		ToolbarButton *cancelToolbarButton;
		DSListCtrl    *addressesListCtrl;

};

#endif /* ADDRESSSELECTDIALOG_H_ */
