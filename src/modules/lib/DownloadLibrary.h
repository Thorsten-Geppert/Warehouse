#ifndef DOWNLOAD_LIBRARY_H_
#define DOWNLOAD_LIBRARY_H_

#include <DSPosixSocket.h>

using namespace DynSoft;

class DownloadLibrary {

	public:
		DownloadLibrary(const int bufferSize = 100);

		bool Start(
			const wxString &address,
			const unsigned short port,
			const wxString &url,
			const wxString &pathAndFilename,
			const long timeOut = 5
		);

		unsigned long long GetSize() const;
		unsigned long long GetPosition() const;

		bool Run(bool &wroteSuccess);

		bool Stop();
	
	protected:
		int bufferSize;
		unsigned long long size;
		DSPosixSocket connection;
		unsigned long long position;
		wxString pathAndFilename;

};

#endif /* DOWNLOAD_LIBRARY_H_ */
