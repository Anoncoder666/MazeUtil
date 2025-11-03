#include "control.h"
#include "dfs.h"
#include "maze.h"
#include "tessellate.h"
#include "wilson.h"
#include "keyboardInput.h"
#include "render.h"

using namespace std;
extern vector<vector<unsigned char>> maze;
extern pair<int, int> maze_in;
extern pair<int, int> maze_out;
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H" << flush;
    system("clear");
#endif
}

MenuState mainmenu() {
    const vector<Option> options = {
        {"Start", AlgorithmMenu},
        {"Settings", Settings},
        {"Exit", Exit}
    };
    const Menu menu(30,options, "Welcome to MazeUtil!");
    const MenuState end = menu.run();
    clearScreen();
    return end;
}

MenuState prompt_algorithm() {
    const vector<Option> options = {
        {"Randomized DFS", DFSSize},
        { "Wilson's Algorithm", WilsonSize},
        {"Tessellation", TessellationSize},
        {"Go Back", MainMenu}
    };
    const Menu menu(30,options, "Choose an algorithm");
    const MenuState end = menu.run();
    clearScreen();
    return end;
}

MenuState prompt_size_tessellation() {
    const vector<Option> options = {
        {"2x2", Success},
        {"4x4", Success},
        {"8x8", Success},
        {"16x16", Success},
        {"32x32", Success},
        {"Go Back", AlgorithmMenu}
    };
    const Menu menu(30,options, "Choose a size");
    int size = 0;
    const MenuState end = menu.run(&size);
    clearScreen();
    if (end == Success) {
        tessellate(size+1);
    }
    return end;
}

MenuState prompt_size_dfs() {
    const vector<Option> options = {
        {"10x10", Success},
        {"20x20", Success},
        {"30x30", Success},
        {"Custom", CustomDFSSize},
        {"Go Back", AlgorithmMenu}
    };
    const Menu menu(30,options, "Choose a size");
    int size = 0;
    const MenuState end = menu.run(&size);
    clearScreen();
    if (end == Success) {

        maze = maze_template(10*(size+1),10*(size+1));
        dfs(maze);

        carve_openings(maze);
        print_maze(maze);
    }

    return end;
}

MenuState custom_size_dfs() {
    const vector<Option> options = {
        {"Go Back", AlgorithmMenu}
    };
    const Menu menu(30, options);
    menu.display();
    auto test = [](int a) { return a >= 1 && a <= 50; };
    const int width = customintInput("Select maze width (2-50): ", "Please select a number 2-50", test);
    if (width == 1) {
        clearScreen();
        return DFSSize;
    }
    const int height = customintInput("Select maze height (2-50): ", "Please select a number 2-50", test);
    if (height == 1) {
        clearScreen();
        return DFSSize;
    }
    clearScreen();
    maze = maze_template(width,height);
    dfs(maze);
    carve_openings(maze);
    print_maze(maze);
    return Success;
}

MenuState prompt_size_wilson() {

    const vector<Option> options = {
        {"10x10", Success},
        {"20x20", Success},
        {"30x30", Success},
        {"Custom", CustomDFSSize},
        {"Go Back", AlgorithmMenu}
    };
    const Menu menu(30,options, "Choose a size");
    int size = 0;
    const MenuState end = menu.run(&size);
    clearScreen();
    if (end == Success) {
        maze = maze_template(10*(size+1),10*(size+1));
        wilson(maze);
        carve_openings(maze);
        print_maze(maze);
    }
    return end;
}

MenuState custom_size_wilson() {
    const vector<Option> options = {
        {"Go Back", AlgorithmMenu}
    };
    const Menu menu(30, options);
    menu.display();
    auto test = [](int a) { return a >= 1 && a <= 50; };
    int width = customintInput("Select maze width (2-50): ", "Please select a number 2-50", test);
    if (width == 1) {
        clearScreen();
        return DFSSize;
    }
    int height = customintInput("Select maze height (2-50): ", "Please select a number 2-50", test);
    if (height == 1) {
        clearScreen();
        return DFSSize;
    }
    clearScreen();
    //template_for_wilson(width,height);
    return Success;
}
MenuState success() {
    cout << "Your maze was successfully generated!" << endl;
    const vector<Option> options = {
        {"Solve Generated Maze", Solve},
            {"Generate New Maze", AlgorithmMenu},
            {"Return to Main Menu", MainMenu},
            {"Exit", Exit}
        };
    const Menu menu(30,options);
    const MenuState end = menu.run();
    clearScreen();
    return end;
    }

MenuState settings() {
    const vector<Option> options = {
        {"Color", Color},
        {"Go Back", MainMenu}
    };
    const Menu menu = {30, options, "Settings"};
    const MenuState end = menu.run();
    clearScreen();
    return end;
}

MenuState exit() {
    clearScreen();
    exit(0);
}

