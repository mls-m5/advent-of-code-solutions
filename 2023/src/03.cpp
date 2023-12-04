
#include <cctype>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

struct Num {
    int x = 0;
    int y = 0;

    std::string value;
    std::string symbols;

    bool isActive = false;

    int intValue() {
        return std::stoi(value);
    }

    bool isNear(int ox, int oy) {
        if (ox + 1 >= x && ox - 1 < x + value.size()) {
            if (oy + 1 >= y && oy - 1 <= y) {
                return true;
            }
        }
        return false;
    }
};

struct Symbol {
    int x = 0;
    int y = 0;

    char value = 0;
};

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/03"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    int y = 0;

    auto numbers = std::vector<Num>{};
    auto symbols = std::vector<Symbol>{};

    for (std::string line; std::getline(file, line);) {
        int x = -1;

        auto current = std::optional<Num>{};

        for (auto c : line) {
            ++x;

            if (!std::isdigit(c)) {
                if (current) {
                    numbers.push_back(*current);
                    current = std::nullopt;
                }

                if (c != '.') {
                    symbols.push_back({.x = x, .y = y, .value = c});
                }

                continue;
            }

            if (!current) {
                current = Num{.x = x, .y = y};
            }

            current->value += c;
        }
        if (current) {
            numbers.push_back(*current);
        }

        ++y;
    }

    for (auto &number : numbers) {
        for (auto &symbol : symbols) {
            if (number.isNear(symbol.x, symbol.y)) {
                number.isActive = true;
                number.symbols += symbol.value;
            }
        }
    }

    for (auto &number : numbers) {
        std::cout << "n: " << number.value << ", " << number.x << ", "
                  << number.y << " active? " << number.symbols << "\n";
    }

    for (auto &symbol : symbols) {
        std::cout << "s: " << symbol.value << ", " << symbol.x << ", "
                  << symbol.y << "\n";
    }

    int sum1 = 0;
    for (auto &number : numbers) {
        sum1 += number.intValue() * number.isActive;
        std::cout << number.intValue() * number.isActive << "\n";
    }

    std::cout << "sum 1: " << sum1 << "\n";

    return 0;
}
