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

bool inBounds(int x, int y, int h, int w) {
    return x > 0 && y > 0 && x < h - 1 && y < w - 1;
}

// Utility: get a random *odd* coordinate inside bounds
int rand_odd(int limit) {
    int val = rand_int(limit / 2); // 1..limit/2
    return val * 2 - 1;            // 1,3,5,...
}

void prim(vector<vector<unsigned char>>& maze) {
    int h = maze.size(), w = maze[0].size();

    // Choose random odd start cell
    int startX = rand_odd(h);
    int startY = rand_odd(w);
    maze[startX][startY] = 0;

    vector<Frontier> frontierList;

    auto addFrontier = [&](int fx, int fy, int wx, int wy) {
        if (inBounds(fx, fy, h, w) && maze[fx][fy] == 1) {
            bool exists = ranges::any_of(frontierList,[&](const Frontier& f){ return f.fx == fx && f.fy == fy; });
            if (!exists)
                frontierList.push_back({fx, fy, wx, wy});
        }
    };

    // Add initial frontier cells (2 steps away)
    for (int d = 0; d < 4; ++d) {
        int fx = startX + 2 * DX[d];
        int fy = startY + 2 * DY[d];
        int wx = startX + DX[d];
        int wy = startY + DY[d];
        addFrontier(fx, fy, wx, wy);
    }

    while (!frontierList.empty()) {
        int idx = rand_int(frontierList.size()) - 1; // rand_int is 1-based
        Frontier f = frontierList[idx];

        // Remove all duplicates of this frontier cell
        frontierList.erase(
            remove_if(frontierList.begin(), frontierList.end(),
                      [&](const Frontier& other){
                          return other.fx == f.fx && other.fy == f.fy;
                      }),
            frontierList.end()
        );

        // Carve passage and connecting wall
        if (maze[f.fx][f.fy] == 1) {
            maze[f.fx][f.fy] = 0;
            maze[f.wx][f.wy] = 0;

            // Add new frontier cells
            for (int d = 0; d < 4; ++d) {
                int fx = f.fx + 2 * DX[d];
                int fy = f.fy + 2 * DY[d];
                int wx = f.fx + DX[d];
                int wy = f.fy + DY[d];
                addFrontier(fx, fy, wx, wy);
            }
        }
    }
}
