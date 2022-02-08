#include "RpcThread.h"
#include "RpcRunnerThread.h"

RpcThread::RpcThread(
	RuntimeInformationType *rit,
	const wxString &version,
	const wxString &hostname,
	const unsigned int port,
	const unsigned int serverTimeoutInSeconds,
	const unsigned int clientTimeoutInSeconds
) : wxThread(
), listen(
	ERROR_NO
), runnerThreads(
	rit
) {
	this->rit = rit;
	this->version = version;
	this->hostname = hostname;
	this->port = port;
	this->serverTimeoutInSeconds = serverTimeoutInSeconds;
	this->clientTimeoutInSeconds = clientTimeoutInSeconds;
}

unsigned int RpcThread::Start() {
	unsigned int result = ERROR_NO;

	// XXX
	server.SetAddress(GetHostname(), GetPort());
	server.SetTimeout(GetServerTimeoutInSeconds());

	if(server.Bind()) {
		if(server.Listen(40)) {
			wxThreadError error = Create();
			if(error != wxTHREAD_NO_ERROR) {
				switch(error) {
					case wxTHREAD_NO_RESOURCE:
						result = CREATE_NO_RESOURCE;
						break;
					case wxTHREAD_RUNNING:
						result = CREATE_RUNNING;
						break;
					default:
						result = CREATE_ERROR_UNKNOWN;
						break;
				}
			} else {
				error = Run();
				if(error != wxTHREAD_NO_ERROR) {
					switch(error) {
						case wxTHREAD_NO_RESOURCE:
							result = RUN_NO_RESOURCE;
							break;
						case wxTHREAD_RUNNING:
							result = RUN_RUNNING;
							break;
						default:
							result = RUN_ERROR_UNKNOWN;
							break;
					}
				}
			}
		} else {
			listen = SOCKET_BIND;
		}
	} else {
		listen = SOCKET_LISTEN;
	}

	return result;
}

bool RpcThread::Stop(const unsigned int timeout) {
	runnerThreads.Stop();
	wxSleep(timeout);
	if(runnerThreads.Count() == 0) {
		Quit();
		server.Disconnect();

		return true;
	}

	return false;
}

bool RpcThread::Kill(const unsigned int timeout) {
	runnerThreads.Kill();
	wxSleep(timeout);
	if(runnerThreads.Count() == 0) {
		Quit();
		server.Disconnect();

		return true;
	}

	return false;
}

wxThread::ExitCode RpcThread::Entry() {
	bool run = true;
	wxString received;
	DSPosixSocket *client = NULL;
	while(run) {
		TestDestroy();
		client = server.Accept();
		if(client) {
			client->SetTimeout(GetClientTimeoutInSeconds());
			client->Receive(&received, '\n');

			if(received.Length() == 1 && received[0].GetValue() == '\4') {
				run = false;
				client->Disconnect();
				delete client;
			} else {
				RpcRunnerThread *rpcRunnerThread = new RpcRunnerThread(
					rit,
					&runnerThreads,
					client,
					GetVersion(),
					received
				);
				rpcRunnerThread->Create();
				rpcRunnerThread->Run();
				runnerThreads.Add(rpcRunnerThread);
			}
		}
	}

	return NULL;
}

void RpcThread::Quit() {
	wxString ip;
	unsigned int port;
	server.GetAddress(&ip, &port);

	DSPosixSocket client;
	client.SetAddress(ip, port);
	client.SetTimeout(GetClientTimeoutInSeconds());
	if(client.Connect()) {
		client.Send(_N("\4\n"));
		client.Disconnect();
	}
}

wxString RpcThread::GetHostname() const {
	return hostname;
}

wxString RpcThread::GetVersion() const {
	return version;
}

unsigned int RpcThread::GetPort() const {
	return port;
}

unsigned int RpcThread::GetServerTimeoutInSeconds() const {
	return serverTimeoutInSeconds;
}

unsigned int RpcThread::GetClientTimeoutInSeconds() const {
	return clientTimeoutInSeconds;
}
