#include "input.h"
#include <algorithm>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[]) {
    auto file = openFile("02", argc, argv);

    int result1 = 0;
    int result2 = 0;

    for (std::string line; getline(file, line);) {
        auto a = line.front();
        auto x = line.at(2);

        { // 1
            int offset = (3 + (x - 'X') - (a - 'A')) % 3;

            result1 += (x - 'X') + 1;
            result1 += ((offset + 1) % 3) * 3;
        }

        { // 2
            int move = 'A' + ((3 + 2 + (x - 'X') + (a - 'A')) % 3);
            result2 += (move - 'A') + 1;
            result2 += (x - 'X') * 3;
        }
    }

    std::cout << "score 1: " << result1 << std::endl;
    std::cout << "score 2: " << result2 << std::endl;

    return 0;
}
