#ifndef RPC_THREAD_H_
#define RPC_THREAD_H_

#include <wx/thread.h>
#include <DSPosixSocket.h>
#include "RuntimeInformationType.h"
#include "RpcRunnerThreads.h"

using namespace DynSoft;

class RpcThread : public wxThread {

	public:
		static const unsigned int ERROR_NO             = 0;
		static const unsigned int ERROR_UNKNOWN        = 1;
		static const unsigned int CREATE_ERROR_UNKNOWN = 2;
		static const unsigned int CREATE_NO_RESOURCE   = 3;
		static const unsigned int CREATE_RUNNING       = 4;
		static const unsigned int RUN_ERROR_UNKNOWN    = 5;
		static const unsigned int RUN_NO_RESOURCE      = 6;
		static const unsigned int RUN_RUNNING          = 7;
		static const unsigned int SOCKET_BIND          = 8;
		static const unsigned int SOCKET_LISTEN        = 9;

	public:
		RpcThread(
			RuntimeInformationType *rit,
			const wxString &version,
			const wxString &hostname,
			const unsigned int port,
			const unsigned int serverTimeoutInSeconds,
			const unsigned int clientTimeoutInSeconds
		);

		wxString GetHostname() const;
		wxString GetVersion() const;
		unsigned int GetPort() const;
		unsigned int GetServerTimeoutInSeconds() const;
		unsigned int GetClientTimeoutInSeconds() const;

		unsigned int Start();
		bool Stop(const unsigned int timeout);
		bool Kill(const unsigned int timeout);
		virtual ExitCode Entry();

		using wxThread::Kill;
	
	protected:
		RuntimeInformationType *rit;
		DSPosixSocket server;
		unsigned int listen;
		RpcRunnerThreads runnerThreads;
		wxString hostname;
		unsigned int port;
		unsigned int serverTimeoutInSeconds;
		unsigned int clientTimeoutInSeconds;
		wxString version;

		void Quit();

};

#endif /* RPC_THREAD_H_ */
