#include <algorithm>
#include <fstream>
#include <future>
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

    auto remap = [&maps](SizeT seed) {
        for (auto &map : maps) {
            seed = map.at(seed);
        }
        return seed;
    };

    auto results = std::vector<std::future<SizeT>>{};

    for (size_t i = 0; i < seeds.size(); ++i, ++i) {
        auto f = [i, &seeds, remap] {
            auto localMin = std::numeric_limits<SizeT>::max();
            std::cout << i << "/" << seeds.size() << std::endl;

            for (int index = 0; index < seeds.at(i + 1); ++index) {
                auto seed = seeds.at(i) + index;

                auto value = remap(seed);

                localMin = std::min(localMin, value);
            }

            std::cout << i << "/" << seeds.size() << " done -> " << localMin
                      << " ..." << std::endl;
            return localMin;
        };

        results.push_back(std::async(f));
    }

    auto min = std::numeric_limits<SizeT>::max();
    for (auto &res : results) {
        min = std::min(res.get(), min);
        std::cout << "current min: " << min << std::endl;
    }

    std::cout << "calculated min:" << min << std::endl;

    std::cout << "\n";

    for (auto &seed : seeds) {
        seed = remap(seed);
    }

    std::cout << "answer 1: " << *std::min_element(seeds.begin(), seeds.end())
              << std::endl;
    std::cout << "answer 2: " << min << std::endl;
}
