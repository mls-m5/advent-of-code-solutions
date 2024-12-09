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

using DiskT = std::vector<int>;

void print(auto &disk) {
    for (auto d : disk) {
        std::cout << ((d == -1) ? std::string{"."} : std::to_string(d));
    }
    std::cout << "\n";
};

int findFree(const DiskT &disk, int size) {
    for (auto i : iota_view{0uz, disk.size() - size}) {
        auto found = true;
        for (auto s : iota_view{i, i + size}) {
            if (disk.at(s) != -1) {
                found = false;
                break;
            }
        }

        if (found) {
            return i;
        }
    }

    return -1;
}

auto checkSum(DiskT &disk) {
    uint64_t sum = 0;
    for (auto i : iota_view{0uz, disk.size()}) {
        if (disk.at(i) == -1) {
            continue;
        }
        sum += i * disk.at(i);
    }
    return sum;
};

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/09"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    std::string line;
    std::getline(file, line);

    auto disk = DiskT{};

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

    auto disk2 = disk;

    {
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
    }

    {
        for (int i = disk2.size() - 1; i >= 0; --i) {
            if (disk2.at(i) == -1) {
                continue;
            }

            auto fileNum = disk2.at(i);

            int size = 1;
            for (int j = 1; i - j >= 0; ++j) {
                if (disk2.at(i - j) != fileNum) {
                    break;
                }
                ++size;
            }

            auto slot = findFree(disk2, size);
            i -= size - 1;
            if (slot == -1 || slot > i) {
                continue;
            }

            for (auto j : iota_view(0, size)) {
                disk2.at(slot + j) = fileNum;
                disk2.at(i + j) = -1;
            }

            // print(disk2);
        }
    }

    auto sum1 = checkSum(disk);
    auto sum2 = checkSum(disk2);

    std::cout << std::format("Part 1: {}\n", sum1);
    std::cout << std::format("Part 2: {}\n", sum2);
}
