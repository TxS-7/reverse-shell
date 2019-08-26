#include <iostream>
#include "tcp_client.h"

int main() {
	TCPClient *client = new TCPClient("127.0.0.1", 9999);
	client->start();
	delete client;
	return 0;
}
