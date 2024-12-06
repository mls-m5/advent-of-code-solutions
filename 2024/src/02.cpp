#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/02"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    int numSafe = 0;

    for (std::string line; std::getline(file, line);) {
        auto ss = std::istringstream{line};

        bool isSafe = true;
        auto prev = std::optional<int>{};
        int dir = 0;

        for (int i = 0; ss >> i; prev = i) {
            if (!prev) {
                continue;
            }

            auto d = *prev - i;
            auto dist = std::abs(d);
            auto tmpDir = std::copysign(1, d);

            if (dist < 1 || dist > 3) {
                isSafe = false;
                break;
            }

            if (dir == 0) {
                dir = tmpDir;
            }

            if (dir != tmpDir) {
                isSafe = false;
            }

            dir = tmpDir;
        }

        numSafe += isSafe;
    }

    std::cout << std::format("Part 1: {}\n", numSafe);
}