MenuState color() {
    const vector<Option> options = {
        {"White", Color},
        {"Sand", Color},
        {"Grey", Color},
        {"Go Back", Settings}
    };
    const Menu menu(30,options, "Colors");
    menu.display();
    while (true) {
        switch (int output = Menu::getInput(4)) {
            case 1: {
                bgcolor = "\033[48;5;15m⠀⠀\033[0m";
                cout << "Color set to white!" << endl;
                continue;
            }
            case 2: {
                bgcolor = "\033[48;5;222m⠀⠀\033[0m";
                cout << "Color set to sand!" << endl;
                continue;
            }
            case 3: {
                bgcolor = "\033[48;5;253m⠀⠀\033[0m";
                cout << "Color set to grey!" << endl;
                continue;
            }
            default: {
                clearScreen();
                return Settings;
            }
        }
    }
}

int customintInput(const string& prompt, const string& errormsg, bool (*condition)(int)) {
    int choice;
    while (true) {
        cout << prompt;
        string line;
        getline(cin, line);

        try {
            choice = stoi(line);
        } catch (exception& e) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (!condition(choice)) {
            cout << errormsg << endl;
            continue;
        }

        break;
    }
    return choice;
}
MenuState solved() {
    cout << "Good job! You solved the maze!" << endl;
    const vector<Option> options = {
            {"Generate New Maze", AlgorithmMenu},
            {"Return to Main Menu", MainMenu},
            {"Exit", Exit}
    };
    const Menu menu(30,options);
    const MenuState end = menu.run();
    clearScreen();
    return end;
}
MenuState solve() {

    std::pair<int,int> location = maze_in;
    std::pair<int,int> goal = maze_out;
    auto& [y, x] = location; // structured binding references
    maze[y][x] = 2;
    maze[goal.first][goal.second] = 3;
    bool reRender = true;
    while (location != goal) {
        if (reRender) {
            clearScreen();
            cout << flush;
            print_maze(maze);
            reRender = false;
        }
        switch (char move = getNormalizedKey()) {
            case 'w':
            case 'W':
                if (y > 0 && maze[y-1][x] == 0) { // move up
                    std::swap(maze[y-1][x], maze[y][x]);
                    y--;
                    reRender = true;
                }
                else if (maze[y-1][x] == 3) {
                    std::swap(maze[y-1][x], maze[y][x]);
                    maze[y][x] = 0;
                    y--;
                    clearScreen();
                    print_maze(maze);
                    return Solved;
                }
                break;

            case 's':
            case 'S':
                if (y < maze.size()-1 && maze[y+1][x] == 0) { // move down
                    std::swap(maze[y+1][x], maze[y][x]);
                    y++;
                    reRender = true;
                }
                else if (maze[y+1][x] == 3) {
                    std::swap(maze[y+1][x], maze[y][x]);
                    maze[y][x] = 0;
                    y++;
                    clearScreen();
                    print_maze(maze);
                    return Solved;
                }
                break;

            case 'a':
            case 'A':
                if (x > 0 && maze[y][x-1] == 0) { // move left
                    std::swap(maze[y][x-1], maze[y][x]);
                    x--;
                    reRender = true;
                }
                else if (maze[y][x-1] == 3) {
                    std::swap(maze[y][x-1], maze[y][x]);
                    maze[y][x] = 0;
                    x--;
                    clearScreen();
                    print_maze(maze);
                    return Solved;
                }
                break;

            case 'd':
            case 'D':
                if (x < maze[y].size()-1 && maze[y][x+1] == 0) { // move right
                    std::swap(maze[y][x+1], maze[y][x]);
                    x++;
                    reRender = true;
                }
                else if (maze[y][x+1] == 3) {
                    std::swap(maze[y][x+1], maze[y][x]);
                    maze[y][x] = 0;
                    x++;
                    clearScreen();
                    print_maze(maze);
                    return Solved;
                }
                break;
            default:
                break;
        }
    }
    return MainMenu;
}

string Line(const string& s, int width) {
    string ss;
    for (int i = 0; i < width; i++) {
        ss += s;
    }
    return ss;
}
//CLASS FUNCTIONS

void Menu::display () const {
    if (this->title != "") {
        cout << "┌" << Line("─", width) << "┐" << endl;
        cout << "│" << string((width - title.length()) / 2, ' ') << title << string((width - title.length() + 1) / 2, ' ') << "│" << endl;
        cout << "├" << Line("─", width) << "┤" << endl;
    }
    else cout << "┌"<< Line("─", width) <<"┐" << endl;
    for (int i = 0; i < options.size(); i++) {
        string s = "│  " + to_string(i+1) + ") " + options[i].name;
        cout << s << setw(width+3 - s.length()) << " "<<  "│" << endl;
    }
    cout << "└" << Line("─", width) << "┘" << endl;
}

Menu::Menu(const int width, const vector<Option>& options, const char* title ) {
    this->title = title;
    this->width = width;
    this->options = options;
}

int Menu::getInput(int max) {
    int choice;
    while (true) {
        cout << "Select option (" << 1 << "-" << max << "): ";
        string line;
        getline(cin, line);

        try {
            choice = stoi(line);
        } catch (exception& e) {
            cout << "Invalid input! Please enter a number.\n";
            continue;
        }

        if (choice < 1 || choice > max) {
            cout << "Choice out of range! Try again.\n";
            continue;
        }

        break;
    }
    return choice;
}

MenuState Menu::run(int* n) const {
    display();
    const int index = getInput(options.size())-1;
    if (n != nullptr) {
        *n = index;
    }
    return options[index].action;
}
