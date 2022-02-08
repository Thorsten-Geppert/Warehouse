#include "FileController.h"
#include <DSPostgreSQLQuery.h>
#include <wx/mimetype.h>
#include <wx/filename.h>

FileController::FileController(
	RuntimeInformationType *rit
) : Controller(
	rit,
	_N("files"),
	_N("fileId"),
	_N("name")
) {
	LoadResource(rit->GRP(_N("sql"), _N("Files.res")));
}

bool FileController::Add(FileEntity &fileEntity) {
	DSPostgreSQLQuery query(ndb);
	query.Begin();

	const unsigned int oid = query.ImportLargeObject(fileEntity.Get(_N("origin"))->ToString());
	if(oid > 0) {
		fileEntity.SetValue(_N("fileOid"), oid);
		
		FileController fileController(rit);
		fileController.Insert(&fileEntity, true, NULL, &query);
	}

	if(!query.Commit()) {
		query.Rollback();
		return false;
	}

	return true;
}

bool FileController::Edit(FileEntity &fileEntity, const bool newOrigin) {
	DSPostgreSQLQuery query(ndb);
	query.Begin();

	bool ok = true;
	if(newOrigin) {
		const unsigned int oid = query.ImportLargeObject(fileEntity.Get(_N("origin"))->ToString(), fileEntity.Get(_N("fileOid"))->ToUnsignedInt());
		if(oid > 0)
			fileEntity.SetValue(_N("fileOid"), oid);
		else
			ok = false;
	}
		
	if(ok) {
		FileController fileController(rit);
		fileController.Update(&fileEntity, wxEmptyString, true, NULL, &query);
	}

	if(!query.Commit()) {
		query.Rollback();
		return false;
	}

	return true;
}

bool FileController::Remove(const unsigned int id) {
	FileController fileController(rit);
	fileController.Get(DSCasts::ToString(id));
	if(fileController.Run()) {
		FileEntity fileEntity(rit);
		fileController.Get(&fileEntity);
		fileController.Reset();
		
		return Remove(fileEntity);
	}

	return false;
}

bool FileController::RemoveByCategory(const unsigned int categoryId) {
	bool ok = true;

	FileEntity fileEntity(rit);
	FileController fileController(rit);
	fileController.Get(_N("categoryId"), DSCasts::ToString(categoryId));
	while(fileController.Run()) {
		fileController.Get(&fileEntity);
		
		if(!Remove(fileEntity))
			ok = false;
	}

	return ok;
}

bool FileController::Remove(FileEntity &fileEntity) {
	DSPostgreSQLQuery query(ndb);
	query.Begin();

	if(query.ExistsLargeObject(fileEntity.Get(_N("fileOid"))->ToUnsignedInt()))
		query.RemoveLargeObject(fileEntity.Get(_N("fileOid"))->ToUnsignedInt());

	if(!query.Commit())
		query.Rollback();

	FileController fileController(rit);
	return fileController.Delete(&fileEntity, wxEmptyString, &query);
}

void FileController::GetByCategory(const unsigned int categoryId) {
	wxString orderBy;
	GetOrderBy(&orderBy, NULL);

	DSQueryParser parser;
	parser.Prepare(categoryId == 0 ? GetResourceByName(_N("Files.GetWithoutCategory")) : GetResourceByName(_N("Files.GetByCategory")));
	parser.Bind(_N("categoryId"), categoryId);
	parser.Bind(_N("orderBy"), orderBy, false);
	Create(parser.GetParsedQuery());
}

long FileController::OpenFile(const unsigned int id, wxString *pathWithFilename) {
	long result = -1;

	FileController fileController(rit);
	fileController.Get(DSCasts::ToString(id));
	if(fileController.Run()) {
		FileEntity fileEntity(rit);
		fileController.Get(&fileEntity);
		fileController.Reset();

		const wxString filename(rit->GetTemporaryPath() + CreateValidFilename(fileEntity.Get(_N("name"))->ToString()));
		const wxString extension(fileEntity.Get(_N("extension"))->ToString());
		wxString fullFilename;

		unsigned int number = 0;
		do {
			fullFilename = ConcatFilename(filename, extension, number++);
		} while(wxFileName::FileExists(fullFilename));

		if(pathWithFilename)
			*pathWithFilename = fullFilename;

		const unsigned int fileOid = fileEntity.Get(_N("fileOid"))->ToUnsignedInt();
		DSPostgreSQLQuery query(ndb);
		query.Begin();
		if(query.ExistsLargeObject(fileOid))
			query.ExportLargeObject(fullFilename, fileOid);

		if(!query.Commit())
			query.Rollback();

		if(wxFileName::FileExists(fullFilename)) {
			wxMimeTypesManager mimeTypesManager;
			wxFileType *fileType = mimeTypesManager.GetFileTypeFromExtension(fileEntity.Get(_N("extension"))->ToString());
			if(fileType)
			#ifdef __WXMAC__
				result = wxExecute(fileType->GetOpenCommand(fullFilename), wxEXEC_SYNC);
			#else
				result = wxExecute(fileType->GetOpenCommand(fullFilename), wxEXEC_ASYNC);
			#endif
			delete fileType;
		}
	}

	return result;
}

wxString FileController::ConcatFilename(const wxString &filename, const wxString &extension, const unsigned int number) {
	if(number == 0)
		return filename + (extension.IsEmpty() ? wxString() : wxString(_N(".") + extension));
	return filename + _N("_") + DSCasts::ToString(number) + (extension.IsEmpty() ? wxString() : wxString(_N(".") + extension));
}

bool FileController::MoveFile(const unsigned int id, const unsigned int categoryId) {
	DSPostgreSQLQuery query(ndb);
	query.Prepare(GetResourceByName(_N("Files.Move")));
	query.Bind(_N("categoryId"), categoryId);
	query.Bind(_N("fileId"), id);
	return query.Exec(false);
}

wxString FileController::CreateValidFilename(const wxString &filename) {
	wxString newFilename;
	wxChar c;
	const unsigned int length = filename.Length();
	for(unsigned int i = 0; i < length; i++) {
		c = filename[i];
		if((c >= '0' && c <= '9') || (c >= 'A' && c <= 'z') || c == '_' || c == '.')
			newFilename += c;
		else if(c == ' ')
			newFilename += _N("_");
	}
	return newFilename;
}

bool FileController::ExportFile(const unsigned int id, const wxString &pathWithFilename) {
	bool result = false;

	FileController fileController(rit);
	fileController.Get(DSCasts::ToString(id));
	if(fileController.Run()) {
		FileEntity fileEntity(rit);
		fileController.Get(&fileEntity);
		fileController.Reset();

		const unsigned int fileOid = fileEntity.Get(_N("fileOid"))->ToUnsignedInt();
		DSPostgreSQLQuery query(ndb);
		query.Begin();
		if(query.ExistsLargeObject(fileOid))
			result = query.ExportLargeObject(pathWithFilename, fileOid);

		if(!query.Commit())
			query.Rollback();
	}

	return result;
}

wxString FileController::GetFilename(const unsigned int id) {
	wxString filename;

	FileController fileController(rit);
	fileController.Get(DSCasts::ToString(id));
	if(fileController.Run()) {
		FileEntity fileEntity(rit);
		fileController.Get(&fileEntity);
		fileController.Reset();

		filename = CreateValidFilename(fileEntity.Get(_N("name"))->ToString());
		const wxString extension(fileEntity.Get(_N("extension"))->ToString());
		filename = ConcatFilename(filename, extension);
	}

	return filename;
}
