#include "RpcRunnerThread.h"
#include "RpcRunnerThreads.h"

RpcRunnerThread::RpcRunnerThread(
	RuntimeInformationType *rit,
	RpcRunnerThreads *runnerThreads,
	DSPosixSocket *connection,
	const wxString &serverVersion,
	const wxString &clientVersion
) : wxThread(
), stop(
	false
) {
	this->rit = rit;
	this->runnerThreads = runnerThreads;
	this->connection = connection;
	this->serverVersion = serverVersion;
	this->clientVersion = clientVersion;
}

RpcRunnerThread::~RpcRunnerThread() {
	if(runnerThreads)
		runnerThreads->Remove(this);
}

wxThread::ExitCode RpcRunnerThread::Entry() {
printf("Connected\n");
	wxString tmp;
	
	do {
		connection->Send(GetServerVersion() + _N(" - ") + GetClientVersion() + _N("\n"));
		connection->Disconnect();
		delete connection;
		DS_PL(tmp);
	} while(!stop && connection->Receive(&tmp, '\n'));

	return NULL;
}

void RpcRunnerThread::Stop() {
	stop = true;
}

wxString RpcRunnerThread::GetServerVersion() {
	return serverVersion;
}

wxString RpcRunnerThread::GetClientVersion() {
	return clientVersion;
}
