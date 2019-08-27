#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server {
public:
	Server(int);
	virtual ~Server();

	int start();
	virtual int setupServer() = 0;
protected:
	int serverfd;
	int clientfd;
	int listenPort;
private:
	static const unsigned int FILE_TRANSFER_PORT = 45588;
	// Built-in commands names
	static const std::string CMD_UPLOAD; // Upload file
	static const std::string CMD_DOWNLOAD; // Download file
	static const std::string CMD_GETSUID; // Get files with SUID set
	static const std::string CMD_GETSGID; // Get files with SGID set
	static const std::string CMD_USERFILES; // Get files owned by user
	static const std::string CMD_GROUPFILES; // Get files owned by group
	static const std::string CMD_LISTCRON; // List cron jobs
	static const std::string CMD_SSHGEN; // Generate SSH key and create authorized_keys file to enable SSH connection
	static const std::string CMD_GETKEY; // View ~/.ssh/id_rsa
	static const std::string CMD_SUDOLIST; // List sudo permissions of current user
	static const std::string CMD_PORTS; // View listening ports
	static const std::string CMD_PS; // View running processes
	static const std::string CMD_GETPTY; // Spawn a PTY shell using the system's Python
	static const std::string LOCAL_SSH_KEY_NAME; // Local filename for generated SSH private key

	void readCommands() const;
	void runBuiltInCommand(const std::string&) const;
	virtual void sendCommand(const std::string&, bool output = true) const = 0;
	virtual void readMessage() const = 0;

	bool lastCommandOK() const;
	std::string getUser() const;
	std::string getGroup() const;
	std::string getHostname() const;
	bool programExists(const std::string&) const;
	// Built-in commands methods
	void uploadFile(const std::string&, const std::string&) const;
	void downloadFile(const std::string&, const std::string&) const;
	void getSUID() const;
	void getSGID() const;
	void listUserFiles(const std::string&) const;
	void listGroupFiles(const std::string&) const;
	void listCron() const;
	void generateSSHKey() const;
	void getSSHKey() const;
	void listSudoPermissions() const;
	void listPorts() const;
	void listProcesses() const;
	void spawnPTY() const;
};

#endif // SERVER_H
