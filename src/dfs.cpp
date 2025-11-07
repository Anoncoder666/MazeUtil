#include "dfs.h"

void dfs(std::vector<std::vector<unsigned char>> &v) {
    assert(!v.empty() && !v.front().empty());
    stack<Direction> s;
    vector directions = { Direction::UP, Direction::DOWN, Direction::LEFT, Direction::RIGHT };
    int x = v[0].size()/2;
    int y = v.size()/2;
    if (x%2 == 0) {
        x++;
    }
    if (y%2 == 0) {
        y++;
    }
    v[y][x] = false;
    bool deadEnd = false;
    do {
        if (!deadEnd) {
            deadEnd = traverse_dfs(v, directions, s, x, y);
        }
        else if (!s.empty()){
            backtrack_dfs(s,x,y);
            deadEnd = traverse_dfs(v, directions, s, x, y);
        }
        else break;
    } while (!s.empty());
}

bool traverse_dfs(vector<vector<unsigned char>> &v, vector<Direction> &directions, stack<Direction> &q,int& x, int& y) {
    assert(!v.empty() && !v.front().empty());
    const int max_x = v[0].size();
    const int max_y = v.size();
    ranges::shuffle(directions, rng);
    for (const auto &direction : directions) {
        if (direction == Direction::UP) {
            if (y + 2 < max_y && v[y+2][x] == true) {
                v[y+1][x] = false;
                v[y+2][x] = false;
                y += 2;
                q.push(Direction::UP);
                return false;
            }
        }
        else if (direction == Direction::DOWN) {
            if (y - 2 > 0 && v[y-2][x] == true) {
                v[y-1][x] = false;
                v[y-2][x] = false;
                y -= 2;
                q.push(Direction::DOWN);
                return false;
            }
        }
        else if (direction == Direction::RIGHT) {
            if (x + 2 < max_x && v[y][x+2] == true) {
                v[y][x+1] = false;
                v[y][x+2] = false;
                x += 2;
                q.push(Direction::RIGHT);
                return false;
            }
        }
        else if (direction == Direction::LEFT) {
            if (x - 2 > 0 && v[y][x-2] == true) {
                v[y][x-1] = false;
                v[y][x-2] = false;
                x -= 2;
                q.push(Direction::LEFT);
                return false;
            }
        }
    }
    return true;
}

void backtrack_dfs(stack<Direction> &q, int& x, int& y) {
    assert(!q.empty());
    if (q.top() == Direction::UP) {
        y-=2;
        q.pop();
    }
    else if (q.top()== Direction::DOWN) {
        y += 2;
        q.pop();
    }
    else if (q.top() == Direction::RIGHT) {
        x -= 2;
        q.pop();
    }
    else if (q.top() == Direction::LEFT) {
        x += 2;
        q.pop();
    }
}
