#include "prim.h"
#include <cassert>
#include <random>

#include "rand.h"

extern std::mt19937 rng;

struct Wall {
    int wx, wy;
    int cx, cy;
};

static bool in_bounds(const std::vector<std::vector<unsigned char>> &maze, int x, int y) {
    return y >= 0 && y < (int)maze.size() && x >= 0 && x < (int)maze[0].size();
}

static void add_walls(const std::vector<std::vector<unsigned char>> &maze, int x, int y, std::vector<Wall> &walls) {
    int h = (int)maze.size();
    int w = (int)maze[0].size();

    if (y - 2 > 0 && maze[y - 2][x] == 1) walls.push_back({x, y - 1, x, y - 2});
    if (y + 2 < h && maze[y + 2][x] == 1) walls.push_back({x, y + 1, x, y + 2});
    if (x - 2 > 0 && maze[y][x - 2] == 1) walls.push_back({x - 1, y, x - 2, y});
    if (x + 2 < w && maze[y][x + 2] == 1) walls.push_back({x + 1, y, x + 2, y});
}

void prim(std::vector<std::vector<unsigned char>> &maze) {
    assert(!maze.empty() && !maze.front().empty());
    int h = (int)maze.size();
    int w = (int)maze[0].size();

    for (auto &row : maze) std::fill(row.begin(), row.end(), 1);

    if (h < 3 || w < 3) return;

    const int sx = rand_int((w - 1) / 2) * 2 - 1;
    const int sy = rand_int((h - 1) / 2) * 2 - 1;

    maze[sy][sx] = 0;

    std::vector<Wall> walls;
    add_walls(maze, sx, sy, walls);

    while (!walls.empty()) {
        std::uniform_int_distribution<int> pick(0, (int)walls.size() - 1);
        int idx = pick(rng);
        Wall w = walls[idx];
        walls.erase(walls.begin() + idx);

        if (in_bounds(maze, w.cx, w.cy) && maze[w.cy][w.cx] == 1) {
            maze[w.wy][w.wx] = 0;
            maze[w.cy][w.cx] = 0;

            add_walls(maze, w.cx, w.cy, walls);
        }
    }
}
