#ifndef PGHBA_LIBRARY_H_
#define PGHBA_LIBRARY_H_

#include <DSFile.h>
#include "PgHbaHostsType.h"
#include "PgHbaHostType.h"

#define PGHBA_ERROR_OK               0
#define PGHBA_ERROR_FILE_NOT_OPENED  1
#define PGHBA_ERROR_FILE_NOT_CLOSED  2
#define PGHBA_ERROR_FILE_NOT_WRITTEN 3

using namespace DynSoft;

class PgHbaLibrary {

	public:
		unsigned int ReadPgHba(const wxString &filename, wxArrayString &lines, PgHbaHostsType &hosts);
		unsigned int WritePgHba(const wxString &filename, wxArrayString &lines, PgHbaHostsType &hosts);
	
	protected:
		bool IsHostLine(const wxString &line);
		PgHbaHostType *ParseHostLine(wxString line);

};

#endif /* PGHBA_LIBRARY_H_ */
