#include <algorithm>
#include<rand.h>
#include<prim.h>
using namespace std;

// Directions
constexpr int DX[4] = { -1, 1, 0, 0 };
constexpr int DY[4] = { 0, 0, -1, 1 };

struct Frontier {
    int fx, fy; // frontier cell (2 away)
    int wx, wy; // wall cell between it and the maze
};

bool inBounds(const int x, const int y, const int h, const int w) {
    return x > 0 && y > 0 && x < h - 1 && y < w - 1;
}

// Utility: get a random *odd* coordinate inside bounds
int rand_odd(const int limit) {
    const int val = rand_int(limit / 2); // 1..limit/2
    return val * 2 - 1;            // 1,3,5,...
}

void prim(vector<vector<unsigned char>>& maze) {
    const int h = maze.size();
    const int w = maze[0].size();

    // Choose random odd start cell
    const int startX = rand_odd(h);
    const int startY = rand_odd(w);
    maze[startX][startY] = 0;

    vector<Frontier> frontierList;

    auto addFrontier = [&](const int fx, const int fy, const int wx, const int wy) {
        if (inBounds(fx, fy, h, w) && maze[fx][fy] == 1) {
            if (const bool exists = ranges::any_of(frontierList,[&](const Frontier& f){ return f.fx == fx && f.fy == fy; }); !exists)
                frontierList.push_back({fx, fy, wx, wy});
        }
    };

    // Add initial frontier cells (2 steps away)
    for (int d = 0; d < 4; ++d) {
        const int fx = startX + 2 * DX[d];
        const int fy = startY + 2 * DY[d];
        const int wx = startX + DX[d];
        const int wy = startY + DY[d];
        addFrontier(fx, fy, wx, wy);
    }

    while (!frontierList.empty()) {
        const int idx = rand_int(frontierList.size()) - 1; // rand_int is 1-based
        auto [fx, fy, wx, wy] = frontierList[idx];

        // Remove all duplicates of this frontier cell
        frontierList.erase(
            ranges::remove_if(frontierList,
                              [&](const Frontier& other){
                                  return other.fx == fx && other.fy == fy;
                              }).begin(),
            frontierList.end()
        );

        // Carve passage and connecting wall
        if (maze[fx][fy] == 1) {
            maze[fx][fy] = 0;
            maze[wx][wy] = 0;

            // Add new frontier cells
            for (int d = 0; d < 4; ++d) {
                int fx = fx + 2 * DX[d];
                int fy = fy + 2 * DY[d];
                int wx = fx + DX[d];
                int wy = fy + DY[d];
                addFrontier(fx, fy, wx, wy);
            }
        }
    }
}
