#include "../include/mazeutil/bfs.h"

#include "maze.h"
#include "rand.h"

void bfs(std::vector<std::vector<unsigned char>> &v) {
    assert(!v.empty() && !v.front().empty());
    queue<std::pair<int,int>> q;
    std::vector<Direction> directions = { Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT };

    int x = v[0].size() / 2;
    int y = v.size() / 2;
    if (x % 2 == 0) x++;
    if (y % 2 == 0) y++;

    v[y][x] = false;
    q.push({x, y});

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        std::ranges::shuffle(directions, rng);
        for (const auto &direction : directions) {
            if (direction == Direction::UP) {
                if (cy + 2 < (int)v.size() && v[cy+2][cx]) {
                    v[cy+1][cx] = false;
                    v[cy+2][cx] = false;
                    q.push({cx, cy+2});
                }
            }
            else if (direction == Direction::DOWN) {
                if (cy - 2 > 0 && v[cy-2][cx]) {
                    v[cy-1][cx] = false;
                    v[cy-2][cx] = false;
                    q.push({cx, cy-2});
                }
            }
            else if (direction == Direction::RIGHT) {
                if (cx + 2 < (int)v[0].size() && v[cy][cx+2]) {
                    v[cy][cx+1] = false;
                    v[cy][cx+2] = false;
                    q.push({cx+2, cy});
                }
            }
            else if (direction == Direction::LEFT) {
                if (cx - 2 > 0 && v[cy][cx-2]) {
                    v[cy][cx-1] = false;
                    v[cy][cx-2] = false;
                    q.push({cx-2, cy});
                }
            }
        }
    }
}