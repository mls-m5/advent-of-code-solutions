#include <cctype>
#include <charconv>
#include <format>
#include <fstream>
#include <iostream>
#include <libintl.h>
#include <sstream>
#include <string>
#include <string_view>

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/03"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto content = std::string{};
    for (std::string line; std::getline(file, line);) {
        content += line + "\n";
    }

    std::cout << content << std::endl;

    int sum1 = 0;

    for (auto view = std::string_view{content}; !view.empty();
         view.remove_prefix(1)) {
        if (!view.starts_with("mul(")) {
            continue;
        }
        view.remove_prefix(4);

        if (!std::isdigit(view.front())) {
            continue;
        }

        auto ss1 = std::stringstream{};
        for (; std::isdigit(view.front()); view.remove_prefix(1)) {
            ss1 << view.front();
        }
        int i1 = 0;
        ss1 >> i1;
        // std::cout << ss1.str() << std::endl;

        char c = view.front();
        if (c != ',') {
            continue;
        }
        view.remove_prefix(1);

        auto ss2 = std::stringstream{};
        for (; std::isdigit(view.front()); view.remove_prefix(1)) {
            ss2 << view.front();
        }
        int i2 = 0;
        ss2 >> i2;
        // std::cout << ss2.str() << std::endl;

        c = view.front();
        if (c != ')') {
            continue;
        }

        std::cout << std::format(" {}*{}={}\n", i1, i2, i1 * i2);
        sum1 += i1 * i2;
    }

    std::cout << std::format("Part 1: {}\n", sum1);
}
