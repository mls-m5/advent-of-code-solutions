
#include "input.h"
#include <algorithm>
#include <iostream>
#include <set>

int main(int argc, char *argv[]) {
    auto file = openFile("06", argc, argv);

    for (std::string line; getline(file, line);) {
        for (size_t i = 4; i < line.size(); ++i) {
            auto set = std::set<char>{};
            auto markerTest = line.substr(i - 4, 4);
            set.insert(markerTest.begin(), markerTest.end());
            if (set.size() == 4) {
                std::cout << "found start sequence at " << i << "\n";
                break;
            }
        }
    }
}
