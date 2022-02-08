#ifndef FILESPANEL_H_
#define FILESPANEL_H_

#include <wx/wx.h>
#include <wx/treectrl.h>
#include <wx/splitter.h>
#include <DSListCtrl.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/FileEntity.h"
#include "../lib/CategoryEntity.h"
#include "CategoriesPanel.h"

using namespace DynSoft;

class FilesPanel : public wxPanel {

	public:
		FilesPanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &table
		);
		~FilesPanel();

		void NewCategory();
		void EditCategory();
		void RemoveCategory();
		void NewFile();
		void EditFile();
		void RemoveFile();
		void MoveFile();
		void OpenFile(const unsigned int *fileId = NULL);
		void DownloadFile(const unsigned int *fileId = NULL);
		void Update();
		void AddOrChangeToFilesListCtrl(FileEntity *fileEntity, const bool justAppend);
		DSListCtrl *GetFilesListCtrl() const;
		wxTreeCtrl *GetCategoriesTreeCtrl() const;
		bool IsFilePointerNotNull(const unsigned int *filePointer, wxString message = wxEmptyString);
		unsigned int *GetSelectedFileId();
	
	protected:
		void SelectCategoryEvent(wxTreeEvent &event);
		void ResizeSplitterWindowEvent(wxSizeEvent &event);

		void SelectCategory(CategoryEntity *categoryEntity);
		unsigned int LoadFiles(const unsigned int categoryId);
		void UnloadFiles();

		RuntimeInformationType *rit;
		unsigned int actualCategoryId;

		wxPanel          *categoriesPanel;
		wxBoxSizer       *categoriesBoxSizer;
		CategoriesPanel  *treeCategoriesPanel;
		wxSplitterWindow *contentSplitterWindow;
		wxTreeCtrl       *treeCtrl;
		DSListCtrl       *filesListCtrl;

};

#endif /* FILESPANEL_H_ */
