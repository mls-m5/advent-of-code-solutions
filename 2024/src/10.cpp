#include <algorithm>
#include <format>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <set>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using std::ranges::iota_view;

struct Coord {
    int x = 0;
    int y = 0;

    Coord operator+(const Coord &other) {
        return {
            x + other.x,
            y + other.y,
        };
    }

    bool operator==(const Coord &other) const {
        return x == other.x && y == other.y;
    }
};

struct Head {
    Coord coord;
    int index = 0;
};

using HeadListT = std::vector<Head>;

struct Canvas {
    std::vector<int> data;
    size_t width = 0;
    size_t height = 0;

    Canvas(size_t width, size_t height, int def = -1)
        : width{width}, height{height} {
        data.resize(width * height, def);
    }

    Canvas(std::vector<std::string> content)
        : width{content.front().size()}, height{content.size()} {
        for (auto &line : content) {
            for (auto c : line) {
                data.push_back(c == '.' ? -1 : (c - '0'));
            }
        }
    }

    auto &at(int x, int y) {
        return data.at(y * width + x);
    }

    auto at(int x, int y) const {
        return data.at(y * width + x);
    }

    auto &at(Coord c) {
        return at(c.x, c.y);
    }

    auto at(Coord c) const {
        return at(c.x, c.y);
    }

    void print() const {
        for (auto y : iota_view(0uz, height)) {
            for (auto x : iota_view{0uz, width}) {
                std::cout << std::setw(2);
                auto value = at(x, y);
                if (value == -1) {
                    std::cout << '.';
                }
                else {
                    std::cout << value;
                }
            }
            std::cout << "\n";
        }

        std::cout << "\n";
    }

    bool isInside(Coord c) const {
        return c.x >= 0 && c.y >= 0 && c.x < width && c.y < height;
    }
};

HeadListT findNumber(const Canvas &c, int number) {
    auto ret = HeadListT{};

    int index = 0;

    for (auto y : iota_view(0uz, c.height)) {
        for (auto x : iota_view(0uz, c.width)) {
            auto value = c.at(x, y);
            if (value == number) {
                ret.push_back(
                    {{static_cast<int>(x), static_cast<int>(y)}, index});
                ++index;
            }
        }
    }

    return ret;
}

void getNext(const Canvas &heightMap,
             const HeadListT &heads,
             Canvas &resultMap,
             HeadListT &resultCoords) {
    resultCoords.clear();

    auto addCoord = [&resultCoords, &heightMap, &resultMap](Coord old,
                                                            Head ncoord) {
        if (heightMap.isInside(ncoord.coord)) {
            if (heightMap.at(old) + 1 == heightMap.at(ncoord.coord)) {
                resultCoords.push_back(ncoord);
                ++resultMap.at(ncoord.coord);
            }
        }
    };

    for (auto head : heads) {
        addCoord(head.coord, {head.coord + Coord{1, 0}, head.index});
        addCoord(head.coord, {head.coord + Coord{-1, 0}, head.index});
        addCoord(head.coord, {head.coord + Coord{0, 1}, head.index});
        addCoord(head.coord, {head.coord + Coord{0, -1}, head.index});
    }
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/10"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto isTest = argc > 1;

    auto content = std::vector<std::string>{};
    for (std::string line; std::getline(file, line);) {
        content.push_back(line);
    }

    const auto heightMap = Canvas{content};

    heightMap.print();

    auto resultMap = Canvas{heightMap.width, heightMap.height, 0};
    auto starts = findNumber(heightMap, 0);

    auto stops = findNumber(heightMap, 9);
    auto count1 = std::vector<std::set<int>>(stops.size());
    auto count2 = std::vector<int>(stops.size());

    for (auto startingPosition : starts) {
        auto current = decltype(starts){startingPosition};
        auto next = decltype(starts){};
        for (size_t i = 0; i < 9; ++i) {
            getNext(heightMap, current, resultMap, next);
            std::swap(current, next);

            if (isTest) {
                resultMap.print();
            }
        }

        for (auto i : iota_view{0uz, count1.size()}) {
            for (auto pos : current) {
                if (stops.at(i).coord == pos.coord) {
                    count1.at(i).insert(startingPosition.index);
                    ++count2.at(i);
                }
            }
        }
    }

    int sum1 = 0;

    for (auto c : count1) {
        sum1 += c.size();
        std::cout << c.size() << " ";
    }

    int sum2 = 0;

    for (auto c : count2) {
        sum2 += c;
        std::cout << c << " ";
    }

    std::cout << "\n";

    std::cout << std::format("Part 1: {}\n", sum1);
    std::cout << std::format("Part 2: {}\n", sum2);
}
