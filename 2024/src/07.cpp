#include <charconv>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

enum Operators {
    Add,
    Mul,
    OperatorCount,
    Invalid,
};

using std::ranges::iota_view;

struct Equation {
    uint64_t expectedResult = 0;
    std::vector<uint64_t> values;
    std::vector<Operators> testOperators;

    Equation(std::string line) {
        auto ss = std::istringstream{line};
        auto first = std::string{};
        ss >> first;
        first.pop_back();
        std::from_chars(
            first.data(), first.data() + first.size(), expectedResult);
        for (uint64_t value = 0; ss >> value;) {
            values.push_back(value);
        }

        testOperators.resize(values.size() - 1, Invalid);
    }

    void print() const {
        std::cout << std::format("{}: ", expectedResult);

        for (auto v : values) {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }

    bool isValid() const {
        uint64_t tmp = values.front();
        for (auto i : iota_view{1uz, values.size()}) {
            switch (testOperators.at(i - 1)) {
            case Add:
                tmp += values.at(i);
                break;
            case Mul:
                tmp *= values.at(i);
                break;
            default:
                throw std::runtime_error{"invalid operator"};
            }
        }
        return tmp == expectedResult;
    }

    bool isAnyValid() {
        // The easiest way to make permutations on a computer
        const auto maxNum = 1 << values.size();
        for (auto currentPermutation : iota_view{0, maxNum}) {
            for (size_t i : iota_view(0uz, testOperators.size())) {
                testOperators.at(i) =
                    static_cast<Operators>(!!((1 << i) & currentPermutation));
            }

            if (isValid()) {
                return true;
            }
        }

        return false;
    }
};

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/07"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto equations = std::vector<Equation>{};

    for (std::string line; std::getline(file, line);) {
        equations.emplace_back(line);
    }

    std::cout << "\n";

    long sum1 = 0;
    for (auto &e : equations) {
        e.print();
        auto isValid = e.isAnyValid();
        sum1 += isValid * e.expectedResult;

        if (isValid) {
            e.print();
        }
    }

    std::cout << std::format("Part1: {}\n", sum1);
}
