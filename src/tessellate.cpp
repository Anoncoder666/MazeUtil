#include "tessellate.h"

#include "maze.h"
#include "rand.h"

using namespace std;
extern vector<vector<unsigned char>> maze;
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

void tessellate(const int size) {
    assert(size > 0);
    std::vector<std::vector<unsigned char>>().swap(maze);
    generate_unit(maze);
    for (int i = 0; i < size; i++) {
        duplicate(maze);
        carve_internal_paths(maze, i);
    }
    carve_openings(maze);
    print(maze);
}
