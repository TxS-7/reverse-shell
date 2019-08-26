#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include <sys/ioctl.h> // ioctl, TIOCGWINSZ
#include <unistd.h> // STDOUT_FILENO

class Log {
private:
	static const char * const RED;
	static const char * const GREEN;
	static const char * const BLUE;
	static const char * const YELLOW;
	static const char * const RESET;
public:
	static void error(const char *msg) {
		std::cout << RED << msg << RESET << std::endl;
	}

	static void success(const char *msg) {
		std::cout << GREEN << msg << RESET << std::endl;
	}

	static void info(const char *msg) {
		std::cout << BLUE << msg << RESET << std::endl;
	}

	static void error(const std::string msg) {
		std::cout << RED << msg << RESET << std::endl;
	}

	static void success(const std::string msg) {
		std::cout << GREEN << msg << RESET << std::endl;
	}

	static void info(const std::string msg) {
		std::cout << BLUE << msg << RESET << std::endl;
	}

	static void separator() {
		// Get terminal size (rows and columns)
		struct winsize size;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
		std::cout << YELLOW;
		std::string sep = "";
		for (unsigned short i = 0; i < size.ws_col; i++) {
			sep += "=";
		}
		std::cout << sep << RESET << std::endl;
	}
};

#endif // LOG_H
