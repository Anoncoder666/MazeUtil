#pragma once
#ifndef DFS_H
#define DFS_H
#include "rand.h"
#include <vector>
#include <stack>
#include <algorithm>
#include <queue>
#include <cassert>

#include "maze.h"

using namespace std;

bool traverse_dfs(vector<vector<unsigned char>> &v, vector<Direction> &directions, stack<Direction> &q, int& x, int& y);
void backtrack_dfs(stack<Direction> &q, int& x, int& y);
void dfs(vector<vector<unsigned char>> &v);
#endif //DFS_H
