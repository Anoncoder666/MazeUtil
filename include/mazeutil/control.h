#ifndef PROMPT_H
#define PROMPT_H
#include <functional>
#include <iostream>
#include <iomanip>
using namespace std;
extern string bgcolor;
extern string fgcolor;
enum MenuState {MainMenu, AlgorithmMenu, TessellationSize, Exit, Success, DFSSize,CustomDFSSize, WilsonSize, CustomWilsonSize, BFSSize, CustomBFSSize, KruskalSize, CustomKruskalSize, PrimSize, CustomPrimSize, Settings, Color, Solve, Solved};
enum Algorithm {BFS, DFS, Wilson, Kruskal, Prim, Tessellation};
MenuState mainmenu();
MenuState prompt_algorithm();
MenuState exit();
MenuState color();
MenuState success();
MenuState settings();
MenuState prompt_size(Algorithm);
MenuState custom_size(Algorithm);
MenuState solve();
MenuState solved();
void clearScreen();
int customintInput(const string& prompt, const string& errormsg, bool (*condition)(int));
string Line(const string& s, int width);
//class for displaying menu
struct Option {
    string name;
    MenuState action;
};

class Menu {
    int width = 30;
    vector<string> title;
public:
    vector<Option> options;
    void display() const;

    static int getInput(int max);

    MenuState run(int* a = nullptr) const;

    void setopts(const vector<Option> &options);

    void settitle(const string &title);

    void setwidth(const int &width);

    Menu(int width, const vector<Option>& options, const vector<string> &title = {});
};

#endif //PROMPT_H
