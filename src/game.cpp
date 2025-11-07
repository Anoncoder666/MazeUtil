//
// Created by atharva on 11/3/25.
//

#include "../include/mazeutil/game.h"
#include "render.h"
#include "control.h"
#include "keyboardInput.h"
extern pair<int,int> maze_in;
extern pair<int,int> maze_out;
extern vector<vector<unsigned char>> maze;
MenuState game_start() {
    std::cout << "\033[?25l" << std::flush;

    std::pair<int,int> location = maze_in;
    std::pair<int,int> goal = maze_out;
    auto& [player_y, player_x] = location;
    auto& [goal_y, goal_x] = goal;

    maze[player_y][player_x] = 2;
    maze[goal_y][goal_x] = 3;

    constexpr int viewport_height = 20;
    constexpr int viewport_width  = 40;

    bool running = true;
    bool render = true;

    const int visible_height = std::min(viewport_height, (int)maze.size());
    const int visible_width  = std::min(viewport_width,  (int)maze[0].size());

    std::vector view(
        visible_height + 2,
        std::vector<unsigned char>(visible_width + 2, 4)
    );

    while (running) {
        int top = 0, left = 0;

        if ((int)maze.size() > viewport_height)
            top = std::max(0, player_y - viewport_height / 2);

        if ((int)maze[0].size() > viewport_width)
            left = std::max(0, player_x - viewport_width / 2);

        // Clamp to maze bounds
        if (top + viewport_height > (int)maze.size())
            top = std::max(0, (int)maze.size() - viewport_height);
        if (left + viewport_width > (int)maze[0].size())
            left = std::max(0, (int)maze[0].size() - viewport_width);

        for (int y = 0; y < visible_height; y++) {
            for (int x = 0; x < visible_width; x++) {
                view[y + 1][x + 1] = maze[top + y][left + x];
            }
        }

        if (render) {
            clearScreen();
            print_maze(view);
            print_coordinates(player_x, maze.size() - player_y, goal_x, maze.size() - goal_y);
            render = false;
        }

        char move = getNormalizedKey();
        int new_y = player_y, new_x = player_x;

        switch (move) {
            case 'w': case 'W': new_y--; break;
            case 's': case 'S': new_y++; break;
            case 'a': case 'A': new_x--; break;
            case 'd': case 'D': new_x++; break;
            default: break;
        }

        if (new_y >= 0 && new_y < (int)maze.size() &&
            new_x >= 0 && new_x < (int)maze[0].size()) {
            if (maze[new_y][new_x] == 0) { // empty
                render = true;
                std::swap(maze[player_y][player_x], maze[new_y][new_x]);
                player_y = new_y;
                player_x = new_x;
            } else if (maze[new_y][new_x] == 3) { // goal
                maze[new_y][new_x] = 0;
                std::swap(maze[player_y][player_x], maze[new_y][new_x]);
                player_y = new_y;
                player_x = new_x;
                clearScreen();
                for (int y = 0; y < visible_height; y++) {
                    for (int x = 0; x < visible_width; x++) {
                        view[y + 1][x + 1] = maze[top + y][left + x];
                    }
                }
                print_maze(view);
                std::cout << "\nReached exit at (" << goal_x << ", " << goal_y << ")\n";
                return Solved;
            }
        }
    }
    return MainMenu;
}
