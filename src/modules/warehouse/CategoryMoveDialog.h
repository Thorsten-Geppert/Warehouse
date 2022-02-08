#ifndef CATEGORYMOVEDIALOG_H_
#define CATEGORYMOVEDIALOG_H_

#include <wx/wx.h>
#include "../lib/RuntimeInformationType.h"
#include "../lib/Toolbar.h"
#include "CategoriesPanel.h"

using namespace DynSoft;

class CategoryMoveDialog : public wxDialog {

	public:
		CategoryMoveDialog(
			wxWindow *parent,
			wxWindowID id,
			RuntimeInformationType *rit,
			const wxString &table
		);
		unsigned int GetSelectedCategoryId() const;

	protected:
		enum {
			USE_EVENT,
			CANCEL_EVENT
		};
		
		DECLARE_EVENT_TABLE()

		void UseEvent(wxMouseEvent &event);
		void UseKeyEvent(wxCommandEvent &event);
		void CancelEvent(wxMouseEvent &event);
		void CancelKeyEvent(wxCommandEvent &event);

		void Cancel();
		void Use();

		RuntimeInformationType *rit;
		unsigned int selectedCategoryId;

		wxPanel         *mainPanel;
		wxBoxSizer      *mainBoxSizer;
		ToolbarButton   *useToolbarButton;
		ToolbarButton   *cancelToolbarButton;
		wxPanel         *treePanel;
		wxBoxSizer      *treeBoxSizer;
		CategoriesPanel *categoriesPanel;

};

#endif /* CATEGORYMOVEDIALOG_H_ */
