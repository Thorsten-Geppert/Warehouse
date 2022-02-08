#ifndef WAREHOUSE_DEMO_ENABLER_H_
#define WAREHOUSE_DEMO_ENABLER_H_

#include <wx/wx.h>
#include <wx/statline.h>

class WarehouseDemoEnabler : public wxApp {

	public:
		bool OnInit();

};

class WarehouseDemoEnablerFrame : public wxFrame {

	public:
		WarehouseDemoEnablerFrame();

	protected:
		enum {
			BROWSE_EVENT,
			ENABLE_EVENT,
			INFO_EVENT
		};

		DECLARE_EVENT_TABLE()

		void BrowseEvent(wxCommandEvent &event);
		void EnableEvent(wxCommandEvent &event);
		void InfoEvent(wxCommandEvent &event);

		void Browse();
		void Enable();
		void Info();
		bool ExistsExecutableFile(wxString path, wxString *fullPath = NULL);

		wxString pathSeparator;

		wxPanel *mainPanel;
		wxBoxSizer *mainBoxSizer;
		wxStaticBitmap *logoStaticBitmap;

		wxBoxSizer *borderBoxSizer;
		wxBoxSizer *dataBoxSizer;
		wxStaticText *headlineStaticText;
		wxStaticLine *headlineStaticLine;
		wxStaticText *infoStaticText;
		wxStaticLine *infoStaticLine;

		wxStaticText *pathStaticText;
		wxBoxSizer *pathBoxSizer;
		wxTextCtrl *pathTextCtrl;
		wxButton *pathButton;

		wxButton *enableButton;

		wxButton *infoButton;

};

#endif /* WAREHOUSE_DEMO_ENABLER_H_ */
