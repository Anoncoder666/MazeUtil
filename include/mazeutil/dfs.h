
#pragma once
#ifndef DFS_H
#define DFS_H
#include "rand.h"
#include <vector>
#include <stack>
#include <cassert>
using namespace std;
vector<vector<unsigned char>> maze_template(int units_width, int units_height);
bool traverse_dfs(vector<vector<unsigned char>> &v, vector<Direction> &directions, stack<Direction> &q, int& x, int& y);
void backtrack_dfs(stack<Direction> &q, int& x, int& y);
void dfs(vector<vector<unsigned char>> &v);
void carve_openings_dfs(vector<vector<unsigned char>> &v);
#endif //DFS_H
