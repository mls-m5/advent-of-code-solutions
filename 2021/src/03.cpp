#include "openfile.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct BitStats {
    std::vector<int> numOnes;
    std::vector<int> numZeros;

    void handle(std::string_view line) {
        std::cout << line << std::endl;
        if (numOnes.empty()) {
            numOnes.resize(line.size());
            numZeros.resize(line.size());
        }

        for (size_t i = 0; i < line.size(); ++i) {
            auto c = line.at(i);
            if (c == '1') {
                ++numOnes.at(i);
            }
            else {
                ++numZeros.at(i);
            }
        }
    }

    std::pair<int, int> sum() {
        int gamma = 0;
        int epsilon = 0;
        for (size_t i = 0; i < numOnes.size(); ++i) {
            gamma <<= 1;
            epsilon <<= 1;
            if (numOnes.at(i) > numZeros.at(i)) {
                gamma |= 1;
            }
            else {
                epsilon |= 1;
            }
        }
        return {gamma, epsilon};
    }
};

void solve(std::istream &file) {
    auto stats = BitStats{};

    for (std::string line; getline(file, line);) {
        stats.handle(line);
    }

    for (auto n : stats.numOnes) {
        std::cout << n << " ";
    }
    auto [gamma, epsilon] = stats.sum();

    std::cout << std::endl;
    std::cout << gamma << "\n";
    for (auto n : stats.numZeros) {
        std::cout << n << " ";
    }
    std::cout << std::endl;
    std::cout << epsilon << "\n";
    std::cout << gamma * epsilon << "\n";
}

int main(int argc, char *argv[]) {
    auto file = openFile("03", argc, argv);
    solve(file);

    return 0;
}
