#include "PgHbaLibrary.h"

unsigned int PgHbaLibrary::ReadPgHba(const wxString &filename, wxArrayString &lines, PgHbaHostsType &hosts) {
	DSFile file(filename, _N("r"));
	if(!file.IsOpened())
		return PGHBA_ERROR_FILE_NOT_OPENED;

	wxArrayString tmp(file.ReadToArrayString());

	if(!file.Close())
		return PGHBA_ERROR_FILE_NOT_CLOSED;

	hosts.Clear(true);
	lines.Clear();

	const unsigned int count = tmp.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		if(!IsHostLine(tmp[i]))
			lines.Add(tmp[i]);
		else
			hosts.Add(ParseHostLine(tmp[i]));
	}

	return PGHBA_ERROR_OK;
}

bool PgHbaLibrary::IsHostLine(const wxString &line) {
	return line.StartsWith(_N("host"));
}

PgHbaHostType *PgHbaLibrary::ParseHostLine(wxString line) {
	line.Replace(_N("\t"), _N(" "));

	while(line.Contains(_N("  ")))
		line.Replace(_N("  "), _N(" "));

	wxArrayString splitted(DSLib::Split(line, ' '));
	const unsigned int count = splitted.GetCount();
	if(count < 4)
		return NULL;

	PgHbaHostType *pgHbaHostType = new PgHbaHostType;
	pgHbaHostType->SetType(splitted[0]);
	pgHbaHostType->SetDatabase(splitted[1]);
	pgHbaHostType->SetUser(splitted[2]);

	switch(count) {
		case 4:
			pgHbaHostType->SetMethod(splitted[3]);
			break;
		case 5:
			pgHbaHostType->SetAddress(splitted[3]);
			pgHbaHostType->SetMethod(splitted[4]);
			break;
		case 6:
			pgHbaHostType->SetAddress(splitted[3]);
			pgHbaHostType->SetSubnet(splitted[4]);
			pgHbaHostType->SetMethod(splitted[5]);
			break;
	}
	wxString address(pgHbaHostType->GetAddress());
	if(address.Find(_N("/")) != wxNOT_FOUND) {
		splitted = DSLib::Split(address, '/');
		pgHbaHostType->SetAddress(splitted[0]);
		pgHbaHostType->SetSubnet(splitted[1]);
	}

	return pgHbaHostType;
}

unsigned int PgHbaLibrary::WritePgHba(const wxString &filename, wxArrayString &lines, PgHbaHostsType &hosts) {
	DSFile file(filename, _N("w"));
	if(!file.IsOpened())
		return PGHBA_ERROR_FILE_NOT_OPENED;
	
	wxString tmp;
	unsigned int count = lines.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		if(i < count - 1)
			tmp = lines[i] + _N("\n");
		else
			tmp = lines[i];
		//if(!file.Write(lines[i] + _N("\n"))) {
		if(!file.Write(tmp)) {
			file.Close();
			return PGHBA_ERROR_FILE_NOT_WRITTEN;
		}
	}

	PgHbaHostType *pgHbaHostType = NULL;
	count = hosts.GetCount();
	for(unsigned int i = 0; i < count; i++) {
		pgHbaHostType = hosts.Get(i);
		if(!pgHbaHostType)
			continue;

		/*if(pgHbaHostType->GetSubnet().Find(_N(".")) == wxNOT_FOUND) {
			pgHbaHostType->SetAddress(pgHbaHostType->GetAddress() + _N("/") + pgHbaHostType->GetSubnet());
			pgHbaHostType->SetSubnet(wxEmptyString);
		}*/

		if(hosts.Get(i) && !file.Write(hosts.Get(i)->ToLine() + _N("\n"))) {
			file.Close();
			return PGHBA_ERROR_FILE_NOT_WRITTEN;
		}
	}

	if(!file.Close())
		return PGHBA_ERROR_FILE_NOT_CLOSED;

	return PGHBA_ERROR_OK;
}
