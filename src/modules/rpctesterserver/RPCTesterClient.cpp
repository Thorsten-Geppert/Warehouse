#include <DSPosixSocket.h>

using namespace DynSoft;

int main() {
	DSPosixSocket client;
	client.SetAddress(_N("localhost"), 8181);
	client.SetTimeout(10);

	if(!client.Connect()) {
		DS_PLE("Could not connect to server");
		return 1;
	}

	client.Send(_N("\4\n"));

	client.Disconnect();

	return 0;
}
