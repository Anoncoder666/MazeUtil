#include <vector>
#include <string>
#include <iostream>

std::string fgcolor = "\033[40m⠀⠀\033[0m";
std::string bgcolor = "\033[107m⠀⠀\033[0m";

using namespace std;
void print (vector<vector<unsigned char>>& v) {

    for (auto& row : v) {
        for (auto && t : row) {
            cout << (t ? fgcolor : bgcolor);
        }
        cout << endl;
    }
}