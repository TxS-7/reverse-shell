#ifndef CLIENT_H
#define CLIENT_H

class Client {
public:
	Client(char *, int);
	virtual ~Client();

	int start();
	virtual int establishConnection() = 0;
protected:
	int sockfd;
	char *host;
	int port;
};

#endif // CLIENT_H
