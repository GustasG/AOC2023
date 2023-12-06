module;

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

export module utility;

namespace utility
{
    export std::string_view sv_lstrip(std::string_view s)
    {
        s.remove_prefix(std::min(s.find_first_not_of(' '), s.size()));

        return s;
    }

    export std::string_view sv_rstrip(std::string_view s)
    {
        s.remove_suffix(std::min(s.size() - s.find_last_not_of(' ') - 1, s.size()));

        return s;
    }

    export std::string_view sv_strip(std::string_view s)
    {
        return sv_lstrip(sv_rstrip(s));
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

    export std::string read_file(std::ifstream& file)
	{
		std::string content;

		file.seekg(0, std::ios::end);
		content.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

		return content;
	}

    export class Timer
    {
    public:
        using Clock = std::chrono::steady_clock;

        Timer()
        {
            start();
        }

        ~Timer()
        {
            stop();
        }

        void start()
		{
            start_ = Clock::now();
		}

        void stop() const
        {
            auto end = Clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_);

            std::cout << " (elapsed " << duration.count() << "ms)" << '\n';
        }

    private:
        std::chrono::time_point<Clock> start_;
    };
} // namespace utility