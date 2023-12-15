module;

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

import utility;

export module day9;

namespace day9
{
    static std::vector<std::vector<long long>> parse_file(std::string_view content)
    {
        return content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform([] (auto&& line) {
                return line
                    | std::views::split(' ')
                    | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
                    | std::views::transform([] (std::string_view literal) {
                        long long value;
                        std::from_chars(literal.data(), literal.data() + literal.size(), value);
                        return value;
                    })
                    | std::ranges::to<std::vector<long long>>();
            })
            | std::ranges::to<std::vector<std::vector<long long>>>();
    }

    static std::vector<long long> difference(const std::vector<long long>& numbers)
    {
        std::vector<long long> result;
        result.reserve(numbers.size());

        for (size_t i = 1; i < numbers.size(); i++)
        {
            result.push_back(numbers[i] - numbers[i - 1]);
        }

        return result;
    }

    static long long extrapolate(const std::vector<long long>& numbers)
    {
        std::vector<std::vector<long long>> layers;
        layers.push_back(numbers);

        while (!std::all_of(layers.back().begin(), layers.back().end(), [] (long long value) { return value == 0; }))
        {
            layers.push_back(difference(layers.back()));
        }

        layers.back().push_back(0);

        for (size_t i = layers.size() - 1; i > 0; i--)
        {
            layers[i - 1].push_back(layers[i - 1].back() + layers[i].back());
        }

        return layers.front().back();
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        auto view = parse_file(content)
            | std::views::transform(extrapolate)
            | std::views::common;

        long long result = std::accumulate(view.begin(), view.end(), static_cast<long long>(0));

        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {
        utility::Timer t;

        auto view = parse_file(content)
            | std::views::transform([] (std::vector<long long>& numbers) {
                std::reverse(numbers.begin(), numbers.end());
                return numbers;
            })
            | std::views::transform(extrapolate)
            | std::views::common;

        long long result = std::accumulate(view.begin(), view.end(), static_cast<long long>(0));

        std::cout << '\t' << "part 1: " << result;
    }

    export void solution()
    {
        std::ifstream file("data/day9.txt");

        std::cout << "day 9:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day9.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
        part2(content);
    }
} // namespace day9