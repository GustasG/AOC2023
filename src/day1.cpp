#include "day1.hpp"

#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

namespace day1
{
	static void part1()
	{
		std::ifstream file("data/day1.txt");

		if (!file.is_open())
		{
			std::cerr << '\t' << "Unable to open file: " << "data/day1.txt" << '\n';
			return;
		}

		std::string line;
		int total = 0;

		while (std::getline(file, line))
		{
			int left = 0, right = 0;

			for (char c : line)
			{
				if (std::isdigit(c))
				{
					left = c - '0';
					break;
				}
			}

			for (auto it = line.rbegin(); it != line.rend(); ++it)
			{
				if (std::isdigit(*it))
				{
					right = *it - '0';
					break;
				}
			}

			total += 10 * left + right;
		}

		std::cout << '\t' << "part 1: " << total << '\n';
	}

	static int match(std::string_view line)
	{
		static const auto digits = std::to_array<std::pair<std::string_view, int>>({
			{ "one", 1 },
			{ "two", 2 },
			{ "three", 3 },
			{ "four", 4 },
			{ "five", 5 },
			{ "six", 6 },
			{ "seven", 7 },
			{ "eight", 8 },
			{ "nine", 9 }
		});

		for (size_t i = 0; i < line.size(); i++)
		{
			for (const auto& [key, value] : digits)
			{
				if (key == line.substr(i))
				{
					return value;
				}
			}
		}

		return 0;
	}

	static void part2()
	{
		std::ifstream file("data/day1.txt");

		if (!file.is_open())
		{
			std::cerr << '\t' << "Unable to open file: " << "data/day1.txt" << '\n';
			return;
		}

		std::string line;
		int total = 0;

		while (std::getline(file, line))
		{
			int left = 0, right = 0;

			for (size_t i = 0; i < line.size(); i++)
			{
				int result = 0;

				if (std::isdigit(line[i]))
				{
					result = line[i] - '0';
				}
				else
				{
					result = match(std::string_view(line.c_str(), i + 1));
				}

				if (result != 0)
				{
					if (left == 0)
					{
						left = result;
					}

					right = result;
				}
			}

			total += 10 * left + right;
		}

		std::cout << '\t' << "part 2: " << total << '\n';
	}

	void solution()
	{
		std::cout << "day 1:" << '\n';

		part1();
		part2();
	}
} // namespace day1