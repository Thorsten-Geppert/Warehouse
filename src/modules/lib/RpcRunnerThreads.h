#ifndef RPC_RUNNER_THREADS_H_
#define RPC_RUNNER_THREADS_H_

#include <DSTemplateContainer.h>
#include "RuntimeInformationType.h"
#include "RpcRunnerThread.h"

using namespace DynSoft;

class RpcRunnerThreads {

	public:
		RpcRunnerThreads(RuntimeInformationType *rit);

		void Add(RpcRunnerThread *runnerThread);
		void Remove(const bool removePointer = false);
		void Remove(const int index, const bool removePointer = false);
		void Remove(RpcRunnerThread *runnerThread, const bool removePointer = false);
		unsigned int Count() const;

		void Stop();
		void Stop(const int index);
		void Stop(RpcRunnerThread *runnerThread);
		bool Stop(const int index, const unsigned int timeout);
		bool Stop(RpcRunnerThread *runnerThread, const unsigned int timeout);

		void Kill();
		void Kill(const int index);
		void Kill(RpcRunnerThread *runnerThread);
	
	protected:
		DSTemplateContainer<RpcRunnerThread *> runnerThreads;

		RuntimeInformationType *rit;

};

#endif /* RPC_RUNNER_THREADS_H_ */
