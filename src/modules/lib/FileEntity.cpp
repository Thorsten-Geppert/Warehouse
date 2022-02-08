#include "FileEntity.h"
#include <wx/filename.h>
#include <wx/mimetype.h>

FileEntity::FileEntity(
	RuntimeInformationType *rit
) : Entity(
	rit,
	_N("files"),
	_N("fileId")
) {
	Set(_N("fileId"), 0U);
	Set(_N("categoryId"), 0U);
	Set(_N("name"));
	Set(_N("description"));
	Set(_N("origin"));
	Set(_N("mimetype"));
	Set(_N("extension"));
	Set(_N("size"), 0U);
	Set(_N("humanSize"));
	Set(_N("fileOid"), 0U);
}

FileEntity *FileEntity::CreateByFile(
	RuntimeInformationType *rit,
	const wxString &filename,
	const unsigned int fileId,
	const unsigned int categoryId,
	const wxString &name,
	const wxString &description
) {
	FileEntity *fileEntity = NULL;
	if(filename.IsEmpty())
		return fileEntity;

	wxFileName fileName(filename);
	if(!fileName.FileExists())
		return fileEntity;

	const wxString extension(fileName.GetExt());
	
	wxString mimetype;
	wxMimeTypesManager mimeTypeManager;
	wxFileType *fileType = mimeTypeManager.GetFileTypeFromExtension(extension);
	if(fileType) {
		fileType->GetMimeType(&mimetype);
		delete fileType;
	}
	
	fileEntity = new FileEntity(rit);
	fileEntity->SetValue(_N("fileId"), fileId);
	fileEntity->SetValue(_N("categoryId"), categoryId);
	fileEntity->SetValue(_N("name"), name);
	fileEntity->SetValue(_N("description"), description);
	fileEntity->SetValue(_N("origin"), filename);
	fileEntity->SetValue(_N("mimetype"), mimetype);
	fileEntity->SetValue(_N("extension"), extension);
	fileEntity->SetValue(_N("size"), fileName.GetSize().ToULong());
	fileEntity->SetValue(_N("humanSize"), fileName.GetHumanReadableSize(_("n/a")));

	return fileEntity;
}
