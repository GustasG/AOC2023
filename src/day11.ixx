module;

#include <numeric>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>
#include <ranges>
#include <unordered_set>
#include <queue>
#include <unordered_map>

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
    static std::vector<std::vector<char>> parse_file(std::string_view content)
    {
        return content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::vector<char>(std::from_range, std::move(range)); })
            | std::ranges::to<std::vector<std::vector<char>>>();
    }

    static std::vector<std::string_view> expand_horizontally(std::string_view content)
    {
        std::vector<std::string_view> lines;

        for (auto&& range : std::ranges::views::split(content, '\n'))
        {
            std::string_view line(std::move(range));
            lines.push_back(line);

            if (!line.contains('#'))
                lines.push_back(line);
        }

        return lines;
    }

    static std::vector<size_t> horizontal_lines(std::span<std::string_view> lines)
    {
        std::vector<size_t> result;

        for (size_t i = 0; i < lines.size(); i++)
        {
            if (lines[i].contains('#'))
                result.push_back(i);
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

    static std::vector<std::vector<char>> expand_vertically(std::span<std::string_view> lines)
    {
        std::vector<std::vector<char>> map(lines.size());

        for (size_t i = 0; i < lines[0].size(); i++)
        {
            for (size_t j = 0; j < map.size(); j++)
                map[j].push_back(lines[j][i]);

            if (!contains(lines, i, '#'))
            {
                for (size_t j = 0; j < map.size(); j++)
                    map[j].push_back(lines[j][i]);
            }
        }

        return map;
    }

    static std::vector<size_t> vertical_lines(std::span<std::string_view> lines)
    {
        std::vector<size_t> result;

        for (size_t i = 0; i < lines.size(); i++)
        {
            if (contains(lines, i, '#'))
                result.push_back(i);
        }

        return result;
    }
    
    static std::vector<Vertex> find_galaxies(std::vector<std::vector<char>> map)
    {
        std::vector<Vertex> vertices;

        for (size_t i = 0; i < map.size(); i++)
        {
            for (size_t j = 0; j < map[i].size(); j++)
            {
                if (map[i][j] == '#')
                {
                        vertices.push_back(Vertex
                        {
                            .x = static_cast<int>(j),
                            .y = static_cast<int>(i)
                        });
                }
            }
        }

        return vertices;
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        std::vector<std::string_view> lines = expand_horizontally(content);
        std::vector<std::vector<char>> map = expand_vertically(lines);
        std::vector<Vertex> vertices = find_galaxies(map);
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
        std::vector<std::vector<char>> map = parse_file(content);
        std::vector<Vertex> vertices = find_galaxies(map);

        for (auto&& vertex : vertices)
        {
            std::cout << vertex.x << ' ' << vertex.y << '\n';
        }
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