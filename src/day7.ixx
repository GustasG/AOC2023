module;

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <ranges>
#include <vector>
#include <map>
#include <numeric>

import utility;

export module day7;

namespace day7
{
    enum class HandType
    {
        HighCard,
        OnePair,
        TwoPairs,
        ThreeOfAKind,
        FullHouse,
        FourOfAKind,
        FiveOfAKind,
    };

    struct Hand
    {
        std::array<char, 5> cards;
        HandType type;
        size_t bid;
    };

    static std::vector<Hand> parse_file(std::string_view content)
    {
        return content
            | std::ranges::views::split('\n')
            | std::ranges::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::ranges::views::transform([] (std::string_view line) {
                auto view = line
                    | std::ranges::views::split(' ')
                    | std::ranges::views::transform([] (auto&& range) { return std::string_view(std::move(range)); });

                std::array<std::string_view, 2> parts;
                std::ranges::copy(view, parts.begin());

                std::array<char, 5> cards = { parts[0][0], parts[0][1], parts[0][2], parts[0][3], parts[0][4] };

                size_t bid;
                std::from_chars(parts[1].data(), parts[1].data() + parts[1].size(), bid);

                return Hand
                {
                    .cards = cards,
                    .type = HandType::FiveOfAKind,
                    .bid = bid
                };
            })
            | std::ranges::to<std::vector<Hand>>();
    }

    static HandType part1_calulate_type(const std::array<char, 5> cards)
    {
        std::map<char, int> counts;

        for (char card : cards)
        {
            counts[card]++;
        }

        switch (counts.size())
        {
            case 1:
            {
                return HandType::FiveOfAKind;
            }
            case 2:
            {
                for (const auto& [card, count] : counts)
                {
                    if (count == 4 || count == 1)
                        return HandType::FourOfAKind;
                    else if (count == 3 || count == 2)
                        return HandType::FullHouse;
                }
            }
            case 3:
            {
                for (const auto& [card, count] : counts)
                {
                    if (count == 3)
                        return HandType::ThreeOfAKind;
                    else if (count == 2)
                        return HandType::TwoPairs;
                }
            }
            case 4:
            {
                return HandType::OnePair;
            }
            case 5:
            {
                return HandType::HighCard;
            }
        }

        std::cerr << "Unknown hand type (" << std::string_view(cards.begin(), cards.end()) << ")" << '\n';
        std::abort();
    }

    static HandType part2_calulate_type(const std::array<char, 5>& cards)
    {


        return HandType::HighCard;
    }

    static size_t calculate_winnings(const std::vector<Hand>& hands)
    {
        size_t result = 0;

        for (const auto& [rank, hand] : hands | std::views::enumerate)
        {
            result += (rank + 1) * hand.bid;
        }

        return result;
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        auto hands = parse_file(content);
        std::for_each(hands.begin(), hands.end(), [] (Hand& hand) { hand.type = part1_calulate_type(hand.cards); });

        auto strength = [] (char c) {
            switch (c)
            {
                case 'A':
                    return 14;
                case 'K':
                    return 13;
                case 'Q':
                    return 12;
                case 'J':
                    return 11;
                case 'T':
                    return 10;
                default:
                    return c - '0';
            }
        };

        std::sort(hands.begin(), hands.end(), [&] (const Hand& lhs, const Hand& rhs) {
            if (lhs.type != rhs.type)
                return static_cast<int>(lhs.type) < static_cast<int>(rhs.type);

            for (const auto& [l, r] : std::views::zip(lhs.cards, rhs.cards))
            {
                if (l != r)
                    return strength(l) < strength(r);
            }

            return true;
        });

        size_t result = calculate_winnings(hands);

        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {
        auto hands = parse_file(content);
        std::for_each(hands.begin(), hands.end(), [] (Hand& hand) { hand.type = part2_calulate_type(hand.cards); });

        auto strength = [] (char c) {
            switch (c)
            {
                case 'A':
                    return 13;
                case 'K':
                    return 12;
                case 'Q':
                    return 11;
                case 'T':
                    return 10;
                case 'J':
                    return 1;
                default:
                    return c - '0';
                }
        };

        std::sort(hands.begin(), hands.end(), [&] (const Hand& lhs, const Hand& rhs) {
            if (lhs.type != rhs.type)
                return static_cast<int>(lhs.type) < static_cast<int>(rhs.type);

            for (const auto& [l, r] : std::views::zip(lhs.cards, rhs.cards))
            {
                if (l != r)
                    return strength(l) < strength(r);
            }

            return true;
        });

        size_t result = calculate_winnings(hands);

        std::cout << '\t' << "part 2: Not completed!" << result;
    }

    export void solution()
    {
        std::ifstream file("data/day7.txt");

        std::cout << "day 7:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day7.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
        part2(content);
    }
} // namespace day7