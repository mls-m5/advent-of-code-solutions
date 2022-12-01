#pragma once

#include <filesystem>
#include <fstream>
#include <iostream>
#include <vector>

std::ifstream openFile(std::string name, int argc, char *argv[]) {
    using namespace std::literals;

    auto args = std::vector<std::string_view>(argv + 1, argv + argc);
    auto isTest = (!args.empty() && args.at(0) == "--test");

    auto path = std::filesystem::path{"data/"s + name +
                                      (isTest ? "-test.txt" : ".txt")};

    auto file = std::ifstream{path};

    if (!file) {
        std::cerr << "could not open " << path << "\n";
        exit(1);
    }

    return file;
}
