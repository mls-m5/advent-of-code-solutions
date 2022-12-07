#include "input.h"
#include <filesystem>
#include <iostream>
#include <regex>
#include <unordered_map>

int main(int argc, char *argv[]) {
    auto file = openFile("07", argc, argv);

    auto cdRe = std::regex{"\\$ cd (.*)"};
    auto fileRe = std::regex{"([0-9]+) (.*)"};

    auto dirSizes = std::unordered_map<std::string, int>{};
    auto current = std::filesystem::path{"/"};

    auto countSize = [&dirSizes](std::filesystem::path path, int size) {
        for (; path != "/"; path = path.parent_path()) {
            dirSizes[path.string()] += size;
        }
        dirSizes[path.string()] += size; // count "/"
    };

    for (std::string line; getline(file, line);) {
        auto match = std::smatch{};
        if (line == "$ cd /") {
            current = "/"; // First line only
        }
        else if (line == "$ cd ..") {
            current = current.parent_path();
        }
        else if (std::regex_match(line, match, cdRe)) {
            current = current / match[1].str();
        }
        else if (std::regex_match(line, match, fileRe)) {
            countSize(current, std::stoi(match[1].str()));
        }
    }

    int result1 = 0;

    size_t storageSize = 70000000;
    auto requiredSpace = 30000000 - (storageSize - dirSizes["/"]);
    auto smallestSize = storageSize;

    for (auto &it : dirSizes) {
        std::cout << "dir " << it.first << ": " << it.second << "\n";
        if (it.second < 100000) {
            result1 += it.second;
        }

        if (it.second > requiredSpace && it.second < smallestSize) {
            smallestSize = it.second;
        }
    }

    std::cout << "result 1: " << result1 << "\n";
    std::cout << "result 2: " << smallestSize << "\n";
}
