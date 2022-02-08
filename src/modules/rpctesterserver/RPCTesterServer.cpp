#include <wx/socket.h>
#include "../lib/RpcThread.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
	wxInitialize();

	RpcThread *rpcThread = new RpcThread(NULL, _N("0.0.1"), _N("localhost"), 8181, 10, 5);
	printf("Status: %i\n", rpcThread->Start());

	//wxSleep(5);
	//rpcThread->Stop(15);

	while(1);
	
	return 0;
}
