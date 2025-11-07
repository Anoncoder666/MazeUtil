#include "wilson.h"
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <random>
#include <algorithm>
#include <cassert>

#include "maze.h"
#include "rand.h"

using namespace std;

static pair<int,int> moveCoord(const pair<int,int>& coord, const Direction dir) {
    switch (dir) {
        case Direction::UP:    return {coord.first, coord.second - 2};
        case Direction::DOWN:  return {coord.first, coord.second + 2};
        case Direction::LEFT:  return {coord.first - 2, coord.second};
        case Direction::RIGHT: return {coord.first + 2, coord.second};
    }
    return coord;
}

static bool inBounds(const int x, const int y, const int width, const int height) {
    return x > 0 && y > 0 && x < width - 1 && y < height - 1 &&
        x % 2 == 1 && y % 2 == 1;
}

static uint64_t encode(int x, int y) {
    return (static_cast<uint64_t>(y) << 32) | static_cast<uint32_t>(x);
}

static void carvePath(
    vector<vector<unsigned char>>& v,
    const vector<pair<int,int>>& path,
    unordered_set<uint64_t>& unvisited
) {
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        auto a = path[i];
        auto b = path[i + 1];
        v[a.second][a.first] = 0;
        v[b.second][b.first] = 0;
        v[(a.second + b.second) / 2][(a.first + b.first) / 2] = 0;
        unvisited.erase(encode(a.first, a.second));
        unvisited.erase(encode(b.first, b.second));
    }
}

void wilson(std::vector<std::vector<unsigned char>>& v) {
    assert(!v.empty() && !v.front().empty());
    const int height = v.size();
    const int width = v[0].size();
    const std::vector dirs = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };


    unordered_set<uint64_t> unvisited;
    for (int y = 1; y < height; y += 2)
        for (int x = 1; x < width; x += 2)
            unvisited.insert(encode(x, y));

    uniform_int_distribution dist_x(1, width - 2);
    uniform_int_distribution dist_y(1, height - 2);
    int sx = dist_x(rng) | 1;
    int sy = dist_y(rng) | 1;
    v[sy][sx] = 0;
    unvisited.erase(encode(sx, sy));

    auto pickRandomUnvisited = [&]() -> pair<int,int> {
        const size_t idx = uniform_int_distribution<size_t>(0, unvisited.size() - 1)(rng);
        auto it = unvisited.begin();
        advance(it, idx);
        const uint64_t key = *it;
        return {static_cast<int>(key & 0xffffffffu), static_cast<int>(key >> 32)};
    };

    while (!unvisited.empty()) {
        const auto cur = pickRandomUnvisited();
        vector path = {cur};
        unordered_map<uint64_t, size_t> index;
        index[encode(cur.first, cur.second)] = 0;

        while (true) {
            vector<Direction> validMoves;
            for (auto d : dirs) {
                auto [fst, snd] = moveCoord(path.back(), d);
                if (inBounds(fst, snd, width, height))
                    validMoves.push_back(d);
            }

            if (validMoves.empty())
                break;

            const Direction d = validMoves[uniform_int_distribution<int>(0, validMoves.size() - 1)(rng)];
            auto next = moveCoord(path.back(), d);

            if (uint64_t key = encode(next.first, next.second); index.contains(key)) {
                path.resize(index[key] + 1);
            } else {
                path.push_back(next);
                index[key] = path.size() - 1;
            }

            if (v[next.second][next.first] == 0) {
                carvePath(v, path, unvisited);
                break;
            }
        }
    }
}
