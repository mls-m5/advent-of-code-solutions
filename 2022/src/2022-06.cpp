#include "input.h"
#include <iostream>
#include <set>

int main(int argc, char *argv[]) {
    auto file = openFile("06", argc, argv);

    auto findSequence = [](auto &line, int markerSize) {
        for (size_t i = markerSize; i < line.size(); ++i) {
            auto set =
                std::set<char>{line.begin() + i - markerSize, line.begin() + i};
            if (set.size() == markerSize) {
                return i;
            }
        }
        return size_t{};
    };

    for (std::string line; getline(file, line);) {
        std::cout << "start sequence at " << findSequence(line, 4);
        std::cout << ", message at " << findSequence(line, 14) << "\n";
    }
}
