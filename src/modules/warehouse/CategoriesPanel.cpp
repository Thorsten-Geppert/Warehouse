#include "CategoriesPanel.h"
#include <wx/xrc/xmlres.h>
#include <wx/imaglist.h>
#include "../lib/PermissionLibrary.h"
#include "../lib/CategoryEntities.h"
#include "../lib/CategoryController.h"
#include "TextInputDialog.h"
#include "YesNoDialog.h"

CategoriesPanel::CategoriesPanel(
	wxWindow *parent,
	wxWindowID id,
	RuntimeInformationType *rit,
	const wxString &table
) {
	this->rit = rit;
	SetTable(table);
	wxXmlResource::Get()->Load(rit->GRP(_N("gui"), _N("CategoriesPanel.xml")));
	wxXmlResource::Get()->LoadPanel(this, parent, _N("CategoriesPanel"));

	mainBoxSizer       = static_cast<wxBoxSizer *>(GetSizer());
	categoriesTreeCtrl = XRCCTRL(*this, "categoriesTreeCtrl", wxTreeCtrl);

	wxImageList *imageList = new wxImageList(16, 16);
	imageList->Add(wxBitmap(rit->GRP(_N("images"), _N("FolderClosed.png")), wxBITMAP_TYPE_PNG));
	imageList->Add(wxBitmap(rit->GRP(_N("images"), _N("FolderOpened.png")), wxBITMAP_TYPE_PNG));

	categoriesTreeCtrl->AssignImageList(imageList);
	categoriesTreeCtrl->AddRoot(_("Files"), 0, 1, reinterpret_cast<wxTreeItemData *>(new CategoryEntity(rit, wxEmptyString)));

	CategoryController categoryController(rit, GetTable());
	CategoryEntities *categoryEntities = categoryController.GetAll();
	categoryEntities->FillTreeCtrl(*categoriesTreeCtrl, categoriesTreeCtrl->GetRootItem(), 0);
	delete categoryEntities;

	mainBoxSizer->Layout();
}

