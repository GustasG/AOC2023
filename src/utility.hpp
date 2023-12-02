#pragma once

#include <fstream>
#include <string>
#include <vector>

namespace utility
{
	void lstrip(std::string& s);

	void rstrip(std::string& s);

	void strip(std::string& s);

	std::vector<std::string> read_lines(std::ifstream& file);
} // namespace utility