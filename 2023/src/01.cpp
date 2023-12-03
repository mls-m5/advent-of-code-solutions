#include <array>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

using namespace std::literals;

constexpr auto numbers = std::array<std::pair<int, std::string_view>, 9>{
    {{1, "one"},
     {2, "two"},
     {3, "three"},
     {4, "four"},
     {5, "five"},
     {6, "six"},
     {7, "seven"},
     {8, "eight"},
     {9, "nine"}},
};

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/01"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    int sum = 0;
    for (std::string line; std::getline(file, line);) {
        int res = 0;
        for (size_t i = 0; i < line.size(); ++i) {
            auto c = line.at(i);
            if (std::isdigit(c)) {
                res += (c - '0') * 10;
                break;
            }

            bool shouldBreak = false;

            for (auto number : numbers) {
                if (std::string_view{line}.substr(i, number.second.size()) ==
                    number.second) {
                    res += number.first * 10;
                    shouldBreak = true;
                }
            }

            if (shouldBreak) {
                break;
            }
        }
        for (int i = line.size() - 1; i >= 0; --i) {
            auto c = line.at(i);
            if (std::isdigit(c)) {
                res += (c - '0');
                break;
            }

            bool shouldBreak = false;
            for (auto number : numbers) {
                if (std::string_view{line}.substr(i, number.second.size()) ==
                    number.second) {
                    res += number.first;
                    shouldBreak = true;
                }
            }
            if (shouldBreak) {
                break;
            }
        }

        std::cout << res << std::endl;
        sum += res;
    }

    std::cout << sum << std::endl;

    return 0;
}
