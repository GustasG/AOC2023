module;

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <set>
#include <unordered_map>
#include <vector>

import utility;

export module day4;

namespace day4
{
    struct Card
    {
        std::vector<int> winning_numebrs;
        std::vector<int> my_numbers;
    };

    static std::vector<int> parse_numbers(std::string_view numbers)
    {
        return numbers
            | std::views::split(' ')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform([] (std::string_view literal) { return utility::sv_strip(literal); })
            | std::views::filter([] (std::string_view literal) { return !literal.empty(); })
            | std::views::transform([] (std::string_view literal) {
                int value;
                std::from_chars(literal.data(), literal.data() + literal.size(), value);
                return value;
            })
            | std::ranges::to<std::vector<int>>();
    }

    static Card parse_card(std::string_view line)
    {
        line = line.substr(line.find(':') + 2); // skip Card x:
        size_t divider = line.find('|');
        auto winning_numbers = line.substr(0, divider);
        auto losing_numbers = line.substr(divider + 2);

        return Card
        {
            .winning_numebrs = parse_numbers(winning_numbers),
            .my_numbers = parse_numbers(losing_numbers)
        };
    }

    static int count_matching(Card& card)
    {
        std::sort(card.winning_numebrs.begin(), card.winning_numebrs.end());
        std::sort(card.my_numbers.begin(), card.my_numbers.end());

        std::vector<int> result;
        std::set_intersection(
            card.winning_numebrs.begin(), card.winning_numebrs.end(),
            card.my_numbers.begin(), card.my_numbers.end(),
            std::back_inserter(result)
        );

        return static_cast<int>(result.size());
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        auto view = content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform(parse_card)
            | std::views::transform([] (Card&& card) { return count_matching(card); })
            | std::views::filter([] (int points) { return points > 0; })
            | std::views::transform([] (int points) { return 1 << (points - 1); })
            | std::views::common;

        int result = std::accumulate(view.begin(), view.end(), 0);

        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {
        utility::Timer t;

        auto card_matches = content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform(parse_card)
            | std::views::transform([] (Card&& card) { return count_matching(card); })
            | std::views::enumerate
            | std::ranges::to<std::unordered_map<size_t, int>>();

        auto card_counts = std::views::iota(0)
            | std::views::take(card_matches.size())
            | std::views::transform([] (size_t i) { return std::make_pair(i, 1); })
            | std::ranges::to<std::unordered_map<size_t, int>>();

        for (const auto& [i, matches] : card_matches)
        {
            for (size_t j = i + 1; j <= i + matches; j++)
            {
                card_counts[j] += card_counts[i];
            }
        }

        auto view = std::views::values(card_counts)
            | std::views::common;

        int result = std::accumulate(view.begin(), view.end(), 0);

        std::cout << '\t' << "part 2: " << result;
    }

    export void solution()
    {
        std::ifstream file("data/day4.txt");

        std::cout << "day 4:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day4.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
        part2(content);
    }
} // namespace day4