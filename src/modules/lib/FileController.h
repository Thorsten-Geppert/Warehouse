#ifndef FILE_CONTROLLER_H_
#define FILE_CONTROLLER_H_

#include "Controller.h"
#include "RuntimeInformationType.h"
#include "FileEntity.h"

using namespace DynSoft;

class FileController : public Controller {

	public:
		FileController(RuntimeInformationType *rit);
		bool Add(FileEntity &fileEntity);
		bool Edit(FileEntity &fileEntity, const bool newOrigin);
		bool Remove(const unsigned int id);
		bool Remove(FileEntity &fileEntity);
		bool RemoveByCategory(const unsigned int categoryId);
		bool MoveFile(const unsigned int id, const unsigned int categoryId);
		long OpenFile(const unsigned int id, wxString *pathWithFilename = NULL);
		wxString OpenCommand(FileEntity &fileEntity);
		static wxString CreateValidFilename(const wxString &filename);
		bool ExportFile(const unsigned int id, const wxString &pathWithFilename);
		wxString GetFilename(const unsigned int id);

		void GetByCategory(const unsigned int categoryId);
	
	protected:
		wxString ConcatFilename(const wxString &filename, const wxString &extension, const unsigned int number = 0);

};

#endif /* FILE_CONTROLLER_H_ */
