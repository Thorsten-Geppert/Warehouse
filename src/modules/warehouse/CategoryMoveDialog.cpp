#include "CategoryMoveDialog.h"
#include "../lib/CategoryEntity.h"
#include "Configuration.h"
#include "resources/images/WarehouseIcon.xpm"

BEGIN_EVENT_TABLE(CategoryMoveDialog, wxDialog)
	EVT_MENU(USE_EVENT, CategoryMoveDialog::UseKeyEvent)
	EVT_MENU(CANCEL_EVENT, CategoryMoveDialog::CancelKeyEvent)
END_EVENT_TABLE()

CategoryMoveDialog::CategoryMoveDialog(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &table
) {
	SetId(id);
	this->rit = rit;
	this->selectedCategoryId = 0;

	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("CategoryMoveDialog.xml")));
	wxXmlResource::Get()->LoadDialog(this, parent, _N("CategoryMoveDialog"));

	mainPanel           = XRCCTRL(*this, "mainPanel", wxPanel);
	mainBoxSizer        = static_cast<wxBoxSizer *>(mainPanel->GetSizer());
	useToolbarButton    = XRCCTRL(*this, "useToolbarButton", ToolbarButton);
	cancelToolbarButton = XRCCTRL(*this, "cancelToolbarButton", ToolbarButton);
	treePanel           = XRCCTRL(*this, "treePanel", wxPanel);
	treeBoxSizer        = static_cast<wxBoxSizer *>(treePanel->GetSizer());
	categoriesPanel     = new CategoriesPanel(treePanel, -1, rit, table);
	treeBoxSizer->Add(categoriesPanel, 1, wxEXPAND);

	mainBoxSizer->SetSizeHints(this);

	// Events
	CONNECT_TOOLBARBUTTON(cancelToolbarButton, CategoryMoveDialog::CancelEvent);
	CONNECT_TOOLBARBUTTON(useToolbarButton, CategoryMoveDialog::UseEvent);

	wxAcceleratorEntry acceleratorEntries[3];
	acceleratorEntries[0].Set(wxACCEL_NORMAL,  WXK_ESCAPE, CANCEL_EVENT);
	acceleratorEntries[1].Set(wxACCEL_CTRL,    (int) 'w',  CANCEL_EVENT);
	acceleratorEntries[2].Set(wxACCEL_CTRL,    (int) 'u',  USE_EVENT);
	wxAcceleratorTable acceleratorTable(3, acceleratorEntries);
	SetAcceleratorTable(acceleratorTable);

	SetSize(wxSize(400, 600));

	ICON();
}

// Events
void CategoryMoveDialog::CancelEvent(wxMouseEvent &event) {
	Cancel();
}

void CategoryMoveDialog::UseEvent(wxMouseEvent &event) {
	Use();
	SKIP();
}

void CategoryMoveDialog::CancelKeyEvent(wxCommandEvent &event) {
	Cancel();
}

void CategoryMoveDialog::UseKeyEvent(wxCommandEvent &event) {
	Use();
}

// Methods
void CategoryMoveDialog::Use() {
	CategoryEntity *categoryEntity = categoriesPanel->GetCategoryEntity();
	if(categoryEntity)
		selectedCategoryId = categoryEntity->Get(_N("categoryId"))->ToUnsignedInt();
	else
		selectedCategoryId = 0;

	EndModal(wxID_OK);
}

void CategoryMoveDialog::Cancel() {
	EndModal(wxID_CANCEL);
}

unsigned int CategoryMoveDialog::GetSelectedCategoryId() const {
	return selectedCategoryId;
}
