
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

    bool isActive() {
        return !symbols.empty();
    }

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
    int ratio = 1;
    int count = 0;

    void multiply(int value) {
        ratio *= value;
        ++count;
    }

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
                number.symbols += symbol.value;
                symbol.multiply(number.intValue());
            }
        }
    }

    int sum1 = 0;
    for (auto &number : numbers) {
        std::cout << "n: " << number.value << ", " << number.x << ", "
                  << number.y << " active? " << number.symbols << "\n";
        auto val = number.intValue() * number.isActive();
        sum1 += val;
        std::cout << val << "\n";
    }

    int sum2 = 0;
    for (auto &symbol : symbols) {
        std::cout << "s: " << symbol.value << ", " << symbol.x << ", "
                  << symbol.y << ", ratio: " << symbol.ratio << "\n";
        sum2 += symbol.ratio * (symbol.count > 1);
    }

    std::cout << "sum 1: " << sum1 << "\n";
    std::cout << "sum 2: " << sum2 << "\n";

    return 0;
}
