#ifndef REPORTGENERATORFRAME_H_
#define REPORTGENERATORFRAME_H_

#include <wx/wx.h>
#include "../lib/Toolbar.h"
#include "../lib/RuntimeInformationType.h"
#include "../lib/ReportTypes.h"
#include "../lib/HtmlEditPanel.h"

using namespace DynSoft;

class ReportGeneratorFrame : public wxFrame {

	public:
		ReportGeneratorFrame(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);
		~ReportGeneratorFrame();

	protected:
		enum {
			CLOSE_EVENT,
			SAVE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void CloseEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void AreasListBoxEvent(wxCommandEvent &event);
		void ElementsListBoxEvent(wxCommandEvent &event);
		void PlaceholdersListBoxEvent(wxCommandEvent &event);

		void Close();
		void Save();
		void AreasListBox();
		void ElementsListBox();
		void PlaceholdersListBox(const wxString &item);
		void SetHtmlEditPanel(HtmlEditPanel *htmlEditPanel);

		RuntimeInformationType *rit;
		ReportType reportType;
		HtmlEditPanel *htmlEditPanel;

		wxPanel          *mainPanel;
		wxBoxSizer       *mainBoxSizer;
		ToolbarButton    *saveToolbarButton;
		ToolbarButton    *closeToolbarButton;

		wxListBox        *areasListBox;
		wxListBox        *elementsListBox;
		wxListBox        *placeholdersListBox;
		
		wxPanel          *editPanel;
		wxBoxSizer       *editBoxSizer;

};

#endif /* REPORTGENERATORFRAME_H_ */
