#include "LinkedFileController.h"
#include <DSTemplate.h>
#include "FileEntity.h"
#include "FileController.h"
#include "CategoryController.h"

LinkedFileController::LinkedFileController(
	RuntimeInformationType *rit,
	const wxString &table
) : Controller(
	rit,
	table,
	_N("linkedFileId")
) {
}

bool LinkedFileController::Save(
	DSPostgreSQLQuery &query,
	const unsigned int id,
	LinkedFileEntities &linkedFileEntities
) {
	if(id == 0)
		return false;
	
	bool ok = true;

	LinkedFileEntity *linkedFileEntity = NULL;
	LinkedFileController linkedFileController(rit, GetTable());
	unsigned int count = linkedFileEntities.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		linkedFileEntity = linkedFileEntities.Get(i);
		if(linkedFileEntity) {
			linkedFileEntity->SetValue(_N("id"), id);
			if(linkedFileEntity->Get(_N("linkedFileId"))->ToUnsignedInt() == 0)
				ok = linkedFileController.Insert(linkedFileEntity, true, NULL, &query);
			else
				ok = linkedFileController.Update(linkedFileEntity, wxEmptyString, true, NULL, &query);

			if(!ok)
				break;
		}
	}

	if(!ok)
		return ok;

	count = linkedFileEntities.GetRemoveCount();
	for(unsigned int i = 0; i < count; i++) {
		ok = linkedFileController.Delete(linkedFileEntities.GetRemove(i), wxEmptyString, &query);
		if(!ok)
			break;
	}

	return ok;
}

unsigned int LinkedFileController::Load(
	const unsigned int id,
	LinkedFileEntities &linkedFileEntities
) {
	unsigned int count = 0;
	LinkedFileEntity *linkedFileEntity = NULL;
	LinkedFileController linkedFileController(rit, GetTable());
	linkedFileController.Get(_N("id"), DSCasts::ToString(id));
	while(linkedFileController.Run()) {
		linkedFileEntity = new LinkedFileEntity(rit, GetTable());
		linkedFileController.Get(linkedFileEntity);
		linkedFileEntities.Add(linkedFileEntity);
		count++;
	}

	return count;
}

wxString LinkedFileController::Print(HtmlPrintoutLibrary &htmlPrintoutLibrary, LinkedFileEntities &linkedFileEntities) {
	const unsigned int count = linkedFileEntities.GetCount();
	if(count == 0)
		return wxEmptyString;
	
	wxString pathAndFile;
	FileEntity fileEntity(rit);
	FileController fileController(rit);
	wxArrayString pathArrayString;
	CategoryController categoryController(rit, _N("file_categories"));
	LinkedFileEntity *linkedFileEntity = NULL;
	DSTemplate linkedFileRow = htmlPrintoutLibrary.GetTemplateByDatabase(_N("LinkedFileRow"));
	for(unsigned int i = 0; i < count; i++) {
		linkedFileEntity = linkedFileEntities.Get(i);
		if(linkedFileEntity) {
			fileController.Get(linkedFileEntity->Get(_N("fileId"))->ToString());
			if(fileController.Run()) {
				fileController.Get(&fileEntity);
				fileController.Reset();

				categoryController.GetPath(fileEntity.Get(_N("categoryId"))->ToUnsignedInt(), &pathArrayString);
				pathAndFile = categoryController.ConvertPathToString(pathArrayString, _N("/"), _("Files")) + fileEntity.Get(_N("name"))->ToString();
			}

			linkedFileRow.SetVariable(_N("description"), linkedFileEntity->Get(_N("description"))->ToString(), true);
			linkedFileRow.SetVariable(_N("pathAndFile"), pathAndFile, true);
			linkedFileRow.Parse();
		}
	}

	return linkedFileRow.Get();
}
