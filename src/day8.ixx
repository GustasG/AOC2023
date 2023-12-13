module;

#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <string_view>
#include <string>
#include <unordered_map>

import utility;

export module day8;

namespace day8
{
    struct Node
    {
        std::string left;
        std::string right;
    };

    static std::unordered_map<std::string, Node> parse_file(std::string_view content)
    {
        static const std::regex pattern("(.+) = \\((.+), (.+)\\)");

        return content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string(std::from_range, std::move(range)); })
            | std::views::transform([&] (const std::string& line) {
                std::smatch match;
                std::regex_match(line, match, pattern);

                return std::pair(std::string(match[1]), Node{ match[2], match[3] });
            })
            | std::ranges::to<std::unordered_map<std::string, Node>>();
    }

    static size_t walk(const std::unordered_map<std::string, Node>& network, std::string_view instructions, const std::string& starting)
    {
        size_t result = 0;
        std::string current = starting;

        while (!current.ends_with('Z'))
        {
            for (char c : instructions)
            {
                result += 1;

                if (c == 'L')
                    current = network.at(current).left;
                else
                    current = network.at(current).right;

                if (current.ends_with('Z'))
                    break;
            }
        }

        return result;
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        auto parts = content | std::views::split(std::string_view("\n\n"));
        auto instructions = std::string_view(*parts.begin());
        auto nodes = std::string_view(*std::ranges::next(parts.begin()));
        auto network = parse_file(nodes);

        size_t result = walk(network, instructions, "AAA");
 
        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {
        utility::Timer t;

        auto parts = content | std::views::split(std::string_view("\n\n"));
        auto instructions = std::string_view(*parts.begin());
        auto nodes = std::string_view(*std::ranges::next(parts.begin()));
        auto network = parse_file(nodes);

        auto view = network
            | std::views::filter([] (const auto& pair) { return pair.first.ends_with('A'); })
            | std::views::keys
            | std::views::transform([&] (const std::string& starting) { return walk(network, instructions, starting); })
            | std::views::common;

        size_t result = std::accumulate(view.begin(), view.end(), static_cast<size_t>(1), std::lcm<size_t, size_t>);

        std::cout << '\t' << "part 2: " << result;
    }

    export void solution()
    {
        std::ifstream file("data/day8.txt");

        std::cout << "day 8:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day8.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
        part2(content);
    }
} // namespace day8