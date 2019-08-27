#include <iostream>
#include <string>
#include <sys/socket.h> // socket, setsockopt, bind, listen, accept
#include <netinet/in.h> // sockaddr_in, INADDR_ANY
#include <arpa/inet.h> // htonl, htons
#include <unistd.h> // close, read, write
#include <string.h> // strlen
#include "tcp_server.h"
#include "log.h"

// TODO: Add handling for multiple connections
int TCPServer::setupServer() {
	// Create TCP socket
	this->serverfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverfd < 0) {
		return -1;
	}

	// Avoid TIME_WAIT state after connection is closed
	int reuse = 1;
	if (setsockopt(this->serverfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
		close(this->serverfd);
		return -1;
	}

	// Setup the port to listen for connections
	struct sockaddr_in server_info;
	server_info.sin_family = AF_INET;
	server_info.sin_addr.s_addr = htonl(INADDR_ANY);
	server_info.sin_port = htons(this->listenPort);
	if (bind(this->serverfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0) {
		close(this->serverfd);
		return -1;
	}

	if (listen(this->serverfd, 1) < 0) {
		close(this->serverfd);
		return -1;
	}

	Log::info("Listening for connections on port " + std::to_string(this->listenPort) + "...");

	// Wait for a connection and accept it
	struct sockaddr_in client_info;
	socklen_t client_len = sizeof(client_info);
	this->clientfd = accept(this->serverfd, (struct sockaddr *) &client_info, &client_len);
	if (this->clientfd < 0) {
		close(this->serverfd);
		return -1;
	}

	return 0;
}


void TCPServer::sendCommand(const std::string& cmd, bool output) const {
	// Send the command
	std::string terminatedCmd = cmd + "\n";
	const char *bytes = terminatedCmd.c_str();
	ssize_t bytesWritten = write(this->clientfd, bytes, strlen(bytes));
	if (bytesWritten == -1 || (size_t) bytesWritten < strlen(bytes)) {
		Log::error("write failed");
	}

	/*// Read the result
	std::string result = readMessage();
	if (output) {
		std::cout << result;
	}
	return result;*/
}


void TCPServer::readMessage() const {
	char resultBytes[TCPServer::READ_BUF_SIZE];
	while(true) {
		ssize_t bytesRead = read(this->clientfd, resultBytes, TCPServer::READ_BUF_SIZE - 1);
		if (bytesRead == -1) {
			Log::error("read failed");
			return;
		}
		resultBytes[bytesRead] = '\0';
		std::string result = resultBytes;
		std::cout << result << std::flush;
	}
}
