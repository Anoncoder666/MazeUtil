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
    std::cout << "\033[?25h" << std::flush;
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
                print_maze(maze);
                std::cout << "\nReached goal at (" << goal_x << ", " << goal_y << ")!\n";
                return Solved;
            }
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
    if (!this->title.empty()) {
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
    std::cout << "\033[?25h" << std::flush;
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

MenuState Menu::run(int* a) const {
    display();
    const int index = getInput(options.size())-1;
    if (a != nullptr) {
        *a = index;
    }
    return options[index].action;
}
