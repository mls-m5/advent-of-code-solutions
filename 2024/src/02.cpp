#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/02"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    int numSafe1 = 0;
    int numSafe2 = 0;

    auto numbers = std::vector<std::vector<int>>{};

    for (std::string line; std::getline(file, line);) {
        auto ss = std::istringstream{line};

        numbers.emplace_back();
        for (int i = 0; ss >> i;) {
            numbers.back().push_back(i);
        }
    }

    auto isSafeF = [](auto &nums, int ignore = 0) {
        bool isSafe = true;
        auto prev = std::optional<int>{};
        int dir = 0;

        for (int i = 0; i < nums.size(); ++i) {
            if (ignore == i) {
                continue;
            }
            auto value = nums.at(i);
            if (!prev) {
                prev = value;
                continue;
            }

            auto d = *prev - value;
            auto dist = std::abs(d);
            auto tmpDir = std::copysign(1, d);

            if (dist < 1 || dist > 3) {
                isSafe = false;
                break;
            }

            if (dir == 0) {
                dir = tmpDir;
            }

            if (dir != tmpDir) {
                isSafe = false;
            }

            dir = tmpDir;
            prev = value;
        }
        return isSafe;
    };

    auto toleratingIsSafe = [&isSafeF](auto &nums) {
        if (isSafeF(nums)) {
            return true;
        }

        for (size_t i = 0; i < nums.size(); ++i) {
            if (isSafeF(nums, i)) {
                return true;
            }
        }

        return false;
    };

    for (auto &nums : numbers) {
        numSafe1 += isSafeF(nums);
        numSafe2 += toleratingIsSafe(nums);
    }

    std::cout << std::format("Part 1: {}\n", numSafe1);
    std::cout << std::format("Part 2: {}\n", numSafe2);
}
