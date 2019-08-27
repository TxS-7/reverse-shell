#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <string>
#include "server.h"

class TCPServer: public Server {
public:
	TCPServer(int listenPort) : Server(listenPort) {}

	int setupServer();
private:
	static const ssize_t READ_BUF_SIZE = 4096;

	void sendCommand(const std::string&, bool output = true) const;
	void readMessage() const;
};

#endif // TCP_SERVER_H
