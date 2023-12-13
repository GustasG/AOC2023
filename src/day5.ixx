module;

#include <algorithm>
#include <array>
#include <charconv>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string_view>
#include <string>
#include <unordered_map>
#include <vector>

import utility;

export module day5;

namespace day5
{
    struct Almanac
    {
        std::vector<size_t> seeds;
        std::vector<std::array<size_t, 3>> seed_to_soil_map;
        std::vector<std::array<size_t, 3>> soil_to_fertilizer_map;
        std::vector<std::array<size_t, 3>> fertilizer_to_water_map;
        std::vector<std::array<size_t, 3>> water_to_light_map;
        std::vector<std::array<size_t, 3>> light_to_temperature_map;
        std::vector<std::array<size_t, 3>> temperature_to_humidity_map;
        std::vector<std::array<size_t, 3>> humidity_to_location_map;
    };

    struct Interval
    {
        size_t start;
        size_t end;
    };

    static std::vector<size_t> parse_seed_map(std::string_view part)
    {
        return part
            | std::views::split(' ')
            | std::views::drop(1)
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform([] (std::string_view literal) {
                size_t value;
                std::from_chars(literal.data(), literal.data() + literal.size(), value);
                return value;
            })
            | std::ranges::to<std::vector<size_t>>();
    }

    static std::vector<std::array<size_t, 3>> parse_map(std::string_view part)
    {
        return part
            | std::views::split('\n')
            | std::views::drop(1)
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::views::transform([] (std::string_view literal) {
            auto rng = literal
                | std::views::split(' ')
                | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
                | std::views::transform([] (std::string_view s) {
                    size_t value;
                    std::from_chars(s.data(), s.data() + s.size(), value);
                    return value;
                });

                std::array<size_t, 3> result;
                std::ranges::copy(rng, result.begin());
                return result;
            })
            | std::ranges::to<std::vector<std::array<size_t, 3>>>();
    }

    static Almanac parse_file(std::string_view content)
    {
        auto parts = content
            | std::views::split(std::string_view("\n\n"))
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::ranges::to<std::vector<std::string_view>>();

        return Almanac
        {
            .seeds = parse_seed_map(parts[0]),
            .seed_to_soil_map = parse_map(parts[1]),
            .soil_to_fertilizer_map = parse_map(parts[2]),
            .fertilizer_to_water_map = parse_map(parts[3]),
            .water_to_light_map = parse_map(parts[4]),
            .light_to_temperature_map = parse_map(parts[5]),
            .temperature_to_humidity_map = parse_map(parts[6]),
            .humidity_to_location_map = parse_map(parts[7]),
        };
    }

    static size_t walk_map(const std::vector<std::array<size_t, 3>>& map, size_t value)
    {
        for (const auto& row : map)
        {
            size_t destination = row[0];
            size_t source = row[1];
            size_t length = row[2];

            if (value >= source && value < source + length)
            {
                size_t offset = value - source;

                return destination + offset;
            }
        }

        return value;
    }

    static std::vector<Interval> walk_range(const std::vector<std::array<size_t, 3>>& map, const std::vector<Interval>& range)
    {
        std::vector<Interval> result;



        return result;
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        Almanac almanac = parse_file(content);

        auto view = almanac.seeds
            | std::views::transform([&] (size_t seed) { return walk_map(almanac.seed_to_soil_map, seed); })
            | std::views::transform([&] (size_t seed) { return walk_map(almanac.soil_to_fertilizer_map, seed); })
            | std::views::transform([&] (size_t seed) { return walk_map(almanac.fertilizer_to_water_map, seed); })
            | std::views::transform([&] (size_t seed) { return walk_map(almanac.water_to_light_map, seed); })
            | std::views::transform([&] (size_t seed) { return walk_map(almanac.light_to_temperature_map, seed); })
            | std::views::transform([&] (size_t seed) { return walk_map(almanac.temperature_to_humidity_map, seed); })
            | std::views::transform([&] (size_t seed) { return walk_map(almanac.humidity_to_location_map, seed); });

        size_t result = std::ranges::min(view);

        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {
        utility::Timer t;

        Almanac almanac = parse_file(content);
        std::vector<Interval> intervals;

        for (size_t i = 0; i < almanac.seeds.size(); i += 2)
        {
            auto start = almanac.seeds[i];
            auto size = almanac.seeds[i + 1];
            intervals.push_back({ start, start + size });
        }

        auto fn = [&] (size_t seed) {
            seed = walk_map(almanac.seed_to_soil_map, seed);
            seed = walk_map(almanac.soil_to_fertilizer_map, seed);
            seed = walk_map(almanac.fertilizer_to_water_map, seed);
            seed = walk_map(almanac.water_to_light_map, seed);
            seed = walk_map(almanac.light_to_temperature_map, seed);
            seed = walk_map(almanac.temperature_to_humidity_map, seed);
            seed = walk_map(almanac.humidity_to_location_map, seed);
            return seed;
        };

        auto result = std::numeric_limits<size_t>::max();

        // TODO: implement proper interval intersection search and not use OpenMP
        for (const auto& interval : intervals)
        {
            #pragma omp parallel for reduction(min:result)
            for (ptrdiff_t i = interval.start; i < interval.end; i++)
            {
                result = std::min(result, fn(i));
            }
        }

        std::cout << '\t' << "part 2: " << result << '\n';
    }

    export void solution()
    {
        std::ifstream file("data/day5.txt");

        std::cout << "day 5:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day5.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
       // part2(content);
    }
} // namespace day5