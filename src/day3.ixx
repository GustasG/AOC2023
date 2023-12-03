module;

#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <set>
#include <string>
#include <vector>

import utility;

export module day3;

namespace day3
{
    struct Number
    {
        long long line;
        long long start;
        long long end;
        int number;
    };

    static std::vector<Number> find_numbers(const std::vector<std::string>& lines)
    {
        static const std::regex pattern(R"(\d+)");
        std::vector<Number> numbers;

        for (const auto& [i, line] : lines | std::views::enumerate)
        {
            for (auto it = std::sregex_iterator(line.begin(), line.end(), pattern); it != std::sregex_iterator(); ++it)
            {
                numbers.push_back(Number
                {
                    .line = i,
                    .start = it->position(),
                    .end = it->position() + it->length() - 1,
                    .number = std::stoi(it->str())
                });
            }
        }

        return numbers;
    }

    static bool is_adjacent(const Number& number, const std::vector<std::string>& lines)
    {
        static const auto directions = std::to_array<std::pair<int, int>>({
            { 0, -1 }, // up
            { 0, 1 }, // down
            { -1, 0 }, // left
            { 1, 0 }, // right
            { -1, -1 }, // up left
            { -1, 1 }, // up right
            { 1, -1 }, // down left
            { 1, 1 } // down right
         });

        for (const auto& [x, y] : directions)
        {
            for (long long i = number.start; i <= number.end; ++i)
            {
                long long row = number.line + y;
                long long col = i + x;

                if (row < 0 || row >= static_cast<long long>(lines.size()))
                    continue;

                if (col < 0 || col >= static_cast<long long>(lines[row].size()))
                    continue;

                if (lines[row][col] != '.' && !std::isdigit(lines[row][col]))
                    return true;
            }
        }

        return false;
    }

    static std::vector<const int*> adjacent_numbers(const std::vector<Number>& numbers, long long x_pos, long long y_pos)
    {
        static const auto directions = std::to_array<std::pair<int, int>>({
            { 0, -1 }, // up
            { 0, 1 }, // down
            { -1, 0 }, // left
            { 1, 0 }, // right
            { -1, -1 }, // up left
            { -1, 1 }, // up right
            { 1, -1 }, // down left
            { 1, 1 } // down right
        });

        std::set<const int*> result;

        for (const auto& [x, y] : directions)
        {
            long long row = x_pos + y;
            long long col = y_pos + x;

            for (const auto& number : numbers)
            {
                if (number.line == row && number.start <= col && number.end >= col)
                    result.insert(&number.number);
            }
        }

        return std::vector<const int*>(result.begin(), result.end());
    }

    static std::vector<std::vector<const int*>> find_gears(const std::vector<std::string>& lines, const std::vector<Number>& numbers)
    {
        std::vector<std::vector<const int*>> result;

        for (const auto& [i, line] : lines | std::views::enumerate)
        {
            for (const auto& [j, c] : line | std::views::enumerate)
            {
                if (c == '*')
                    result.push_back(adjacent_numbers(numbers, i, j));
            }
        }

        return result;
    }

    static void part1()
    {
        std::ifstream file("data/day3.txt");

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day3.txt" << '\n';
            return;
        }

        auto lines = utility::read_lines(file);
        auto numbers = find_numbers(lines);

        auto view = find_numbers(lines)
            | std::views::filter([&] (const Number& number) { return is_adjacent(number, lines); })
            | std::views::transform([] (const Number& number) { return number.number; })
            | std::views::common;

        int result = std::accumulate(view.begin(), view.end(), 0);

        std::cout << '\t' << "part 1: " << result << '\n';
    }

    static void part2()
    {
        std::ifstream file("data/day3.txt");

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day3.txt" << '\n';
            return;
        }

        auto lines = utility::read_lines(file);
        auto numbers = find_numbers(lines);

        auto values = find_gears(lines, numbers)
            | std::views::filter([] (const std::vector<const int*>& values) { return values.size() == 2; })
            | std::views::transform([] (const std::vector<const int*>& values) { return (*values[0]) * (*values[1]); })
            | std::views::common;

        int result = std::accumulate(values.begin(), values.end(), 0);

        std::cout << '\t' << "part 2: " << result << '\n';
    }

    export void solution()
    {
        std::cout << "day 3:" << '\n';

        part1();
        part2();
    }
} // namespace day3