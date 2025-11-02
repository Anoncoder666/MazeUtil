#include "../include/mazeutil/maze.h"
#include "../include/mazeutil/rand.h"
vector<vector<unsigned char>> maze;
void carve_openings(vector<vector<unsigned char>> &v) {
    assert(!v.empty() && !v.front().empty());
    switch (int rand = rand_int(4)) {
        case 1: v[0][1] = false; break;
        case 2: v[1][0] = false; break;
        case 3: v[0][v[0].size()-2] = false; break;
        case 4: v[1][v[0].size()-1] = false; break;
        default: break;
    }
    switch (int rand = rand_int(4)) {
        case 1: v[v.size()-1][1] = false; break;
        case 2: v[v.size()-2][0] = false; break;
        case 3: v[v.size()-1][v[0].size()-2] = false; break;
        case 4: v[v.size()-2][v[0].size()-1] = false; break;
        default: break;
    }
}

vector<vector<unsigned char>> maze_template(int units_width, int units_height) {
    assert(units_width > 0 && units_height > 0);
    const int width = units_width*2 +1;
    const int height = units_height*2 +1;
    vector<vector<unsigned char>> v;
    v.resize(height);
    for (int j = 0; j < height; j++) {
        v[j].resize(width, 1);
    }
    return v;

}