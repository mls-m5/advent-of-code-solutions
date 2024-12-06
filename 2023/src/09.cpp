#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

using IntT = int64_t;

IntT report(std::vector<IntT> values) {
    IntT sum = 0;

    int offset = 0;
    int maxSize = 0;

    for (auto value : values) {
        auto str = std::to_string(value);
        maxSize = std::max<size_t>(maxSize, str.size());
    }

    maxSize = maxSize / 2 * 2 + 2; // Make only even numbers

    for (auto diff = std::vector<IntT>{};
         std::accumulate(values.begin(), values.end(), 0);
         std::swap(values, diff), diff.clear()) {

        sum += values.back();

        std::adjacent_difference(
            values.begin(),
            values.end(),
            std::back_inserter(diff) //,
            // [](auto a, auto b) { return b > a ? b - a : a - b; }
        );

        diff.erase(diff.begin());

        ++offset;
        for (int i = 0; i < offset; ++i) {
            std::cout << std::setw(maxSize / 2) << " ";
        }

        for (auto v : values) {
            std::cout << std::setw(maxSize) << v;
        }

        std::cout << " -> " << sum;

        std::cout << "\n";

        // std::cout << sum << std::endl;
    }

    std::cout << "\n";

    return sum;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/09"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    IntT sum = 0;

    for (std::string line; std::getline(file, line);) {
        auto values = std::vector<IntT>{};
        auto ss = std::istringstream{line};

        /// >> does not seem to capture negative values
        for (std::string value; ss >> value;) {
            values.push_back(std::stol(value));
        }

        sum += report(std::move(values));
    }

    std::cout << "final sum 1: " << sum << std::endl;
}
