#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

using VecVec = std::vector<std::string>;
using Vec3 = std::vector<VecVec>;

VecVec rotate90(const VecVec &v) {
    auto w = v.front().size();
    auto h = v.size();

    auto ret = VecVec{};

    for (size_t x = 0; x < w; ++x) {
        auto line = std::string{};

        for (size_t y = 0; y < h; ++y) {
            line.push_back(v.at(h - y - 1).at(x));
        }

        ret.push_back(line);
    }

    return ret;
}

VecVec rotate45(const VecVec &v) {
    auto w = v.front().size();
    auto h = v.size();

    auto ret = VecVec{};
    ret.resize(w + h);

    auto wh = w + h;

    for (size_t y = 0; y < h + w; ++y) {
        ret.at(y).resize(std::abs(static_cast<int>(y - h + 1)), '-');
    }

    for (size_t x = 0; x < w; ++x) {

        for (size_t y = 0; y < h; ++y) {
            auto &line = ret.at(x + y);
            line.push_back(v.at(h - y - 1).at(x));
            line.push_back('-');
        }
    }

    for (auto &line : ret) {
        line.resize(wh, '-');
    }

    return ret;
}

void print(const VecVec &v) {
    for (auto &line : v) {
        std::cout << line << "\n";
    }
    std::cout << std::endl;
}

int count(const VecVec &v, std::string_view str) {
    int sum = 0;
    for (auto &line : v) {
        for (size_t x = 0; (x = line.find(str, x)) != std::string::npos;
             x += str.size()) {
            ++sum;
        }
    }

    return sum;
}

int countMasCross(const VecVec &v) {
    int sum = 0;
    for (size_t y = 2; y < v.size(); ++y) {
        for (size_t x = 2; x < v.size(); ++x) {
            if (v.at(y - 2).at(x - 2) != 'M' || v.at(y).at(x - 2) != 'M') {
                continue;
            }
            if (v.at(y - 1).at(x - 1) != 'A') {
                continue;
            }
            if (v.at(y - 2).at(x) != 'S' || v.at(y).at(x) != 'S') {
                continue;
            }
            ++sum;
        }
    }
    return sum;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/04"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto content = VecVec{};

    for (std::string line; std::getline(file, line);) {
        content.push_back(line);
    }

    auto tmp = content;
    auto tmp45 = rotate45(content);

    auto sum1 = 0;
    auto sum2 = 0;
    for (int i = 0; i < 4; ++i) {
        tmp = rotate90(tmp);
        sum1 += count(tmp, "XMAS");
        sum2 += countMasCross(tmp);

        tmp45 = rotate90(tmp45);
        sum1 += count(tmp45, "X-M-A-S");
    }

    std::cout << std::format("Part 1: {}\n", sum1);
    std::cout << std::format("Part 2: {}\n", sum2);
}
