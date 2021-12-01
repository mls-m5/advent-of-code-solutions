#include <filesystem>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using namespace std::literals;

int main(int argc, char **argv) {
    auto args = std::vector<std::string_view>(argv + 1, argv + argc);
    auto isTest = (!args.empty() && args.at(0) == "--test");

    auto path =
        std::filesystem::path{"data/01"s + (isTest ? "-test.txt" : ".txt")};

    auto file = std::ifstream{path};

    if (!file) {
        std::cerr << "could not open " << path << "\n";
        exit(1);
    }

    //    auto depths = std::vector<int>{};

    int previous = {};
    file >> previous;

    int sum = 0;

    for (int depth; file >> depth;) {
        std::cout << depth << "\n";

        if (depth > previous) {
            std::cout << "increased\n";
            ++sum;
        }

        previous = depth;
    }

    std::cout << "sum " << sum << std::endl;
}
