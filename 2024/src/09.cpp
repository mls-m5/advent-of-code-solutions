#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using std::ranges::iota_view;

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/09"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    std::string line;
    std::getline(file, line);

    auto disk = std::vector<int>{};

    auto view = std::string_view{line};
    int fileNum = 0;
    for (;; view.remove_prefix(1)) {
        if (view.empty()) {
            break;
        }

        int size = static_cast<int>(view.front() - '0');

        for (auto i : iota_view{0, size}) {
            (void)i;
            disk.push_back(fileNum);
        }

        view.remove_prefix(1);

        if (view.empty()) {
            break;
        }

        int space = static_cast<int>(view.front() - '0');

        for (auto i : iota_view{0, space}) {
            (void)i;
            disk.push_back(-1);
        }

        ++fileNum;
    }

    auto print = [](auto &disk) {
        for (auto d : disk) {
            // std::cout.width(3);
            // std::cout.fill(' ');
            std::cout << ((d == -1) ? std::string{"."} : std::to_string(d));
        }
        std::cout << "\n";
    };

    int nextFree = 0;
    // print(disk);
    for (int i = disk.size() - 1; i >= 0 && i > nextFree; --i) {
        if (disk.at(i) == -1) {
            continue;
        }
        for (; nextFree < i && disk.at(nextFree) != -1; ++nextFree) {
        }

        disk.at(nextFree) = std::exchange(disk.at(i), -1);
        // print(disk);
    }

    uint64_t sum1 = 0;

    for (auto i : iota_view{0uz, disk.size()}) {
        if (disk.at(i) == -1) {
            break;
        }
        sum1 += i * disk.at(i);
    }

    std::cout << std::format("Part 1: {}\n", sum1);
}
