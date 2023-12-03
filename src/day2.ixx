module;

#include <charconv>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <string>
#include <vector>

import utility;

export module day2;

namespace day2
{
    struct Round
    {
        int red = 0;
        int green = 0;
        int blue = 0;
    };

    static std::vector<Round> parse_game(std::string_view line)
    {
        std::vector<Round> rounds;

        for (const auto& round_data : std::views::split(line.substr(line.find(':') + 1), ';'))
        {
            Round round;

            for (const auto& color_data : std::views::split(round_data, ','))
            {
                auto data = utility::sv_strip(std::string_view(color_data));

                auto pos = data.find(' ');

                int count;
                std::from_chars(data.data(), data.data() + pos, count);

                auto color = data.substr(pos + 1);

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

        size_t result = 0;

        for (const auto& [i, line] : utility::read_lines(file) | std::views::enumerate)
        {
            if (is_game_possible(parse_game(line), 12, 13, 14))
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

        auto view = utility::read_lines(file)
            | std::views::transform(parse_game)
            | std::views::transform(fewest_cubes)
            | std::views::common;

        int result = std::accumulate(view.begin(), view.end(), 0);

        std::cout << '\t' << "part 2: " << result << '\n';
    }

    export void solution()
    {
        std::cout << "day 2:" << '\n';

        part1();
        part2();
    }
} // namespace day2