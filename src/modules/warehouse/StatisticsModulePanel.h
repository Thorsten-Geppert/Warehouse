#ifndef STATISTICSMODULEPANEL_H_
#define STATISTICSMODULEPANEL_H_

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <DSListCtrl.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/Toolbar.h"
#include "../lib/StatisticsEntity.h"

using namespace DynSoft;

class StatisticsModulePanel : public ModulePanel {

	public:
		StatisticsModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~StatisticsModulePanel();

		virtual void Load();
		virtual void Unload();
	
	protected:
		enum {
			RUN_EVENT,
			FIND_PRODUCT_EVENT,
			FIND_CLIENT_EVENT,
			PRINT_EVENT,
			PRINT_PREVIEW_EVENT,
			EXPORT_EVENT
		};

		DECLARE_EVENT_TABLE()
		
		void RunEvent(wxMouseEvent &event);
		void FindProductEvent(wxMouseEvent &event);
		void FindClientEvent(wxMouseEvent &event);
		void PrintEvent(wxMouseEvent &event);
		void PrintPreviewEvent(wxMouseEvent &event);
		void ExportEvent(wxMouseEvent &event);
		void ExportKeyEvent(wxCommandEvent &event);
		void RunKeyEvent(wxCommandEvent &event);
		void FindProductKeyEvent(wxCommandEvent &event);
		void FindClientKeyEvent(wxCommandEvent &event);
		void PrintKeyEvent(wxCommandEvent &event);
		void PrintPreviewKeyEvent(wxCommandEvent &event);
		void InputClientEvent(wxCommandEvent &event);

		void Run();
		void FindProduct();
		void FindClient();
		void Print(const bool preview = false);
		void Export();
		void InputClient();

		void AddToStatisticsListCtrl(StatisticsEntity *statisticsEntity);
		void ClearStatisticsListCtrl();

		ToolbarButton    *runToolbarButton;
		ToolbarButton    *findClientToolbarButton;
		ToolbarButton    *findProductToolbarButton;
		ToolbarButton    *printToolbarButton;
		ToolbarButton    *printPreviewToolbarButton;
		ToolbarButton    *exportToolbarButton;
		DSListCtrl       *statisticsListCtrl;
		DSTextCtrl       *productIdTextCtrl;
		DSTextCtrl       *clientIdTextCtrl;
		DSTextCtrl       *ownClientIdTextCtrl;
		wxDatePickerCtrl *dateFromDatePickerCtrl;
		wxDatePickerCtrl *dateToDatePickerCtrl;
		wxComboBox       *selectionComboBox;

};

#endif /* STATISTICSMODULEPANEL_H_ */
