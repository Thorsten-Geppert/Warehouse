#ifndef RPC_RUNNER_THREAD_H_
#define RPC_RUNNER_THREAD_H_

#include <wx/thread.h>
#include <DSPosixSocket.h>
#include "RuntimeInformationType.h"

using namespace DynSoft;

class RpcRunnerThreads;
class RpcRunnerThread : public wxThread {

	public:
		RpcRunnerThread(
			RuntimeInformationType *rit,
			RpcRunnerThreads *runnerThreads,
			DSPosixSocket *connection,
			const wxString &serverVersion,
			const wxString &clientVersion
		);
		~RpcRunnerThread();

		virtual ExitCode Entry();
		void Stop();

		wxString GetServerVersion();
		wxString GetClientVersion();
	
	protected:
		RuntimeInformationType *rit;
		RpcRunnerThreads *runnerThreads;
		DSPosixSocket *connection;
		bool stop;
		wxString clientVersion;
		wxString serverVersion;

};

#endif /* RPC_RUNNER_THREAD_H_ */
