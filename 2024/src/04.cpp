#include <algorithm>
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
            std::cout << x << line.substr(0, x) << "!"
                      << line.substr(x, str.size()) << "!"
                      << line.substr(x + str.size()) << "\n";
            ++sum;
        }
    }

    std::cout << std::format("count {}\n", sum);
    std::cout << "\n";

    return sum;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/04"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto content = VecVec{};
    // auto straight = Vec3{};
    // auto staggered = Vec3{};

    for (std::string line; std::getline(file, line);) {
        // std::cout << line << std::endl;
        content.push_back(line);
    }

    auto tmp = content;
    auto tmp45 = rotate45(content);

    auto sum1 = 0;
    for (int i = 0; i < 4; ++i) {
        // straight.push_back(tmp);
        tmp = rotate90(tmp);
        print(tmp);
        sum1 += count(tmp, "XMAS");

        print(tmp45);
        tmp45 = rotate90(tmp45);
        sum1 += count(tmp45, "X-M-A-S");

        // auto flipped = tmp;
        // std::reverse(flipped.begin(), flipped.end());
        // sum1 += count(flipped, "XMAS");

        // flipped = tmp45;
        // std::reverse(flipped.begin(), flipped.end());
        // sum1 += count(flipped, "X-M-A-S");
    }

    // Flipped

    // print(content);
    // std::cout << count(content, "XMAS");

    // std::cout << "\n";

    // print(rotate90(content));
    // print(rotate45(content));

    std::cout << std::format("Part 1: {}\n", sum1);
}
