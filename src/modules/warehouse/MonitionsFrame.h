#ifndef MONITIONSFRAME_H_
#define MONITIONSFRAME_H_

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <DSTextCtrl.h>
#include <DSListCtrl.h>
#include "MonitionsTextsDialog.h"
#include "../lib/Toolbar.h"
#include "../lib/RuntimeInformationType.h"
#include "../lib/MonitionEntity.h"

using namespace DynSoft;

class MonitionsFrame : public wxFrame {

	public:
		MonitionsFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~MonitionsFrame();

	protected:
		enum {
			UPDATE_EVENT,
			FIND_CLIENT_EVENT,
			ALL_EVENT,
			NONE_EVENT,
			PLUS_EVENT,
			MINUS_EVENT,
			TEXTS_EVENT,
			RUN_EVENT,
			CLOSE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void UpdateEvent(wxMouseEvent &event);
		void UpdateKeyEvent(wxCommandEvent &event);
		void FindClientEvent(wxMouseEvent &event);
		void FindClientKeyEvent(wxCommandEvent &event);
		void AllEvent(wxMouseEvent &event);
		void AllKeyEvent(wxCommandEvent &event);
		void NoneEvent(wxMouseEvent &event);
		void NoneKeyEvent(wxCommandEvent &event);
		void PlusEvent(wxMouseEvent &event);
		void PlusKeyEvent(wxCommandEvent &event);
		void MinusEvent(wxMouseEvent &event);
		void MinusKeyEvent(wxCommandEvent &event);
		void TextsEvent(wxMouseEvent &event);
		void TextsKeyEvent(wxCommandEvent &event);
		void RunEvent(wxMouseEvent &event);
		void RunKeyEvent(wxCommandEvent &event);
		void HideEvent(wxCloseEvent &event);
		void CloseEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);

		void Update();
		void FindClient();
		void All();
		void None();
		void Plus();
		void Minus();
		void Texts();
		void Run();
		void Close();
		void CleanUp();
		void CleanUpMonitions();
		void AddToMonitionsListCtrl(const unsigned int position, MonitionEntity *monitionEntity);
		void SetMonition(const bool increment);

		RuntimeInformationType *rit;
		MonitionsTextsDialog *monitionsTextsDialog;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		ToolbarButton    *updateToolbarButton;
		ToolbarButton    *findClientToolbarButton;
		ToolbarButton    *allToolbarButton;
		ToolbarButton    *noneToolbarButton;
		ToolbarButton    *plusToolbarButton;
		ToolbarButton    *minusToolbarButton;
		ToolbarButton    *textsToolbarButton;
		ToolbarButton    *runToolbarButton;
		ToolbarButton    *closeToolbarButton;
		DSTextCtrl       *clientIdTextCtrl;
		DSTextCtrl       *orderIdTextCtrl;
		DSTextCtrl       *invoiceIdTextCtrl;
		wxStaticText     *dateFromStaticText;
		wxDatePickerCtrl *dateFromDatePickerCtrl;
		wxStaticText     *dateToStaticText;
		wxDatePickerCtrl *dateToDatePickerCtrl;
		wxListBox        *statesListBox;
		DSListCtrl       *monitionsListCtrl;
		DSTextCtrl       *priceTextCtrl;

};

#endif /* MONITIONSFRAME_H_ */
