#include <algorithm>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/01"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto v1 = std::vector<int>{};
    auto v2 = std::vector<int>{};

    for (std::string line; std::getline(file, line);) {
        auto ss = std::istringstream{line};
        auto i1 = 0;
        auto i2 = 0;
        ss >> i1 >> i2;
        v1.push_back(i1);
        v2.push_back(i2);
    }

    std::sort(v1.begin(), v1.end());
    std::sort(v2.begin(), v2.end());

    int sum1 = 0;
    int sum2 = 0;

    auto m = std::map<int, int>{};

    for (auto it1 = v1.begin(), it2 = v2.begin(); it1 != v1.end();
         ++it1, ++it2) {
        auto a = *it1;
        auto b = *it2;
        sum1 += std::abs(a - b);

        ++m[b];
    }

    for (auto v : v1) {
        sum2 += v * m[v];
    }

    std::cout << std::format("Part 1: {}\n", sum1);
    std::cout << std::format("Part 2: {}\n", sum2);
}
