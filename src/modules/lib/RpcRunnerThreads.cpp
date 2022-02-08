#include "RpcRunnerThreads.h"

RpcRunnerThreads::RpcRunnerThreads(RuntimeInformationType *rit) {
	this->rit = rit;
}

void RpcRunnerThreads::Add(RpcRunnerThread *runnerThread) {
	runnerThreads.Add(runnerThread);
}

void RpcRunnerThreads::Remove(const bool removePointer) {
	runnerThreads.Clear(removePointer);
}

void RpcRunnerThreads::Remove(const int index, const bool removePointer) {
	if(index >= 0)
		runnerThreads.Remove(index, removePointer);
}

void RpcRunnerThreads::Remove(RpcRunnerThread *runnerThread, const bool removePointer) {
	Remove(runnerThreads.Find(runnerThread), removePointer);
}

unsigned int RpcRunnerThreads::Count() const {
	return runnerThreads.GetCount();
}

void RpcRunnerThreads::Stop() {
	const unsigned int count = Count();
	for(unsigned int i = 0; i < count; i++)
		Stop(i);
}

void RpcRunnerThreads::Stop(const int index) {
	Stop(runnerThreads.Get(index));
}

void RpcRunnerThreads::Stop(RpcRunnerThread *runnerThread) {
	if(runnerThread)
		runnerThread->Stop();
}

bool RpcRunnerThreads::Stop(const int index, const unsigned int timeout) {
	return Stop(runnerThreads.Get(index), timeout);
}

bool RpcRunnerThreads::Stop(RpcRunnerThread *runnerThread, const unsigned int timeout) {
	if(!runnerThread)
		return false;
	
	runnerThread->Stop();
	wxSleep(timeout);
	return runnerThreads.Find(runnerThread) == -1;
}

void RpcRunnerThreads::Kill() {
	const unsigned int count = Count();
	for(unsigned int i = 0; i < count; i++)
		Kill(i);
}

void RpcRunnerThreads::Kill(const int index) {
	Kill(runnerThreads.Get(index));
}

void RpcRunnerThreads::Kill(RpcRunnerThread *runnerThread) {	
	if(runnerThread)
		runnerThread->Kill();
}
