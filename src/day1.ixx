module;

#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string_view>
#include <string>

import utility;

export module day1;

namespace day1
{
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

    static void part1(std::string_view content)
    {
        utility::Timer t;

        auto view = content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform([] (std::string_view line) {
                auto it = line
                    | std::views::filter([] (char c) { return std::isdigit(c); })
                    | std::views::transform([] (char c) { return c - '0'; });

                auto l = it | std::views::take(1);
                auto r = it | std::views::reverse | std::views::take(1);

                return 10 * l.front() + r.front();
            })
            | std::views::common;

        int result = std::accumulate(view.begin(), view.end(), 0);

        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {
        utility::Timer t;

        auto view = content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform([] (std::string_view line) {
                auto it = line
                    | std::views::enumerate
                    | std::views::transform([&] (const auto& pair) {
                        if (std::isdigit(std::get<1>(pair)))
                            return std::get<1>(pair) - '0';

                        return match(std::string_view(line.data(), std::get<0>(pair) + 1));
                    })
                    | std::views::filter([] (int i) { return i != 0; });

                auto l = it | std::views::take(1);
                auto r = it | std::views::reverse | std::views::take(1);

                return 10 * l.front() + r.front();
            })
            | std::views::common;

        int result = std::accumulate(view.begin(), view.end(), 0);

        std::cout << '\t' << "part 2: " << result;
    }

    export void solution()
    {
        std::ifstream file("data/day1.txt");

        std::cout << "day 1:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day1.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
        part2(content);
    }
} // namespace day1