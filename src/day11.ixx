module;

#include <fstream>
#include <iostream>
#include <numeric>
#include <queue>
#include <ranges>
#include <string_view>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

import utility;

export module day11;

struct Vertex
{
    int x;
    int y;

    friend auto operator<=>(const Vertex&, const Vertex&) = default;
};

template<>
struct std::hash<Vertex>
{
    std::size_t operator()(const Vertex& v) const noexcept
    {
        std::size_t h1 = std::hash<int>{}(v.x);
        std::size_t h2 = std::hash<int>{}(v.y);
        return h1 ^ (h2 << 1);
    }
};

namespace day11
{
    static std::vector<std::string_view> parse_file(std::string_view content)
    {
        return content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::ranges::to<std::vector<std::string_view>>();
    }

    static std::vector<int> find_empty_rows(std::span<std::string_view> map, int increment)
    {
        std::vector<int> result;
        int total = 0;

        for (std::string_view line : map)
        {
            if (!line.contains('#'))
                total += increment;

            result.push_back(total);
        }

        return result;
    }

    static bool contains(std::span<std::string_view> lines, size_t column, char symbol)
    {
        for (const auto& line : lines)
        {
            if (line[column] == symbol)
                return true;
        }

        return false;
    }

    static std::vector<int> find_empty_columns(std::span<std::string_view> map, int increment)
    {
        std::vector<int> result;
        int total = 0;

        for (size_t i = 0; i < map[0].size(); i++)
        {
            if (!contains(map, i, '#'))
                total += increment;

            result.push_back(total);
        }

        return result;
    }

    static std::vector<Vertex> find_galaxies(std::span<std::string_view> map, int increment)
    {
        std::vector<int> empty_rows = find_empty_rows(map, increment);
        std::vector<int> empty_columns = find_empty_columns(map, increment);

        std::vector<Vertex> vertices;

        for (size_t i = 0; i < map.size(); i++)
        {
            for (size_t j = 0; j < map[i].size(); j++)
            {
                if (map[i][j] == '#')
                {
                    vertices.push_back(Vertex
                    {
                        .x = static_cast<int>(j) + empty_columns[j],
                        .y = static_cast<int>(i) + empty_rows[i]
                    });
                }
            }
        }

        return vertices;
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        std::vector<std::string_view> lines = parse_file(content);
        std::vector<Vertex> vertices = find_galaxies(lines, 1);
        
        int result = 0;

        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (size_t j = i + 1; j < vertices.size(); j++)
            {
                result += std::abs(vertices[i].x - vertices[j].x) + std::abs(vertices[i].y - vertices[j].y);
            }
        }

        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {
        utility::Timer t;

        std::vector<std::string_view> lines = parse_file(content);
        std::vector<Vertex> vertices = find_galaxies(lines, 1'000'000 - 1);

        size_t result = 0;

        for (size_t i = 0; i < vertices.size(); i++)
        {
            for (size_t j = i + 1; j < vertices.size(); j++)
            {
                result += static_cast<size_t>(std::abs(vertices[i].x - vertices[j].x)) + static_cast<size_t>(std::abs(vertices[i].y - vertices[j].y));
            }
        }

        std::cout << '\t' << "part 2: " << result;
    }

    export void solution()
    {
        std::ifstream file("data/day11.txt");

        std::cout << "day 11:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day11.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
        part2(content);
    }
} // namespace da11