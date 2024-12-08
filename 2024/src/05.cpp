#include <algorithm>
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

struct Page {
    int number;
    float position; // used for solving
};

struct LinkedRule {
    Page *a = nullptr;
    Page *b = nullptr;
    Rule *rule = nullptr;

    // Returns error
    float apply() {
        const auto expected = -1;
        const auto distance = b->position - a->position;
        const auto error = expected - distance;
        if (error < 0) {
            a->position -= error * .1f;
            b->position += error * .1f;
            return error;
        }

        return 0;
    }
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

std::vector<int> sortManual(const std::vector<int> &values,
                            std::vector<Rule> rules) {
    auto errorPages = std::vector<Page>{};
    auto links = std::vector<LinkedRule>{};

    float startLocation = 0;
    for (auto number : values) {
        errorPages.push_back({
            .number = number,
            .position = startLocation,
        });

        ++startLocation;
    }

    for (auto &rule : rules) {
        auto fa = std::find_if(errorPages.begin(),
                               errorPages.end(),
                               [a = rule.a](auto &p) { return p.number == a; });
        auto fb = std::find_if(errorPages.begin(),
                               errorPages.end(),
                               [b = rule.b](auto &p) { return p.number == b; });

        if (fa == errorPages.end()) {
            continue;
        }
        if (fb == errorPages.end()) {
            continue;
        }

        links.push_back({
            .a = &*fa,
            .b = &*fb,
            .rule = &rule,
        });
    }

    for (float error = 100; std::abs(error) > .1;) {
        error = 0;
        for (auto &link : links) {
            error += link.apply();
        }
    }

    std::sort(errorPages.begin(), errorPages.end(), [](auto &a, auto &b) {
        return a.position < b.position;
    });

    auto ret = decltype(values){};
    for (auto &p : errorPages) {
        ret.push_back(p.number);
    }
    return ret;
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
    int sum2 = 0;
    for (auto &manual : manuals) {
        if (isManualValid(manual, rules)) {
            sum1 += manual.at(manual.size() / 2);
        }
        else {
            manual = sortManual(manual, rules);
            sum2 += manual.at(manual.size() / 2);
        }
    }

    std::cout << std::format("Part 1: {}\n", sum1);
    std::cout << std::format("Part 2: {}\n", sum2);
}
