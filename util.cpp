#include <string>

std::string trim(const std::string& str, const std::string& ignore) {
	// Find first character we don't want to ignore
	const size_t strStart = str.find_first_not_of(ignore);
	if (strStart == std::string::npos) {
		return "";
	}

	// Find the last character we want to keep
	const size_t strEnd = str.find_last_not_of(ignore);

	// Return only important string
	return str.substr(strStart, strEnd - strStart + 1);
}
