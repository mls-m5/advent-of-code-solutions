#include "input.h"
#include <algorithm>
#include <filesystem>
#include <iostream>
#include <regex>
#include <unordered_map>

int main(int argc, char *argv[]) {
    auto file = openFile("07", argc, argv);

    auto current = std::filesystem::path{"/"};

    auto cdRe = std::regex{"\\$ cd (.*)"};
    auto fileRe = std::regex{"([0-9]+) (.*)"};

    auto dirSizes = std::unordered_map<std::string, int>{};

    auto countSize = [&dirSizes](std::filesystem::path path, int size) {
        for (; path != "/"; path = path.parent_path()) {
            dirSizes[path.string()] += size;
        }
    };

    for (std::string line; getline(file, line);) {
        auto match = std::smatch{};
        if (line == "$ cd /") {
            current = "/"; // First line only
        }
        else if (line == "$ cd ..") {
            std::cout << "cd parent\n";
            current = current.parent_path();
        }
        else if (std::regex_match(line, match, cdRe)) {
            std::cout << "cd into " << match[1] << "\n";
            current = current / match[1].str();
        }
        else if (std::regex_match(line, match, fileRe)) {
            std::cout << "counting file " << match[2] << "\n";
            countSize(current, std::stoi(match[1].str()));
        }

        std::cout << "current: " << current << "\n";
    }

    int result1 = 0;

    for (auto &it : dirSizes) {
        std::cout << "dir " << it.first << ": " << it.second << "\n";
        if (it.second < 100000) {
            result1 += it.second;
        }
    }

    std::cout << "result 1: " << result1 << "\n";
}
