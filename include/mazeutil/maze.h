//
// Created by atharva on 11/2/25.
//

#ifndef MAZE_H
#define MAZE_H
#include <vector>
#include <cassert>
using namespace std;
vector<vector<unsigned char>> maze_template(int units_width, int units_height);
void carve_openings(vector<vector<unsigned char>> &v);
enum class Direction { UP, DOWN, LEFT, RIGHT };

#endif //MAZE_H
