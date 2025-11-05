#include "kruskal.h"

#include <algorithm>
#include <cassert>
#include <random>

extern std::mt19937 rng;

struct Wall {
    int wx, wy; // wall position
    int cell1_x, cell1_y;
    int cell2_x, cell2_y;
};

struct DisjointSet {
    std::vector<int> parent, rank;
    explicit DisjointSet(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    void unite(int x, int y) {
        int rx = find(x), ry = find(y);
        if (rx == ry) return;
        if (rank[rx] < rank[ry]) parent[rx] = ry;
        else {
            parent[ry] = rx;
            if (rank[rx] == rank[ry]) rank[rx]++;
        }
    }
};

void kruskal(std::vector<std::vector<unsigned char>> &maze) {
    assert(!maze.empty() && !maze.front().empty());
    int h = (int)maze.size();
    int w = (int)maze[0].size();

    for (auto &row : maze) std::fill(row.begin(), row.end(), 1);

    int cells_h = (h - 1) / 2;
    int cells_w = (w - 1) / 2;

    auto cell_id = [cells_w](int y, int x) { return y * cells_w + x; };

    DisjointSet ds(cells_h * cells_w);

    std::vector<Wall> walls;
    for (int y = 0; y < cells_h; ++y) {
        for (int x = 0; x < cells_w; ++x) {
            int cx = 2 * x + 1;
            int cy = 2 * y + 1;
            maze[cy][cx] = 0; // mark cell as open

            if (x + 1 < cells_w) {
                walls.push_back({cx + 1, cy, cx, cy, cx + 2, cy});
            }
            if (y + 1 < cells_h) {
                walls.push_back({cx, cy + 1, cx, cy, cx, cy + 2});
            }
        }
    }

    std::ranges::shuffle(walls, rng);

    for (const auto &w : walls) {
        int id1 = cell_id((w.cell1_y - 1) / 2, (w.cell1_x - 1) / 2);
        int id2 = cell_id((w.cell2_y - 1) / 2, (w.cell2_x - 1) / 2);

        if (ds.find(id1) != ds.find(id2)) {
            maze[w.wy][w.wx] = 0;
            ds.unite(id1, id2);
        }
    }
}
