#include <iostream>
#include "control.h"
using namespace std;

int main() {
    MenuState state = MainMenu;

    while (state != Exit) {
        switch (state) {
            case MainMenu:
                state = mainmenu();
                break;
            case AlgorithmMenu:
                state = prompt_algorithm();
                break;
            case TessellationSize:
                state = prompt_size_tessellation();
                break;
            case Success:
                state = success();
                break;
            case DFSSize:
                state = prompt_size_dfs();
                break;
            case CustomDFSSize:
                state = custom_size_dfs();
                break;
            case WilsonSize:
                state = prompt_size_wilson();
                break;
            case CustomWilsonSize:
                state = custom_size_wilson();
                break;
            case Settings:
                state = settings();
                break;
            case Color:
                state = color();
                break;
        }
    }
}

