#include <fstream>
#include <iostream>
#include <ranges>
#include <string>

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/01"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    int sum = 0;
    for (std::string line; std::getline(file, line);) {
        int res = 0;
        for (auto c : line) {
            if (std::isdigit(c)) {
                res += (c - '0') * 10;
                break;
            }
        }
        for (auto c : std::ranges::reverse_view{line}) {
            if (std::isdigit(c)) {
                res += (c - '0');
                break;
            }
        }

        std::cout << res << std::endl;
        sum += res;
    }

    std::cout << sum << std::endl;

    return 0;
}
