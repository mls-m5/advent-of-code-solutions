#include <algorithm>
#include <format>
#include <fstream>
#include <iostream>
#include <map>
#include <ranges>
#include <string>
#include <string_view>
#include <vector>

using std::ranges::iota_view;

struct Antenna {
    int x = 0;
    int y = 0;
};

struct Canvas {
    std::string data;
    size_t width = 0;
    size_t height = 0;

    Canvas(size_t width, size_t height) : width{width}, height{height} {
        data.resize(width * height, '.');
    }

    Canvas(std::vector<std::string> content)
        : Canvas{content.front().size(), content.size()} {
        data.clear();
        for (auto &line : content) {
            data += line;
        }
    }

    char &at(int x, int y) {
        return data.at(y * width + x);
    }

    char at(int x, int y) const {
        return data.at(y * width + x);
    }

    void print() {
        for (auto y : iota_view(0uz, height)) {
            std::cout << std::string_view{data}.substr(y * width, width)
                      << "\n";
        }

        std::cout << "\n";
    }

    std::map<char, std::vector<Antenna>> uniqueCharacters() const {
        auto ret = std::map<char, std::vector<Antenna>>{};

        for (int y : iota_view(0uz, width)) {
            for (int x : iota_view(0uz, width)) {
                auto c = at(x, y);
                if (c != '.') {
                    ret[c].push_back({x, y});
                }
            }
        }

        return ret;
    }

    bool isInside(int x, int y) const {
        return x >= 0 && y >= 0 && x < width && y < height;
    }

    void combine(const Antenna &a, const Antenna &b) {
        auto nx = b.x * 2 - a.x;
        auto ny = b.y * 2 - a.y;

        if (isInside(nx, ny)) {
            at(nx, ny) = '#';
        }
    }

    void placeNodes(const std::vector<Antenna> &antennas) {
        for (auto i : iota_view(0uz, antennas.size())) {
            for (auto j : iota_view(i + 1, antennas.size())) {
                combine(antennas.at(i), antennas.at(j));
                combine(antennas.at(j), antennas.at(i));
            }
        }
    }

    void combine2(const Antenna &a, const Antenna &b) {
        auto dx = b.x - a.x;
        auto dy = b.y - a.y;

        for (int nx = b.x, ny = b.y; isInside(nx, ny); nx += dx, ny += dy) {
            at(nx, ny) = '#';
        }
    }

    void placeAllNodes(const std::vector<Antenna> &antennas) {
        for (auto i : iota_view(0uz, antennas.size())) {
            for (auto j : iota_view(i + 1, antennas.size())) {
                combine2(antennas.at(i), antennas.at(j));
                combine2(antennas.at(j), antennas.at(i));
            }
        }
    }

    int count() {
        return width * height - std::count(data.begin(), data.end(), '.');
    }
};

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/08"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto content = std::vector<std::string>{};

    for (std::string line; std::getline(file, line);) {
        content.push_back(line);
    }

    auto inputMap = Canvas{content};
    auto resultMap = Canvas{inputMap.width, inputMap.height};
    auto resultMap2 = Canvas{inputMap.width, inputMap.height};

    inputMap.print();
    auto antennas = inputMap.uniqueCharacters();

    for (auto &antenna : antennas) {
        std::cout << std::format(
            "character {}: {}\n", antenna.first, antenna.second.size());

        resultMap.placeNodes(antenna.second);
        resultMap2.placeAllNodes(antenna.second);
    }
    resultMap.print();
    resultMap2.print();

    std::cout << std::format("Part 1: {}\n", resultMap.count());
    std::cout << std::format("Part 2: {}\n", resultMap2.count());
}
