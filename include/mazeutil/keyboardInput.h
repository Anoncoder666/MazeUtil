#pragma once
#include <iostream>

#ifdef _WIN32
#include <conio.h>

inline bool keyPressed() {
    return _kbhit(); // non-blocking key check
}

inline char getKey() {
    return _getch(); // blocking key read
}

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
    return static_cast<char>(ch);
}

#else // Linux/Unix
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

inline void setNonBlocking(bool enable) {
    static bool initialized = false;
    static termios oldt{};
    static int oldf = 0;

    if (!initialized) {
        tcgetattr(STDIN_FILENO, &oldt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
        initialized = true;
    }

    termios newt = oldt;
    if (enable) {
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    } else {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        fcntl(STDIN_FILENO, F_SETFL, oldf);
    }
}

inline bool keyPressed() {
    setNonBlocking(true);
    int ch = getchar();
    if (ch != EOF) {
        ungetc(ch, stdin);
        setNonBlocking(false);
        return true;
    }
    setNonBlocking(false);
    return false;
}

inline char getKey() {
    setNonBlocking(true);
    int ch = getchar();
    while (ch == EOF) { // wait for input
        ch = getchar();
    }
    setNonBlocking(false);
    return static_cast<char>(ch);
}

inline char getNormalizedKey() {
    int ch = getKey();
    if (ch == 27) { // ESC sequence for arrow keys
        int ch1 = getKey();
        if (ch1 == '[') {
            int ch2 = getKey();
            switch (ch2) {
                case 'A': return 'w'; // Up
                case 'B': return 's'; // Down
                case 'C': return 'd'; // Right
                case 'D': return 'a'; // Left
            }
        }
    }
    return static_cast<char>(ch);
}
#endif