bool CategoriesPanel::NewCategory() {
	if(!rit->Access(A_WRITE, _("You have no permission to create a new category."), this))
		return false;

	wxTreeItemId selection = categoriesTreeCtrl->GetSelection();
	if(!selection.IsOk())
		selection = categoriesTreeCtrl->GetRootItem();
	
	CategoryEntity *parentCategoryEntity = reinterpret_cast<CategoryEntity *>(categoriesTreeCtrl->GetItemData(selection));
	if(!parentCategoryEntity) {
		rit->Log(true, _N("CategoriesPanel"), _("Could not get selection category."), _("An error had occured."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not get selected category. An error has occured."),
			_("New category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}

	TextInputDialog *textInputDialog = new TextInputDialog(
		this,
		-1,
		rit,
		_("Add category"),
		_("Category name")
	);
	textInputDialog->CenterOnScreen();

	bool ok = false;

	CategoryEntity *categoryEntity = new CategoryEntity(rit, _N("file_categories"));
	categoryEntity->SetValue(_N("parentId"), parentCategoryEntity->Get(_N("categoryId"))->ToUnsignedInt());
	CategoryController categoryController(rit, _N("file_categories"));

	do {
		ok = textInputDialog->ShowModal() == wxID_OK;
		if(!ok)
			break;

		categoryEntity->SetValue(_N("name"), textInputDialog->GetValue());

		if(categoryController.ExistsInCategory(*categoryEntity)) {
			rit->Log(true, _N("CategoriesPanel"), _("Could not create category (") + categoryEntity->Get(_N("name"))->ToString() + _N(")."), _("A category with this name ") + categoryEntity->Get(_N("name"))->ToString() + _N(" exists."));
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("A category with this name allready exists in the parent category."),
				_("New category")
			);
			errorMessage->CenterOnScreen();
			errorMessage->ShowModal();
			errorMessage->Destroy();

			ok = false;
		}
	} while(!ok);

	textInputDialog->Destroy();

	if(!ok)
		return false;

	if(categoryController.Insert(categoryEntity)) {
		categoriesTreeCtrl->AppendItem(selection, categoryEntity->Get(_N("name"))->ToString(), 0, 1, reinterpret_cast<wxTreeItemData *>(categoryEntity));
		rit->Log(false, _N("CategoriesPanel"), _("New category was created."), categoryEntity->Get(_N("name"))->ToString());

		return true;
	} else {
		delete categoryEntity;

		rit->Log(true, _N("CategoriesPanel"), _("Could not save new category (") + categoryEntity->Get(_N("name"))->ToString() + _N(")."), _("Database error."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not save new category. Database error."),
			_("Save category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	}

	return false;
}

bool CategoriesPanel::EditCategory() {
	if(!rit->Access(A_WRITE, _("You have no permission to edit a new category."), this))
		return false;

	wxTreeItemId selection = categoriesTreeCtrl->GetSelection();
	if(!selection.IsOk()) {
		rit->Log(true, _N("CategoriesPanel"), _("Could not edit category."), _("No category was selected."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the category you want to rename."),
			_("Edit category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}

	if(selection == categoriesTreeCtrl->GetRootItem()) {
		rit->Log(true, _N("CategoriesPanel"), _("Could not edit category."), _("The root category cannot be renamed."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("You cannot rename the root category."),
			_("Edit category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}
	
	CategoryEntity *categoryEntity = reinterpret_cast<CategoryEntity *>(categoriesTreeCtrl->GetItemData(selection));
	if(!categoryEntity) {
		rit->Log(true, _N("CategoriesPanel"), _("Could not edit category."), _("An error has occured."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not get selected category. An error has occured."),
			_("New category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}

	const wxString oldName(categoryEntity->Get(_N("name"))->ToString());
	TextInputDialog *textInputDialog = new TextInputDialog(
		this,
		-1,
		rit,
		_("Rename category"),
		_("Category name"),
		false,
		oldName
	);
	textInputDialog->CenterOnScreen();

	bool ok = false;

	CategoryController categoryController(rit, _N("file_categories"));

	do {
		ok = textInputDialog->ShowModal() == wxID_OK;
		if(!ok)
			break;

		categoryEntity->SetValue(_N("name"), textInputDialog->GetValue());

		if(categoryController.ExistsInCategory(*categoryEntity, oldName)) {
			rit->Log(true, _N("CategoriesPanel"), _("Could not edit category (") + categoryEntity->Get(_N("name"))->ToString() + _N(")."), _("A category with this name allready exists in the parent category."));
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("A category with this name allready exists in the parent category."),
				_("Edit category")
			);
			errorMessage->CenterOnScreen();
			errorMessage->ShowModal();
			errorMessage->Destroy();

			ok = false;
		}
	} while(!ok);

	textInputDialog->Destroy();

	if(!ok)
		return false;

	if(categoryController.Update(categoryEntity)) {
		categoriesTreeCtrl->SetItemText(selection, categoryEntity->Get(_N("name"))->ToString());
		rit->Log(false, _N("CategoriesPanel"), _("Category was edited."), categoryEntity->Get(_N("name"))->ToString());

		return true;
	} else {
		delete categoryEntity;

		rit->Log(true, _N("CategoriesPanel"), _("Could not edit category (") + categoryEntity->Get(_N("name"))->ToString() + _N(")."), _("Database error."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not edit category. Database error."),
			_("Save category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();
	}

	return false;
}

bool CategoriesPanel::RemoveCategory() {
	if(!rit->Access(A_DELETE, _("You have no permission to remove a category."), this))
		return false;

	wxTreeItemId selection = categoriesTreeCtrl->GetSelection();
	if(!selection.IsOk()) {
		rit->Log(true, _N("CategoriesPanel"), _("Could not remove category."), _("No category selected."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Please select the category you want to remove."),
			_("Remove category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}

	if(selection == categoriesTreeCtrl->GetRootItem()) {
		rit->Log(true, _N("CategoriesPanel"), _("Could not remove category."), _("Cannot remove the root category."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("You cannot remove the root category."),
			_("Remove category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}
	
	CategoryEntity *categoryEntity = reinterpret_cast<CategoryEntity *>(categoriesTreeCtrl->GetItemData(selection));
	if(!categoryEntity) {
		rit->Log(true, _N("CategoriesPanel"), _("Could not remove category."), _("An error has occured (no category was selected)."));
		wxMessageDialog *errorMessage = new wxMessageDialog(
			this,
			_("Could not get selected category. An error has occured."),
			_("Remove category")
		);
		errorMessage->CenterOnScreen();
		errorMessage->ShowModal();
		errorMessage->Destroy();

		return false;
	}
	YesNoDialog *removeYesNoDialog = new YesNoDialog(
		this,
		-1,
		rit,
		_("Do you really want to remove the selected category? All files in this category will be deleted!"),
		_("Remove category"),
		_("Remove"),
		_("Cancel")
	);
	removeYesNoDialog->CenterOnScreen();
	const bool ok = removeYesNoDialog->ShowModal() == wxID_YES;
	removeYesNoDialog->Destroy();

	if(ok) {
		CategoryController categoryController(rit, _N("file_categories"));
		if(categoryController.Delete(categoryEntity)) {
			rit->Log(false, _N("CategoriesPanel"), _("Category was removed."), categoryEntity->Get(_N("name"))->ToString());
			categoriesTreeCtrl->Delete(selection);

			return true;
		} else {
			rit->Log(true, _N("CategoriesPanel"), _("Could not remove category (") + categoryEntity->Get(_N("name"))->ToString() + _N(")."), _("An database error has occured."));
			wxMessageDialog *errorMessage = new wxMessageDialog(
				this,
				_("Could not remove selected category. An database error has occured."),
				_("Remove category")
			);
			errorMessage->CenterOnScreen();
			errorMessage->ShowModal();
			errorMessage->Destroy();
		}
	}

	return false;
}

void CategoriesPanel::SetTable(const wxString &table) {
	this->table = table;
}
wxString CategoriesPanel::GetTable() const {
	return table;
}

wxTreeCtrl *CategoriesPanel::GetCategoriesTreeCtrl() const {
	return categoriesTreeCtrl;
}

CategoryEntity *CategoriesPanel::GetCategoryEntity() const {
	CategoryEntity *categoryEntity = reinterpret_cast<CategoryEntity *>(categoriesTreeCtrl->GetItemData(categoriesTreeCtrl->GetSelection()));
	if(!categoryEntity)
		categoryEntity = reinterpret_cast<CategoryEntity *>(categoriesTreeCtrl->GetItemData(categoriesTreeCtrl->GetRootItem()));
	return categoryEntity;
}
