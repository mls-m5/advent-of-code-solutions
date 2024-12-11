#include <cmath>
#include <cstdint>
#include <format>
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <string>
#include <unordered_map>

using NumberContainerT = std::unordered_map<uint64_t, uint64_t>;
using std::ranges::iota_view;

uint64_t numberOfDigits(uint64_t num) {
    uint64_t digits = 0;
    for (; num; num /= 10, ++digits) {
    }
    return digits;
}

uint64_t getMultiplier(uint64_t digits) {
    auto ret = 1;
    for (auto i : iota_view(uint64_t{1}, digits)) {
        ret *= 10;
    }
    return ret;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/11"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto numbers = NumberContainerT{};

    for (int num = 0; file >> num;) {
        ++numbers[num];
    }

    auto count = [](int iterations, auto numbers) {
        auto next = decltype(numbers){};
        for (int i = 0; i < iterations; ++i) {
            for (auto &it : numbers) {
                if (it.first == 0) {
                    next[1] += it.second;
                }
                else if (auto digits = numberOfDigits(it.first);
                         digits % 2 == 0) {
                    auto multiplier = getMultiplier(digits / 2 + 1);
                    auto a = it.first / multiplier;
                    auto b = it.first % multiplier;
                    next[a] += it.second;
                    next[b] += it.second;
                }
                else {
                    next[it.first * 2024] += it.second;
                }
            }
            std::swap(numbers, next);
            next.clear();

            // for (auto &it : numbers) {
            //     std::cout << std::format("{}={} ", it.first, it.second);
            // }
            // std::cout << "\n";
        }
        auto sum1 = std::accumulate(
            numbers.begin(),
            numbers.end(),
            uint64_t{},
            [](uint64_t sum, auto &b) { return sum + b.second; });

        return sum1;
    };

    auto sum1 = count(25, numbers);
    auto sum2 = count(75, numbers);

    std::cout << std::format("Part 1: {}\n", sum1);
    std::cout << std::format("Part 2: {}\n", sum2);
}
