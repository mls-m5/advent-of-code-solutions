#include "input.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>

struct DrawResult {
    std::string name;
    int score = 0;
};

auto res = std::unordered_map<std::string, DrawResult>{};

int main(int argc, char *argv[]) {
    auto file = openFile("02", argc, argv);

    for (auto a = 'A'; a <= 'C'; ++a) {
        for (auto x = 'X'; x <= 'Z'; ++x) {
            auto name = std::string{};
            name.push_back(a);
            name.push_back(x);

            auto &res = ::res[name];

            int offset = (3 + (x - 'X') - (a - 'A')) % 3;

            res.score = (x - 'X') + 1;
            res.name = name;

            switch (offset) {
            case 0: // Draw
                res.score += 3;
                break;
            case 1: // Win
                res.score += 6;
                break;
            case 2: // Loose
                res.score += 0;
                break;
            }
        }
    }

    int result = 0;

    for (std::string line; getline(file, line);) {
        auto a = std::string{};
        auto x = std::string{};
        auto ss = std::istringstream{line};
        ss >> a;
        ss >> x;

        auto name = a + x;

        auto &r = res.at(name);
        result += r.score;
    }

    std::cout << "score: " << result << std::endl;

    return 0;
}
