#include "CategoryController.h"
#include <DSPostgreSQLQuery.h>

CategoryController::CategoryController(
	RuntimeInformationType *rit,
	const wxString &table
) : Controller(
	rit,
	table,
	_N("categoryId"),  // Primary key
	_N("name")
) {
	LoadResource(rit->GRP(_N("sql"), _N("Categories.res")));
}

CategoryEntities *CategoryController::GetAll() {
	CategoryEntities *categoryEntities = new CategoryEntities;
	CategoryEntity *categoryEntity = NULL;

	CategoryController categoryController(rit, GetTable());
	categoryController.Get();
	while(categoryController.Run()) {
		categoryEntity = new CategoryEntity(rit, GetTable());
		categoryController.Get(categoryEntity);
		categoryEntities->Add(categoryEntity);
	}

	return categoryEntities;
}

bool CategoryController::ExistsInCategory(CategoryEntity &categoryEntity, const wxString &oldName) {
	if(categoryEntity.Get(_N("name"))->ToString() == oldName)
		return false;
	
	const unsigned int parentId = categoryEntity.Get(_N("parentId"))->ToUnsignedInt();
	DSPostgreSQLQuery query(ndb);
	query.Prepare(parentId == 0 ? GetResourceByName(_N("Categories.ExistsInRoot")) : GetResourceByName(_N("Categories.ExistsInCategory")));
	query.Bind(_N("table"), GetTable(), false);

	if(parentId != 0)
		query.Bind(_N("parentId"), parentId);
	query.Bind(_N("name"), categoryEntity.Get(_N("name"))->ToString());
	query.Exec();
	query.NextRecord();
	const bool exists = query.GetField(_N("counter")).ToUnsignedInt();
	query.FreeResult();
	
	return exists;
}

void CategoryController::GetPath(const unsigned int categoryId, wxArrayString *path, wxArrayString *ids) {
	if(path)
		path->Clear();
	if(ids)
		ids->Clear();

	if(categoryId == 0)
		return;

	unsigned int parentId = 0;

	CategoryController categoryController(rit, GetTable());
	categoryController.Get(DSCasts::ToString(categoryId));
	if(categoryController.Run()) {
		CategoryEntity categoryEntity(rit, GetTable());
		categoryController.Get(&categoryEntity);

		if(path)
			path->Add(categoryEntity.Get(_N("name"))->ToString());
		if(ids)
			ids->Add(categoryEntity.Get(_N("categoryId"))->ToString());
		parentId = categoryEntity.Get(_N("parentId"))->ToUnsignedInt();

		categoryController.Reset();
	}

	if(parentId != 0)
		GetPath(parentId, path, ids);
}

wxString CategoryController::ConvertPathToString(wxArrayString &path, const wxString &separator, const wxString &prepend) {
	wxString tmp(separator);
	if(!prepend.IsEmpty())
		tmp.Prepend(separator + prepend);

	const unsigned int count = path.GetCount();
	if(count > 0)
		for(unsigned int i = count - 1; (int)i >= 0; i--)
			tmp += path[i] + separator;
	
	return tmp;
}
