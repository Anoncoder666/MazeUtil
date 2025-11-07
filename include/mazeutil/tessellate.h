
#ifndef TESSELLATE_H
#define TESSELLATE_H
#include <vector>
#include <cassert>
#include <algorithm>
#include <vector>
#include "render.h"

using namespace std;
void duplicate(std::vector<std::vector<bool>>& v);
void puncture(std::vector<std::vector<bool>>& v);
void tessellate(int size);
void generate_unit(vector<vector<unsigned char>>& v);
void carve_internal_paths(vector<vector<unsigned char>>& v, int iteration);

#endif