#include "CategoryEntities.h"

CategoryEntities::CategoryEntities(
) : DSTemplateContainer<CategoryEntity *>(
) {
}

void CategoryEntities::FillTreeCtrl(wxTreeCtrl &treeCtrl, wxTreeItemId treeItem, const unsigned int select, const bool clean) {
	if(clean)
		treeCtrl.DeleteChildren(treeItem);

	CategoryEntity *categoryEntity    = reinterpret_cast<CategoryEntity *>(treeCtrl.GetItemData(treeItem));
	CategoryEntity *tmpCategoryEntity = NULL;
	const unsigned int categoryId     = categoryEntity->Get(_N("categoryId"))->ToUnsignedInt();
	unsigned int parentId             = 0;
	wxString name;
	wxTreeItemId tmpTreeItem;

	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		tmpCategoryEntity = Get(i);
		if(!tmpCategoryEntity)
			continue;

		parentId = tmpCategoryEntity->Get(_N("parentId"))->ToUnsignedInt();
		name     = tmpCategoryEntity->Get(_N("name"))->ToString();

		if(parentId == categoryId) {
			tmpTreeItem = treeCtrl.AppendItem(treeItem, name, 0, 1, reinterpret_cast<wxTreeItemData *>(tmpCategoryEntity));

			if(tmpCategoryEntity->Get(_N("categoryId"))->ToUnsignedInt() == select)
				treeCtrl.SelectItem(tmpTreeItem);

			if(HasChildren(tmpCategoryEntity->Get(_N("categoryId"))->ToUnsignedInt()))
				FillTreeCtrl(treeCtrl, tmpTreeItem, select);
		}

	}
	treeCtrl.Expand(treeItem);

}

bool CategoryEntities::HasChildren(const unsigned int id) {
	CategoryEntity *categoryEntity = NULL;
	const unsigned int count = GetCount();
	for(unsigned int i = 0; i < count; i++) {
		categoryEntity = Get(i);
		if(!categoryEntity)
			continue;

		if(categoryEntity->Get(_N("parentId"))->ToUnsignedInt() == id)
			return true;
	}
	return false;
}
