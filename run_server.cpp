#include <iostream>
#include <string>
#include <cstdlib> // atoi
#include "tcp_server.h"
#include "log.h"

int main(int argc, char *argv[]) {
	if (argc < 2) {
		std::string usageMsg = argv[0];
		usageMsg += " <LISTEN_PORT>";
		Log::error("Usage: " + usageMsg);
		return -1;
	}

	TCPServer *server = new TCPServer(atoi(argv[1]));
	server->start();
	delete server;
	return 0;
}
