#include "input.h"
#include <algorithm>
#include <iostream>

int main(int argc, char *argv[]) {
    auto file = openFile("01", argc, argv);

    auto calories = std::vector<long>{};
    calories.emplace_back(0);

    for (std::string line; getline(file, line);) {
        std::cout << line << std::endl;

        if (line.empty()) {
            calories.push_back(0);
        }
        else {
            calories.back() += std::stol(line);
        }
    }

    std::sort(calories.begin(), calories.end());

    std::cout << calories.back() << std::endl;

    return 0;
}
