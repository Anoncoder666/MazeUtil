#include <iostream>
#include "control.h"

#ifdef _WIN32
#include <windows.h>
BOOL WINAPI consoleHandler(DWORD signal) {
    if (signal == CTRL_CLOSE_EVENT || signal == CTRL_C_EVENT) {
        std::cout << "Console closing or Ctrl+C pressed. Exiting cleanly...\n";
        std::exit(0);
    }
    return TRUE;
}
#else
#include <csignal>
void handle_signal(int sig) {
    std::cout << "Caught signal " << sig << ", exiting...\n";
    std::exit(0);
}
#endif

using namespace std;

int main() {
    clearScreen();
#ifdef _WIN32
    SetConsoleCtrlHandler(consoleHandler, TRUE);
#else
    signal(SIGHUP, handle_signal);
    signal(SIGINT, handle_signal);
#endif
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
            case Solve:
                state = solve();
                break;
            case Solved:
                state = solved();
                break;
        }
    }
}
