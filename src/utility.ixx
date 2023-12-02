module;

#include <fstream>
#include <string>
#include <vector>

export module utility;

namespace utility
{
    export void lstrip(std::string& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [] (char ch) {
            return !std::isspace(ch);
        }));
    }

    export void rstrip(std::string& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [] (char ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

	export void strip(std::string& s)
	{
        lstrip(s);
        rstrip(s);
	}

    export std::vector<std::string> read_lines(std::ifstream& file)
    {
        std::vector<std::string> lines;
        std::string line;

        while (std::getline(file, line))
        {
			lines.push_back(line);
		}

        return lines;
    }
} // namespace utility