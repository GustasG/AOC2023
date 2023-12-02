#include "day2.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <string>
#include <vector>

#include "utility.hpp"

namespace day2
{
	struct Round
	{
		int red = 0;
		int green = 0;
		int blue = 0;
	};

	static std::vector<Round> parse_game(const std::string& line)
	{
		std::vector<Round> rounds;
		std::istringstream rs(line.c_str() + line.find(':') + 1);
		std::string rd;

		while (std::getline(rs, rd, ';'))
		{
			std::istringstream cs(rd);
			std::string cd;
			Round round;

			while (std::getline(cs, cd, ','))
			{
				utility::strip(cd);
				auto pos = cd.find(' ');
				auto count = std::stoi(cd.substr(0, pos));
				auto color = cd.substr(pos + 1);

				if (color == "red")
					round.red = count;
				else if (color == "green")
					round.green = count;
				else if (color == "blue")
					round.blue = count;
			}

			rounds.push_back(round);
		}

		return rounds;
	}

	static bool is_game_possible(const std::vector<Round>& rounds, int red, int green, int blue)
	{
		for (const auto& round : rounds)
		{
			if (round.red > red || round.green > green || round.blue > blue)
			{
				return false;
			}
		}

		return true;
	}

	static int fewest_cubes(const std::vector<Round>& rounds)
	{
		int red = 0, green = 0, blue = 0;

		for (const auto& round : rounds)
		{
			red = std::max(red, round.red);
			green = std::max(green, round.green);
			blue = std::max(blue, round.blue);
		}

		return red * green * blue;
	}

	static void part1()
	{
		std::ifstream file("data/day2.txt");

		if (!file.is_open())
		{
			std::cerr << '\t' << "Unable to open file: " << "data/day2.txt" << '\n';
			return;
		}

		auto lines = utility::read_lines(file);
		size_t result = 0;

		for (size_t i = 0; i < lines.size(); i++)
		{
			if (is_game_possible(parse_game(lines[i]), 12, 13, 14))
			{
				result += i + 1;
			}
		}

		std::cout << '\t' << "part 1: " << result << '\n';
	}

	static void part2()
	{
		std::ifstream file("data/day2.txt");

		if (!file.is_open())
		{
			std::cerr << '\t' << "Unable to open file: " << "data/day2.txt" << '\n';
			return;
		}

		int result = 0;

		for (const auto& line : utility::read_lines(file))
		{
			result += fewest_cubes(parse_game(line));
		}

		std::cout << '\t' << "part 2: " << result << '\n';
	}

	void solution()
	{
		std::cout << "day 2:" << '\n';

		part1();
		part2();
	}
} // namespace day2