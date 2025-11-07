#pragma once
#include <cctype>

#ifdef _WIN32
#include <conio.h>

inline char getNormalizedKey() {
    int ch = _getch();
    if (ch == 224) { // arrow prefix on Windows
        ch = _getch();
        switch (ch) {
            case 72: return 'w'; // Up
            case 80: return 's'; // Down
            case 75: return 'a'; // Left
            case 77: return 'd'; // Right
        }
    }
    return static_cast<char>(tolower(ch));
}

#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

inline char getNormalizedKey() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    int ch = getchar();

    if (ch == 27) { // ESC prefix
        int ch1 = getchar();
        if (ch1 == '[') {
            int ch2 = getchar();
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            switch (ch2) {
                case 'A': return 'w';
                case 'B': return 's';
                case 'C': return 'd';
                case 'D': return 'a';
            }
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return static_cast<char>(tolower(ch));
}
#endif
