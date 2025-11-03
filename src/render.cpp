#include <vector>
#include <iostream>

using namespace std;

std::string fgcolor = "\033[48;5;0m  ";
std::string bgcolor = "\033[48;5;15m  ";
std::string playercolor = "\033[48;5;21m  ";
std::string treasurecolor = "\033[48;5;136m  ";
std::string bordercolor = "\033[101m⠀⠀";
std::string reset = "\033[0m";

void print_maze(vector<vector<unsigned char>>& v) {
    for (auto& row : v) {
        for (auto& t : row) {
            switch(t) {
                case 0: cout << bgcolor; break;
                case 1: cout << fgcolor; break;
                case 2: cout << playercolor; break;
                case 3: cout << treasurecolor; break;
                case 4: cout << bordercolor; break;
                default: cout << reset << "  "; break;
            }
        }

        cout << reset << flush << endl;
    }
}

void print_coordinates (const int&player_x, const int&player_y, const int&goal_x, const int&goal_y) {
    std::cout << "\nPosition: (" << player_x << ", " << player_y << ")"
          << "   Exit: (" << goal_x << ", " << goal_y << ")";
    std::cout << std::flush;
}