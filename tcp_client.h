#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "client.h"

class TCPClient: public Client {
public:
	TCPClient(char *host, int port) : Client(host, port) {}
	int establishConnection();
};

#endif // TCP_CLIENT_H
