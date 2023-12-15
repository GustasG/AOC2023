module;

#include <fstream>
#include <iostream>
#include <queue>
#include <ranges>
#include <span>
#include <string_view>
#include <string>
#include <unordered_set>
#include <vector>

import utility;

export module day10;

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

namespace day10
{
    static Vertex find_starting(const std::span<std::string_view>& map)
    {
        for (const auto& [i, line] : map | std::views::enumerate)
        {
            for (const auto& [j, c] : line | std::views::enumerate)
            {
                if (c == 'S')
                {
                    return Vertex
                    {
                        .x = static_cast<int>(j),
                        .y = static_cast<int>(i)
                    };
                }
            }
        }

        std::cerr << '\t' << "Unable to find starting position" << '\n';
        std::quick_exit(1);
    }

    static bool is_valid(const std::span<std::string_view>& map, const Vertex& vertex)
    {
        return vertex.x >= 0 && vertex.x < static_cast<int>(map[0].size()) &&
            vertex.y >= 0 && vertex.y < static_cast<int>(map.size()) &&
            map[vertex.y][vertex.x] != '.';
    }

    static std::vector<Vertex> find_starting_neighbours(std::span<std::string_view> map, const Vertex& vertex)
    {
        auto directions = std::to_array<std::pair<Vertex, std::string_view>>({
            { Vertex { vertex.x, vertex.y - 1 }, "|7F" },
            { Vertex { vertex.x + 1, vertex.y }, "-J7" },
            { Vertex { vertex.x, vertex.y + 1 }, "|LJ" },
            { Vertex { vertex.x - 1, vertex.y }, "-LF" }
        });

        std::vector<Vertex> result;

        for (const auto& [direction, symbols] : directions)
        {
            if (is_valid(map, direction) && symbols.contains(map[direction.y][direction.x]))
                result.push_back(direction);
        }

        return result;
    }

    static std::vector<Vertex> find_neighbours(std::span<std::string_view> map, const Vertex& vertex)
    {
        switch (map[vertex.y][vertex.x])
        {
            case '|':
                return { Vertex{ vertex.x, vertex.y - 1 }, Vertex{ vertex.x, vertex.y + 1 } };
            case '-':
                return { Vertex{ vertex.x + 1, vertex.y }, Vertex{ vertex.x - 1, vertex.y } };
            case 'L':
                return { Vertex{ vertex.x, vertex.y - 1 }, Vertex{ vertex.x + 1, vertex.y } };
            case 'J':
                return { Vertex{ vertex.x, vertex.y - 1 }, Vertex{ vertex.x - 1, vertex.y } };
            case '7':
                return { Vertex{ vertex.x, vertex.y + 1 }, Vertex{ vertex.x - 1, vertex.y } };
            case 'F':
                return { Vertex{ vertex.x + 1, vertex.y }, Vertex{ vertex.x, vertex.y + 1 } };
            case '.':
                return { };
            case 'S':
                return find_starting_neighbours(map, vertex);
            default:
                std::cerr << '\t' << "Unknown character: " << map[vertex.y][vertex.x] << '\n';
                std::quick_exit(1);
        }
    }

    static size_t bfs(std::span<std::string_view> map)
    {
        size_t max_distance = 0;
        std::unordered_set<Vertex> visited;

        std::queue<std::pair<Vertex, size_t>> queue;
        queue.emplace(find_starting(map), 0);

        while (!queue.empty())
        {
            auto [vertex, distance] = queue.front();
            queue.pop();

            if (visited.contains(vertex))
                continue;

            visited.insert(vertex);
            max_distance = std::max(max_distance, distance);

            for (const auto& neighbourd : find_neighbours(map, vertex))
            {
                if (is_valid(map, neighbourd))
                    queue.emplace(neighbourd, distance + 1);
            }
        }

        return max_distance;
    }

    static void part1(std::string_view content)
    {
        utility::Timer t;

        auto map = content
            | std::views::split('\n')
            | std::views::transform([] (auto&& range) { return std::string_view(std::move(range)); })
            | std::ranges::to<std::vector<std::string_view>>();

        size_t result = bfs(map);

        std::cout << '\t' << "part 1: " << result;
    }

    static void part2(std::string_view content)
    {

    }

    export void solution()
    {
        std::ifstream file("data/day10.txt");

        std::cout << "day 10:" << '\n';

        if (!file.is_open())
        {
            std::cerr << '\t' << "Unable to open file: " << "data/day10.txt" << '\n';
            return;
        }

        std::string content = utility::read_file(file);

        part1(content);
        part2(content);
    }
} // namespace day10