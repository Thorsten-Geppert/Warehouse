#ifndef FILEEDITDIALOG_H_
#define FILEEDITDIALOG_H_

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <DSTextCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "../lib/FileEntity.h"

using namespace DynSoft;

class FileEditDialog : public wxDialog {

	public:
		FileEditDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			FileEntity *fileEntity = NULL
		);

	protected:
		enum {
			SAVE_EVENT,
			BROWSE_EVENT,
			CANCEL_EVENT
		};
		
		DECLARE_EVENT_TABLE()

		void OnClose(wxCloseEvent &event);

		void CloseEvent(wxMouseEvent &event);
		void SaveEvent(wxMouseEvent &event);
		void BrowseEvent(wxMouseEvent &event);
		void CloseKeyEvent(wxCommandEvent &event);
		void SaveKeyEvent(wxCommandEvent &event);
		void BrowseKeyEvent(wxCommandEvent &event);
		void FileInputEvent(wxCommandEvent &event);
		void TemplateSelectEvent(wxMouseEvent &event);

		void Close();
		void Save();
		void Browse();
		void FileInput();
		void TemplateSelect(wxObject *object);

		RuntimeInformationType *rit;
		bool changed;
		wxString path;
		FileEntity *fileEntity;

		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		ToolbarButton *saveToolbarButton;
		ToolbarButton *browseToolbarButton;
		ToolbarButton *closeToolbarButton;
		DSTextCtrl    *fileTextCtrl;
		DSTextCtrl    *nameTextCtrl;
		wxStaticText  *descriptionStaticText;
		DSTextCtrl    *descriptionTextCtrl;

};

#endif /* FILEEDITDIALOG_H_ */
