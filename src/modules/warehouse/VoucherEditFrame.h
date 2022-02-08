#ifndef VOUCHEREDITFRAME_H_
#define VOUCHEREDITFRAME_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSBind.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include <DSListCtrl.h>
#include <DSDatePickerCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/VoucherEntity.h"
#include "../lib/VoucherPositionEntity.h"
#include "GuiCallbackInterface.h"
#include "EditedInformationsDialog.h"
#include "MediaPanel.h"
#include "MoreDataPanel.h"

using namespace DynSoft;

class VoucherEditFrame : public wxFrame {

	public:
		VoucherEditFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			VoucherEntity *voucherEntity,
			GuiCallbackInterface *callbackInterface = NULL
		);
		~VoucherEditFrame();

	protected:
		enum {
			CLOSE_EVENT,
			SAVE_EVENT,
			PRINT_EVENT,
			PRINT_PREVIEW_EVENT,
			EDITED_INFORMATIONS_EVENT,
			ADD_POSITION_EVENT,
			REMOVE_POSITION_EVENT,
			FIND_COMPANY_EVENT,
			CALCULATE_EVENT,
			REMINDERS_EVENT
		};

		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void PrintPreviewEvent(wxMouseEvent &event);
		void AddPositionEvent(wxCommandEvent &event);
		void EditPositionEvent(wxCommandEvent &event);
		void RemovePositionEvent(wxCommandEvent &event);
		void ShowEditedInformationsEvent(wxMouseEvent &event);
		void FindCompanyEvent(wxMouseEvent &event);
		void CalculateEvent(wxMouseEvent &event);
		void CalculateKeyEvent(wxCommandEvent &event);
		void RemindersEvent(wxMouseEvent &event);
		void RemindersKeyEvent(wxCommandEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void ShowEditedInformationsKeyEvent(wxCommandEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void PrintPreviewKeyEvent(wxCommandEvent &event);
		void AddPositionKeyEvent(wxCommandEvent &event);
		void EditPositionKeyEvent(wxCommandEvent &event);
		void DirectEditPositionEvent(wxListEvent &event);
		void RemovePositionKeyEvent(wxCommandEvent &event);
		void FindCompanyKeyEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);

		void Close();
		bool Save();
		void ShowEditedInformations();
		void Print(const bool preview = false);
		void AddPosition();
		void EditPosition();
		void RemovePosition();
		void TemplateSelect(wxObject *object);
		void AddOrChangeToVoucherPositionsListCtrl(VoucherPositionEntity *voucherPositionEntity);
		void FindCompany();
		void Calculate();
		void Reminders();
		void CalculateProof(DSCurrency &price, DSCurrency &reducedTax, DSCurrency &fullTax, DSCurrency &sum);
		bool CalculateCheck();

		RuntimeInformationType *rit;
		DSBind bind;
		int voucherId;
		GuiCallbackInterface *callbackInterface;
		bool changed;
		EditedInformationsDialog *editedInformationsDialog;
		wxArrayString removedVoucherPositions;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		ToolbarButton    *saveToolbarButton;
		ToolbarButton    *closeToolbarButton;
		ToolbarButton    *findCompanyToolbarButton;
		ToolbarButton    *editedInformationsToolbarButton;
		ToolbarButton    *printToolbarButton;
		ToolbarButton    *printPreviewToolbarButton;
		ToolbarButton    *addPositionToolbarButton;
		ToolbarButton    *editPositionToolbarButton;
		ToolbarButton    *removePositionToolbarButton;
		ToolbarButton    *calculateToolbarButton;
		ToolbarButton    *remindersToolbarButton;
		DSTextCtrl       *voucherIdTextCtrl;
		DSComboBox       *descriptionComboBox;
		DSTextCtrl       *companyTextCtrl;
		wxStaticText     *notesStaticText;
		DSTextCtrl       *notesTextCtrl;
		DSTextCtrl       *invoiceIdTextCtrl;
		DSDatePickerCtrl *invoiceDateDatePickerCtrl;
		DSListCtrl       *voucherPositionsListCtrl;
		DSTextCtrl       *priceTextCtrl;
		DSTextCtrl       *reducedTaxTextCtrl;
		DSTextCtrl       *fullTaxTextCtrl;
		DSTextCtrl       *fullPriceTextCtrl;
		wxBoxSizer       *mediaBoxSizer;
		MediaPanel       *mediaPanel;
		wxBoxSizer       *moreDataBoxSizer;
		MoreDataPanel    *moreDataPanel;

};

#endif /* VOUCHEREDITFRAME_H_ */
