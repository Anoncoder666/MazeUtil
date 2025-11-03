#ifndef PROMPT_H
#define PROMPT_H
#include <functional>
#include <iostream>
#include <iomanip>
using namespace std;
extern string bgcolor;
extern string fgcolor;
enum MenuState { MainMenu, AlgorithmMenu, TessellationSize, Exit, Success, DFSSize,CustomDFSSize,WilsonSize, CustomWilsonSize, Settings, Color, Solve, Solved};
MenuState mainmenu();
MenuState prompt_algorithm();
MenuState prompt_size_tessellation();
MenuState exit();
MenuState color();
MenuState success();
MenuState settings();
MenuState prompt_size_dfs();
MenuState custom_size_dfs();
MenuState prompt_size_wilson();
MenuState custom_size_wilson();
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
    string title;
public:
    vector<Option> options;
    void display() const;

    static int getInput(int max);

    MenuState run(int* a = nullptr) const;

    void setopts(const vector<Option> &options);

    void settitle(const string &title);

    void setwidth(const int &width);

    Menu( int width, const vector<Option>& options = {}, const char* title = "");
};

#endif //PROMPT_H
