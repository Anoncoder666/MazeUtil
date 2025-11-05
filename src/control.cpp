#include "control.h"
#include "dfs.h"
#include "bfs.h"
#include "game.h"
#include "maze.h"
#include "tessellate.h"
#include "wilson.h"
#include "keyboardInput.h"
#include "kruskal.h"
#include "prim.h"
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
    const Menu menu(30,options, {"Welcome to MazeUtil!"});
    const MenuState end = menu.run();
    clearScreen();
    return end;
}

MenuState prompt_algorithm() {
    const vector<Option> options = {
        {"Randomized DFS", DFSSize},
        { "Wilson's Algorithm", WilsonSize},
        {"Kruskal's Algorithm", KruskalSize},
        {"Prim's Algorithm", PrimSize},
        {"Tessellation", TessellationSize},
        {"Randomized BFS", BFSSize},
        {"Go Back", MainMenu}
    };
    const Menu menu(30,options, {"Choose an algorithm", "(top algorithms are best)"});
    const MenuState end = menu.run();
    clearScreen();
    return end;
}

MenuState prompt_size(Algorithm algorithm) {
    vector<Option> options;
    switch (algorithm) {
        case Tessellation:
            options = {
            {"2x2", Success},
            {"4x4", Success},
            {"8x8", Success},
            {"16x16", Success},
            {"32x32", Success},
            {"Go Back", AlgorithmMenu}
            };
            break;
        default:
            options = {
            {"10x10", Success},
            {"20x20", Success},
            {"30x30", Success},
            {"Custom", CustomDFSSize},
            {"Go Back", AlgorithmMenu}
            };
    }

    string second;
    switch (algorithm) {
        case Tessellation: second = "Tessellation"; break;
        case DFS: second = "DFS"; break;
        case Wilson: second = "Wilson"; break;
        case BFS: second = "BFS"; break;
        case Prim: second = "Prim"; break;
        case Kruskal: second = "Kruskal"; break;
    }
    second = second + " Maze";
    const Menu menu(30,options, {second, "Choose a size"});
    int size = 0;
    const MenuState end = menu.run(&size);
    clearScreen();
    if (end == Success) {

        maze = maze_template(10*(size+1),10*(size+1));
        switch (algorithm) {
            case DFS: dfs(maze); break;
            case Wilson: wilson(maze); break;
            case BFS: bfs(maze); break;
            case Tessellation: tessellate(size + 1); break;
            case Kruskal: kruskal(maze); break;
            case Prim: prim(maze); break;
            default: ;
        }
        carve_openings(maze);
        print_maze(maze);
    }

    return end;
}

MenuState custom_size(Algorithm algorithm) {
    const vector<Option> options = {
        {"Go Back", AlgorithmMenu}
    };
    const Menu menu(30, options);
    menu.display();
    auto test = [](const int a) { return a >= 1 && a <= 100; };
    const int width = customintInput("Select maze width (2-100): ", "Please select a number 2-100", test);
    if (width == 1) {
        clearScreen();
        switch (algorithm) {
            case DFS: return DFSSize;
            case Wilson: return WilsonSize;
            case BFS: return BFSSize;
            case Kruskal: return KruskalSize;
            case Prim: return PrimSize;
            default: ;
        }
    }
    const int height = customintInput("Select maze height (2-100): ", "Please select a number 2-100", test);
    if (height == 1) {
        clearScreen();
        switch (algorithm) {
            case DFS: return DFSSize;
            case Wilson: return WilsonSize;
            case BFS: return BFSSize;
            case Kruskal: return KruskalSize;
            case Prim: return PrimSize;
            default: ;
        }
    }
    clearScreen();
    maze = maze_template(width,height);
    switch (algorithm) {
        case DFS: dfs(maze); break;
        case Wilson: wilson(maze); break;
        case BFS: bfs(maze); break;
        case Kruskal: kruskal(maze); break;
        case Prim: prim(maze); break;
        default: ;
    }
    carve_openings(maze);
    print_maze(maze);
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
    const Menu menu = {30, options, {"Settings"}};
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
    const Menu menu(30,options, {"Colors"});
    menu.display();
    while (true) {
        switch (Menu::getInput(4)) {
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
    MenuState end = game_start();
    return end;
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
        for (auto line: title) {
            cout << "│" << string((width - line.length()) / 2, ' ') << line << string((width - line.length() + 1) / 2, ' ') << "│" << endl;
        }
        cout << "├" << Line("─", width) << "┤" << endl;
    }
    else cout << "┌"<< Line("─", width) <<"┐" << endl;
    for (int i = 0; i < options.size(); i++) {
        string s = "│  " + to_string(i+1) + ") " + options[i].name;
        cout << s << setw(width+3 - s.length()) << " "<<  "│" << endl;
    }
    cout << "└" << Line("─", width) << "┘" << endl;
}

Menu::Menu(const int width, const vector<Option>& options, const vector<string> &title) {
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
