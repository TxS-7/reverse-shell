#include <iostream>
#include <string>
#include <cstdlib> // atoi
#include "tcp_client.h"
#include "log.h"

int main(int argc, char *argv[]) {
	if (argc < 3) {
		std::string usageMsg = argv[0];
		usageMsg += " <SERVER_IP> <SERVER_PORT>";
		Log::error("Usage: " + usageMsg);
		return -1;
	}

	TCPClient *client = new TCPClient(argv[1], atoi(argv[2]));
	client->start();
	delete client;
	return 0;
}
