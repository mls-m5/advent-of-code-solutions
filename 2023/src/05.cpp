#include <algorithm>
#include <fstream>
#include <iostream>
#include <istream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using SizeT = long long;

struct Map {

    struct Range {
        SizeT destination = 0;
        SizeT source = 0;
        SizeT length = 0;
    };

    Map(std::istream &file) {
        // The title is useless
        {
            auto discard = std::string{};
            std::getline(file, discard);
        }

        for (std::string line; std::getline(file, line);) {
            if (line.empty()) {
                break;
            }

            auto range = Range{};

            auto ss = std::istringstream{line};
            ss >> range.destination >> range.source >> range.length;

            ranges.push_back(range);
        }
    }

    SizeT at(SizeT value) {
        for (auto &range : ranges) {
            if (value >= range.source && value < range.source + range.length) {
                return value + range.destination - range.source;
            }
        }
        return value;
    }

    std::vector<Range> ranges;
};

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/05"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto seeds = std::vector<SizeT>{};
    auto maps = std::vector<Map>{};

    {
        auto line = std::string{};
        std::getline(file, line);
        auto ss = std::istringstream{line};

        std::string discard;

        ss >> discard; // "seeds:"

        for (SizeT i = 0; ss >> i;) {
            seeds.push_back(i);
        }
        std::getline(file, line); // Discard empty line
    }

    for (;;) {
        auto map = Map{file};

        if (map.ranges.empty()) {
            break;
        }

        maps.push_back(std::move(map));
    }

    for (auto &seed : seeds) {
        std::cout << seed << " ";
    }

    std::cout << "\n";

    for (auto &map : maps) {
        for (auto &seed : seeds) {
            seed = map.at(seed);

            std::cout << seed << " ";
        }

        std::cout << "\n";
    }

    std::cout << "answer 1: " << *std::min_element(seeds.begin(), seeds.end())
              << std::endl;
}
