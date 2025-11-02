#include "mazeutil/tessellate.h"

#include <algorithm>

#include "mazeutil/rand.h"
using namespace std;

void generate_unit(vector<vector<unsigned char>>& v) {
    v= {{1, 1, 1}, {1, 0, 1},{1, 1, 1}};
}

void duplicate(vector<vector<unsigned char>>& v) {
    int size = v.size();
    for (int i = 0; i < size; i++) {
        v[i].insert(v[i].end(), next(v[i].begin()), v[i].end());
    }
    for (int i = 1; i < size; i++) {
        v.push_back(v[i]);
    }
}

void carve_internal_paths(vector<vector<unsigned char>> &v, const int iteration) {
    auto ignore = static_cast<Direction>(rand_int(4));

    for (int j = 0; j < 4; j++) {
        const auto dir = static_cast<Direction>(j);
        if (dir == ignore) {
            continue;
        }

        int center = 1 << (iteration + 1);
        int location = rand_int(1 << iteration);

        switch (dir) {
            case Direction::RIGHT:
                v[center][center + (2 * location - 1)] = false;
                break;
            case Direction::UP:
                v[center + (2 * location - 1)][center] = false;
                break;
            case Direction::DOWN:
                v[center][center - (2 * location - 1)] = false;
                break;
            case Direction::LEFT:
                v[center - (2 * location - 1)][center] = false;
                break;
        }
    }
}

void carve_openings_dfs(vector<vector<unsigned char>> &v, const int size) {
    vector dir = {
        Direction::UP,
        Direction::DOWN,
        Direction::LEFT,
        Direction::RIGHT
    };
    ranges::shuffle(dir, rng);

    dir.erase(dir.begin(), next(dir.begin(), 2));

    int open = rand_int(1 << (size - 1)) * 2 - 1;

    if (dir[0] == Direction::UP || dir[1] == Direction::UP) {
        v[v.size() - 1][open] = false;
        open = rand_int(1 << (size - 1)) * 2 - 1;
    }
    if (dir[0] == Direction::DOWN || dir[1] == Direction::DOWN) {
        v[0][open] = false;
        open = rand_int(1 << (size - 1)) * 2 - 1;
    }
    if (dir[0] == Direction::RIGHT || dir[1] == Direction::RIGHT) {
        v[open][v.size() - 1] = false;
        open = rand_int(1 << (size - 1)) * 2 - 1;
    }
    if (dir[0] == Direction::LEFT || dir[1] == Direction::LEFT) {
        v[open][0] = false;
    }
}

void tessellate(const int size) {
    assert(size > 0);
    vector<vector<unsigned char>> v;
    generate_unit(v);
    for (int i = 0; i < size; i++) {
        duplicate(v);
        carve_internal_paths(v, i);
    }
    carve_openings_dfs(v, size);
    print(v);
}
