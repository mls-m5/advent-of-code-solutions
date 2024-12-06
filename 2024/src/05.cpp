#include <cstddef>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

struct Rule {
    int a = 0;
    int b = 0;
};

bool isManualValid(const std::vector<int> &manual,
                   const std::vector<Rule> &rules) {
    for (size_t i = 0; i < manual.size(); ++i) {
        for (size_t j = i + 1; j < manual.size(); ++j) {
            auto a = manual.at(i);
            auto b = manual.at(j);
            for (auto &rule : rules) {
                if (a == rule.b && b == rule.a) {
                    return false;
                }
            }
        }
    }
    return true;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/05"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto rules = std::vector<Rule>{};

    auto manuals = std::vector<std::vector<int>>{};

    for (std::string line; std::getline(file, line);) {
        std::cout << line << std::endl;
        if (line.empty()) {
            break;
        }

        auto a = std::stoi(line.substr(0, 2));
        auto b = std::stoi(line.substr(3));
        rules.push_back({a, b});
    }

    for (std::string line; std::getline(file, line);) {
        manuals.emplace_back();

        auto view = std::string_view{line};

        for (std::string n; n = view.substr(0, 2), !view.empty();
             view.size() >= 3 ? view.remove_prefix(3) : view.remove_prefix(2)) {
            manuals.back().push_back(std::stoi(n));
        }
    }

    for (auto &rule : rules) {
        std::cout << std::format("{} -> {}\n", rule.a, rule.b);
    }

    int sum1 = 0;
    for (auto &manual : manuals) {
        if (isManualValid(manual, rules)) {
            sum1 += manual.at(manual.size() / 2);
        }
    }

    std::cout << std::format("Part 1: {}\n", sum1);
}
