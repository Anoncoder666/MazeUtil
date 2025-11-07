#pragma once
#include <iostream>

#ifdef _WIN32
#include <conio.h>

inline bool keyPressed() {
    return _kbhit(); // non-blocking key check
}

inline char getKey() {
    return _getch(); // non-blocking key read, no echo
}

#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

inline void setNonBlocking(bool enable) {
    static bool initialized = false;
    static termios oldt{};
    static int oldf = 0;

    if (!initialized) {
        tcgetattr(STDIN_FILENO, &oldt);
        initialized = true;
    }

    termios newt = oldt;
    if (enable) {
        newt.c_lflag &= ~(ICANON | ECHO); // disable line buffering and echo
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
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
        ungetc(ch, stdin); // put character back
        setNonBlocking(false);
        return true;
    }

    setNonBlocking(false);
    return false;
    }

inline char getKey() {
    setNonBlocking(true);
    int ch = getchar();
    while (ch == EOF) { // wait until key is pressed
        ch = getchar();
    }

    setNonBlocking(false);
    return static_cast<char>(ch);
    }

inline char getNormalizedKey() {
#ifdef _WIN32
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
    return ch;
#else
    int ch = getKey(); // your blocking getKey()
    if (ch == 27) { // ESC
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
    return ch;
#endif
}

#endif
