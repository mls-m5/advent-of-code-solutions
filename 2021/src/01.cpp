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

    auto depths = std::vector<int>{};

    int previous = {};
    file >> previous;
    depths.push_back(previous);

    int sum = 0;

    for (int depth; file >> depth;) {

        if (depth > previous) {
            ++sum;
        }

        previous = depth;

        depths.push_back(depth);
    }

    std::cout << "sum " << sum << std::endl;

    int previousWindow = depths.at(0) + depths.at(1) + depths.at(2);
    int windowSum = 0;

    std::cout << "first " << previousWindow;

    for (size_t i = 2; i < depths.size(); ++i) {
        int window = depths.at(i + 0) + depths.at(i - 1) + depths.at(i - 2);

        if (window > previousWindow) {
            ++windowSum;
        }

        previousWindow = window;
    }

    std::cout << "windowed sum " << windowSum << std::endl;
}
