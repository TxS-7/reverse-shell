#include <iostream>
#include "tcp_client.h"
#include "log.h"

int main() {
	TCPClient *client = new TCPClient("127.0.0.1", 9999);
	client->start();
	delete client;
	std::cout << "OK" << std::endl;
	Log::separator();
	Log::success("aaa");
	Log::error("nooo");
	Log::info("INFO");
	Log::separator();
	return 0;
}
