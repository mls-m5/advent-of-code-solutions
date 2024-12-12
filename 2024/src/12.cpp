#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <istream>
#include <map>
#include <memory>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using std::ranges::iota_view;

struct Coord {
    int x = 0;
    int y = 0;

    Coord(int x, int y) : x{x}, y{y} {}
    Coord(size_t x, size_t y)
        : x{static_cast<int>(x)}, y{static_cast<int>(y)} {}

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

template <typename T>
struct Canvas {
    std::vector<T> data;
    size_t width = 0;
    size_t height = 0;

    Canvas(size_t width, size_t height, T def = {})
        : width{width}, height{height} {
        data.resize(width * height, def);
    }

    Canvas(std::vector<std::string> content)
        : width{content.front().size()}, height{content.size()} {
        for (auto &line : content) {
            for (auto c : line) {
                data.push_back(c);
            }
        }
    }

    auto &at(int x, int y) {
        return data.at(y * width + x);
    }

    const auto &at(int x, int y) const {
        return data.at(y * width + x);
    }

    auto &at(Coord c) {
        return at(c.x, c.y);
    }

    auto at(Coord c) const {
        return at(c.x, c.y);
    }

    template <typename PrintT = T>
    void print(int w = 1) const {
        for (auto y : iota_view(0uz, height)) {
            for (auto x : iota_view{0uz, width}) {
                std::cout << std::setw(w);
                auto value = at(x, y);

                std::cout << static_cast<PrintT>(value);
            }
            std::cout << "\n";
        }

        std::cout << "\n";
    }

    using FT = std::function<void(const T &)>;
    void printCustom(const FT &f) const {
        for (auto y : iota_view(0uz, height)) {
            for (auto x : iota_view{0uz, width}) {
                const auto &value = at(x, y);

                f(value);
            }
            std::cout << "\n";
        }

        std::cout << "\n";
    }

    bool isInside(Coord c) const {
        return c.x >= 0 && c.y >= 0 && c.x < width && c.y < height;
    }
};

struct Cell {
    std::shared_ptr<int> region;
    char name;
};

Canvas<Cell> findSeparateRegions(const Canvas<char> &c) {
    auto actualRegions = Canvas<Cell>{c.width, c.height};
    int num = 0;
    for (auto y : iota_view(0uz, c.height)) {
        for (auto x : iota_view(0uz, c.width)) {
            actualRegions.at(x, y) = Cell{
                .region = std::make_shared<int>(num),
                .name = c.at(Coord{x, y}),
            };
            ++num;
        }
    }

    return actualRegions;
}

auto printF = [](const Cell &data) {
    // std::cout << std::setw(5) << std::format("{}:{}", data.name,
    // *data.region);
    std::cout << std::setw(5) << std::format("{}", *data.region);
};

void group(Canvas<Cell> &map) {
    auto regions = std::map<int, std::vector<Cell *>>{};

    for (auto &d : map.data) {
        regions[*d.region].push_back(&d);
    }

    auto checkNeighbour =
        [&map, &regions](Cell &a, size_t x, size_t y, bool &didChange) {
            if (!map.isInside({x, y})) {
                return;
            }
            auto &other = map.at(x, y);
            if (a.name == other.name && a.region != other.region) {
                auto &region = regions.at(*other.region);
                for (auto &otherCell : region) {
                    otherCell->region = a.region;
                    regions[*a.region].push_back(otherCell);
                }
                region.clear();
                didChange = true;
            }
        };

    for (bool didChange = true; didChange;) {
        didChange = false;
        for (auto y : iota_view(0uz, map.height)) {
            for (auto x : iota_view(0uz, map.width)) {
                checkNeighbour(map.at(x, y), x - 1, y, didChange);
                checkNeighbour(map.at(x, y), x, y - 1, didChange);
                checkNeighbour(map.at(x, y), x + 1, y, didChange);
                checkNeighbour(map.at(x, y), x, y + 1, didChange);
            }
        }

        // map.printCustom(printF);
    }
}

struct ResultT {
    int area = 0;
    int perimiter = 0;
};

auto calculateStuff(const Canvas<Cell> &map) {
    auto resultMap = std::map<int, ResultT>{};

    auto checkNeighbour = [&map,
                           &resultMap](const Cell &a, size_t x, size_t y) {
        auto &res = resultMap[*a.region];
        if (!map.isInside({x, y})) {
            ++res.perimiter;
            return;
        }
        auto &other = map.at(x, y);
        if (*a.region != *other.region) {
            ++res.perimiter;
        }
    };

    for (auto y : iota_view(0uz, map.height)) {
        for (auto x : iota_view(0uz, map.width)) {
            checkNeighbour(map.at(x, y), x - 1, y);
            checkNeighbour(map.at(x, y), x + 1, y);
            checkNeighbour(map.at(x, y), x, y + 1);
            checkNeighbour(map.at(x, y), x, y - 1);
            ++resultMap[*map.at(x, y).region].area;
        }
    }

    return resultMap;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/12"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto content = std::vector<std::string>{};
    for (std::string line; std::getline(file, line);) {
        content.push_back(line);
    }

    auto inputMap = Canvas<char>{content};

    inputMap.print<char>(2);

    auto regionMap = findSeparateRegions(inputMap);

    regionMap.printCustom(printF);

    group(regionMap);

    regionMap.printCustom(printF);

    auto result = calculateStuff(regionMap);

    int sum1 = 0;

    for (auto &it : result) {
        std::cout << std::format("num: {}. area: {}, perimiter: {}",
                                 it.first,
                                 it.second.area,
                                 it.second.perimiter)
                  << "\n";

        sum1 += it.second.area * it.second.perimiter;
    }

    std::cout << std::format("Part 1: {}\n", sum1);
}
