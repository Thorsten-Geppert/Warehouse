#ifndef CATEGORIESPANEL_H
#define CATEGORIESPANEL_H

#include <wx/wx.h>
#include <wx/treectrl.h>
#include "../lib/CategoryEntity.h"
#include "../lib/RuntimeInformationType.h"

using namespace DynSoft;

class CategoriesPanel : public wxPanel {

	public:
		CategoriesPanel(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &table
		);

		bool NewCategory();
		bool EditCategory();
		bool RemoveCategory();

		void SetTable(const wxString &table);
		wxString GetTable() const;

		wxTreeCtrl *GetCategoriesTreeCtrl() const;
		CategoryEntity *GetCategoryEntity() const;

	protected:
		wxString table;
		RuntimeInformationType *rit;

		wxBoxSizer *mainBoxSizer;
		wxTreeCtrl *categoriesTreeCtrl;

};

#endif /* CATEGORIESPANEL_H */
