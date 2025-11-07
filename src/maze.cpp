#include "../include/mazeutil/maze.h"
#include "../include/mazeutil/rand.h"
vector<vector<unsigned char>> maze;
pair<int, int> maze_in;
pair<int, int> maze_out;
void carve_openings(vector<vector<unsigned char>> &v) {
    assert(!v.empty() && !v.front().empty());
    switch (rand_int(4)) {
        case 1: maze_in = {0, 1}; break;
        case 2: maze_in = {1, 0}; break;
        case 3: maze_in = {0, static_cast<int>(v[0].size()) - 2}; break;
        case 4: maze_in = {1, static_cast<int>(v[0].size()) - 1}; break;
        default: maze_in = {0, 0}; break;
    }
    switch (rand_int(4)) {
        case 1: maze_out = {static_cast<int>(v.size()) - 1, 1}; break;
        case 2: maze_out = {static_cast<int>(v.size()) - 2, 0}; break;
        case 3: maze_out = {static_cast<int>(v.size()) - 1, static_cast<int>(v[0].size()) - 2}; break;
        case 4: maze_out = {static_cast<int>(v.size()) - 2, static_cast<int>(v[0].size()) - 1}; break;
        default: maze_out = {0, 0}; break;
    }

    v[maze_in.first][maze_in.second] = false;
    v[maze_out.first][maze_out.second] = false;
}

vector<vector<unsigned char>> maze_template(const int units_width, const int units_height) {
    assert(units_width > 0 && units_height > 0);
    const int width = units_width*2 +1;
    const int height = units_height*2 +1;
    maze.clear();
    maze.resize(height);
    for (int j = 0; j < height; j++) {
        maze[j].resize(width, 1);
    }
    return maze;

}