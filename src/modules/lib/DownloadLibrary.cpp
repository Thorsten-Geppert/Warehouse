#include "DownloadLibrary.h"
#include <DSLib.h>
#include <DSFile.h>

DownloadLibrary::DownloadLibrary(const int bufferSize) {
	this->bufferSize = bufferSize;
	this->position   = 0;
	this->size       = 0;
}

bool DownloadLibrary::Start(
	const wxString &address,
	const unsigned short port,
	const wxString &url,
	const wxString &pathAndFilename,
	const long timeOut
) {
	this->position        = 0;
	this->size            = 0;
	this->pathAndFilename = pathAndFilename;

	connection.SetAddress(address, port);
	connection.SetTimeout(timeOut);
	if(!connection.Connect())
		return false;

	connection.Send(_N("GET ") + url + _N(" HTTP/1.1\r\nHost: ") + address + _N("\r\n\r\n"));

	wxString header;
	char buffer;
	while(connection.Receive(&buffer, 1) > 0) {
		header += buffer;
		if(header.Right(4) == _N("\r\n\r\n"))
			break;
	}

	header.Replace(_N("\r"), wxEmptyString);
	
	bool stop = false;
	wxString stringSize;
	wxArrayString lines(DSLib::Split(header, '\n'));
	const unsigned int count = lines.GetCount();
	for(unsigned int i = 0; i < count && !stop; i++) {
		if(lines[i].StartsWith(_N("Content-Length: "), &stringSize)) {
			stringSize.ToULongLong(&size);
			stop = true;
		}
	}

	return true;
}

unsigned long long DownloadLibrary::GetSize() const {
	return size;
}

unsigned long long DownloadLibrary::GetPosition() const {
	return position;
}

bool DownloadLibrary::Run(bool &wroteSuccess) {
	if(position >= size)
		return false;

	int received  = 0;
	char *content = static_cast<char *>(malloc(sizeof(char) * bufferSize));

	received = connection.Receive(content, bufferSize);

	DSFile file(pathAndFilename, position == 0 ? _N("w") : _N("a"));
	wroteSuccess = file.IsOpened();
	if(wroteSuccess) {
		wroteSuccess = file.Write(content, received);
		if(wroteSuccess)
			wroteSuccess = file.Close();
	}

	position += received;

	free(content);

	return wroteSuccess;
}

bool DownloadLibrary::Stop() {
	return connection.Disconnect();
}
