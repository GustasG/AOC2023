module;

#include <fstream>
#include <iostream>
#include <ranges>
#include <span>
#include <string_view>
#include <string>
#include <vector>

import utility;

export module day6;

namespace day6
{
    static size_t win_counts(size_t time, size_t distance)
    {
        size_t count = 0;

        for (size_t i = 0; i <= time; i++)
        {
            size_t d = i * (time - i);

            if (d > distance)
                count++;
        }

        return count;
    }

    static size_t win_counts(std::span<size_t> time, std::span<size_t> distance)
    {
        size_t result = 1;

        for (const auto& [t, d] : std::views::zip(time, distance))
        {
            result *= win_counts(t, d);
        }

        return result;
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        auto document = content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform([] (std::string_view line) { return line.substr(line.find(':') + 1); })
            | std::views::transform([] (std::string_view line) { return utility::sv_strip(line); })
            | std::views::transform([] (std::string_view line) {
                return line
                    | std::views::split(' ')
                    | std::views::transform([](auto&& range) { return std::string_view(std::move(range)); })
                    | std::views::filter([](std::string_view literal) { return !literal.empty(); })
                    | std::views::transform([](std::string_view literal) {
                        size_t value;
                        std::from_chars(literal.data(), literal.data() + literal.size(), value);
                        return value;
                    })
                    | std::ranges::to<std::vector<size_t>>();
            })
            | std::ranges::to<std::vector<std::vector<size_t>>>();

        size_t result = win_counts(document[0], document[1]);

        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {
        utility::Timer t;

        auto document = content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform([] (std::string_view line) { return line.substr(line.find(':') + 1); })
            | std::views::transform([] (std::string_view line) {
                return line
                    | std::views::filter([] (char c) { return c != ' '; })
                    | std::ranges::to<std::string>();
            })
            | std::views::transform([] (const std::string& line) { 
                size_t value;
                std::from_chars(line.data(), line.data() + line.size(), value);
                return value;
            })
            | std::ranges::to<std::vector<size_t>>();

        size_t result = win_counts(document[0], document[1]);

        std::cout << '\t' << "part 2: " << result;
    }

    export void solution()
    {
        std::ifstream file("data/day6.txt");

        std::cout << "day 6:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day6.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
        part2(content);
    }
} // namespace day6