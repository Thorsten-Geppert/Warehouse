#ifndef IMPORT_LIBRARY_H_
#define IMPORT_LIBRARY_H_

#include <DSFile.h>

using namespace DynSoft;

class ImportLibrary {

	public:
		ImportLibrary();
		ImportLibrary(const wxString &filename);

		void ImportSetFilename(const wxString &filename);
		bool ImportStart();
		unsigned int ImportGetCount() const;
		virtual bool ImportRun(const wxString &line) = 0;
		wxString ImportGetLine(unsigned int row);
		bool ImportStop();
	
	protected:
		DSFile importFile;
		wxString filename;
		wxArrayString lines;
		unsigned int count;

};

#endif /* IMPORT_LIBRARY_H_	 */
