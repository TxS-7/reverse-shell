#include <string.h> // memset
#include <unistd.h> // close
#include <netdb.h> // getaddrinfo, freeaddrinfo
#include <sys/socket.h> // socket
#include <arpa/inet.h> // htons, inet_pton
#include "tcp_client.h"

int TCPClient::establishConnection() {
	// Create TCP socket
	this->sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sockfd < 0) {
		return -1;
	}

	struct sockaddr_in server_info;
	server_info.sin_family = AF_INET; // IPv4
	server_info.sin_port = htons(this->port);

	// Convert host address from text to binary
	int res = inet_pton(AF_INET, this->host, &(server_info.sin_addr));
	if (res > 0) {
		// Create the connection
		if (connect(this->sockfd, (struct sockaddr *) &server_info, sizeof(server_info)) < 0) {
			return -1;
		}
	} else if (res == 0) { // Invalid server IP
		// Try to treat host as a hostname
		struct addrinfo hints;
		memset(&hints, 0, sizeof(hints));
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_family = AF_INET;
		struct addrinfo *info = NULL;

		if (getaddrinfo(this->host, NULL, &hints, &info) != 0) {
			return -1;
		}

		struct sockaddr_in *sa = (struct sockaddr_in *) info->ai_addr;
		sa->sin_port = htons(this->port);
		if ((connect(this->sockfd, info->ai_addr, info->ai_addrlen)) < 0) {
			close(this->sockfd);
			freeaddrinfo(info);
			return -1;
		}
		freeaddrinfo(info);
	} else { // inet_pton failed
		close(this->sockfd);
		return -1;
	}

	return 0;
}
