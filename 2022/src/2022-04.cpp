#include "input.h"
#include <iostream>
#include <regex>
#include <sstream>

using Pair = std::pair<int, int>;

// Is a contained in b?
bool isContained(Pair a, Pair b) {
    return a.first >= b.first && a.second <= b.second;
}

bool doesOverlap(Pair a, Pair b) {
    return isContained({a.first, a.first}, b) ||
           isContained({a.second, a.second}, b) || isContained(b, a);
}

int main(int argc, char *argv[]) {
    auto file = openFile("04", argc, argv);

    auto re = std::regex{"([0-9]+)-([0-9]+),([0-9]+)-([0-9]+)"};

    int result1 = 0;
    int result2 = 0;

    for (std::string line; getline(file, line);) {
        auto matches = std::smatch{};

        std::regex_match(line, matches, re);

        Pair left{std::stoi(matches[1].str()), std::stoi(matches[2].str())};
        Pair right{std::stoi(matches[3].str()), std::stoi(matches[4].str())};

        if (isContained(left, right) || isContained(right, left)) {
            ++result1;
        }

        if (doesOverlap(left, right)) {
            ++result2;
        }
    }

    std::cout << "\n";

    std::cout << "result 1: " << result1 << std::endl;
    std::cout << "result 2: " << result2 << std::endl;

    return 0;
}
