#ifndef FILESMODULEPANEL_H_
#define FILESMODULEPANEL_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/ModulePanel.h"
#include "../lib/Toolbar.h"
#include "FilesPanel.h"

using namespace DynSoft;

class FilesModulePanel : public ModulePanel {

	public:
		FilesModulePanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit
		);

		virtual void Load();
		virtual void Unload();
	
	protected:
		enum {
			NEW_CATEGORY_EVENT,
			EDIT_CATEGORY_EVENT,
			REMOVE_CATEGORY_EVENT,
			NEW_FILE_EVENT,
			EDIT_FILE_EVENT,
			REMOVE_FILE_EVENT,
			MOVE_FILE_EVENT,
			DOWNLOAD_FILE_EVENT,
			UPDATE_FILE_EVENT
		};

		DECLARE_EVENT_TABLE()

		void NewCategoryEvent(wxMouseEvent &event);
		void EditCategoryEvent(wxMouseEvent &event);
		void RemoveCategoryEvent(wxMouseEvent &event);
		void NewFileEvent(wxMouseEvent &event);
		void RemoveFileEvent(wxMouseEvent &event);
		void MoveFileEvent(wxMouseEvent &event);
		void DownloadFileEvent(wxMouseEvent &event);
		void UpdateEvent(wxMouseEvent &event);
		void NewCategoryKeyEvent(wxCommandEvent &event);
		void EditCategoryKeyEvent(wxCommandEvent &event);
		void RemoveCategoryKeyEvent(wxCommandEvent &event);
		void NewFileKeyEvent(wxCommandEvent &event);
		void EditFileEvent(wxMouseEvent &event);
		void EditFileKeyEvent(wxCommandEvent &event);
		void OpenFileEvent(wxListEvent &event);
		void RemoveFileKeyEvent(wxCommandEvent &event);
		void MoveFileKeyEvent(wxCommandEvent &event);
		void DownloadFileKeyEvent(wxCommandEvent &event);
		void UpdateKeyEvent(wxCommandEvent &event);

		void NewCategory();
		void EditCategory();
		void RemoveCategory();
		void NewFile();
		void EditFile();
		void RemoveFile();
		void MoveFile();
		void DownloadFile();
		void Update();
		void OpenFile();

		ToolbarButton *newCategoryToolbarButton;
		ToolbarButton *editCategoryToolbarButton;
		ToolbarButton *removeCategoryToolbarButton;
		ToolbarButton *newFileToolbarButton;
		ToolbarButton *editFileToolbarButton;
		ToolbarButton *removeFileToolbarButton;
		ToolbarButton *downloadFileToolbarButton;
		ToolbarButton *moveFileToolbarButton;
		ToolbarButton *updateToolbarButton;
		wxPanel       *mainPanel;
		wxBoxSizer    *mainBoxSizer;
		FilesPanel    *filesPanel;
		DSListCtrl    *filesListCtrl;

};

#endif /* FILESMODULEPANEL_H_ */
