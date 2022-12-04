#include "input.h"
#include <iostream>
#include <regex>
#include <sstream>

using P = std::pair<int, int>;

bool isContained(P a, P b) {
    return a.first >= b.first && a.second <= b.second;
}

int main(int argc, char *argv[]) {
    auto file = openFile("04", argc, argv);

    auto re = std::regex{"([0-9]+)-([0-9]+),([0-9]+)-([0-9]+)"};

    int result1 = 0;

    for (std::string line; getline(file, line);) {
        auto matches = std::smatch{};

        std::regex_match(line, matches, re);

        //        std::cout << matches[1].str() << ", " << matches[2].str() <<
        //        ", "
        //                  << matches[3].str() << ", " << matches[4].str() <<
        //                  std::endl;

        P left{std::stoi(matches[1].str()), std::stoi(matches[2].str())};
        P right{std::stoi(matches[3].str()), std::stoi(matches[4].str())};

        if (isContained(left, right) || isContained(right, left)) {
            //            std::cout << "is contained in the other\n";
            ++result1;
        }
    }

    std::cout << "\n";

    std::cout << "result 1: " << result1 << std::endl;

    return 0;
}
