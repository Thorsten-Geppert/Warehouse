#ifndef PRODUCTEDITFRAME_H_
#define PRODUCTEDITFRAME_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSBind.h>
#include <DSTextCtrl.h>
#include <DSComboBox.h>
#include <DSCheckBox.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/ProductEntity.h"
#include "GuiCallbackInterface.h"
#include "EditedInformationsDialog.h"
#include "MediaPanel.h"
#include "MoreDataPanel.h"

using namespace DynSoft;

class ProductEditFrame : public wxFrame {

	public:
		ProductEditFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			ProductEntity *productEntity,
			GuiCallbackInterface *callbackInterface = NULL
		);
		~ProductEditFrame();

	protected:
		enum {
			CLOSE_EVENT,
			SAVE_EVENT,
			PRINT_EVENT,
			PRINT_PREVIEW_EVENT,
			EDITED_INFORMATIONS_EVENT,
			REMINDERS_EVENT,
			DISTRIBUTOR_FIND_EVENT,
			DISTRIBUTOR_OPEN_EVENT
		};

		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void PrintPreviewEvent(wxMouseEvent &event);
		void RemindersEvent(wxMouseEvent &event);
		void ShowEditedInformationsEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void ShowEditedInformationsKeyEvent(wxCommandEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void RemindersKeyEvent(wxCommandEvent &event);
		void PrintPreviewKeyEvent(wxCommandEvent &event);
		void CalculateTaxEvent(wxCommandEvent &event);
		void MarginEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);
		void DistributorFindEvent(wxCommandEvent &event);
		void DistributorOpenEvent(wxCommandEvent &event);

		void Close();
		bool Save();
		void ShowEditedInformations();
		void Print(const bool preview = false);
		void Reminders();
		void CalculateTax(const int id);
		void Margin();
		void TemplateSelect(wxObject *object);
		void DistributorFind();
		void DistributorOpen();

		RuntimeInformationType *rit;
		DSBind bind;
		int productId;
		GuiCallbackInterface *callbackInterface;
		bool changed;
		EditedInformationsDialog *editedInformationsDialog;

		bool mandatoryArticleNumber;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *saveToolbarButton;
		ToolbarButton *closeToolbarButton;
		ToolbarButton *editedInformationsToolbarButton;
		ToolbarButton *printToolbarButton;
		ToolbarButton *printPreviewToolbarButton;
		ToolbarButton *remindersToolbarButton;
		DSTextCtrl    *productIdTextCtrl;
		wxStaticText  *articleNumberStaticText;
		DSTextCtrl    *articleNumberTextCtrl;
		DSComboBox    *groupComboBox;
		DSComboBox    *packageComboBox;
		DSCheckBox    *availableCheckBox;
		DSTextCtrl    *distributorTextCtrl;
		wxButton      *distributorFindButton;
		wxButton      *distributorOpenButton;
		DSComboBox    *manufacturerComboBox;
		DSComboBox    *publisherComboBox;
		DSTextCtrl    *eanTextCtrl;
		DSTextCtrl    *shortNameTextCtrl;
		DSTextCtrl    *name1TextCtrl;
		DSTextCtrl    *name2TextCtrl;
		DSTextCtrl    *name3TextCtrl;
		DSTextCtrl    *name4TextCtrl;
		wxStaticText  *descriptionStaticText;
		DSTextCtrl    *descriptionTextCtrl;
		wxStaticText  *noteStaticText;
		DSTextCtrl    *noteTextCtrl;
		DSTextCtrl    *isbn10TextCtrl;
		DSTextCtrl    *isbn13TextCtrl;
		DSComboBox    *taxComboBox;
		DSTextCtrl    *priceWithoutTaxTextCtrl;
		DSTextCtrl    *priceWithTaxTextCtrl;
		DSTextCtrl    *purchasePriceWithoutTaxTextCtrl;
		DSTextCtrl    *purchasePriceWithTaxTextCtrl;
		wxTextCtrl    *marginWithoutTaxTextCtrl;
		wxTextCtrl    *marginWithTaxTextCtrl;
		DSTextCtrl    *weightTextCtrl;
		wxBoxSizer    *mediaBoxSizer;
		MediaPanel    *mediaPanel;
		wxBoxSizer    *moreDataBoxSizer;
		MoreDataPanel *moreDataPanel;

};

#endif /* PRODUCTEDITFRAME_H_ */
