#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <ranges>
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

    Canvas() = default;

    Canvas(const Canvas &) = delete;
    Canvas(Canvas &&) = default;
    Canvas &operator=(const Canvas &) = delete;
    Canvas &operator=(Canvas &&) = default;

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
        std::cout.flush();
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

struct Region {
    int r = -1;
    char name = 0;
};

struct Cell {
    std::shared_ptr<Region> region;
    // char name;
};

Canvas<Cell> findSeparateRegions(const Canvas<char> &c) {
    auto actualRegions = Canvas<Cell>{c.width, c.height};
    int num = 0;
    for (auto y : iota_view(0uz, c.height)) {
        for (auto x : iota_view(0uz, c.width)) {
            actualRegions.at(x, y) = Cell{
                .region = std::make_shared<Region>(num, c.at(Coord{x, y})),
                // .name = c.at(Coord{x, y}),
            };
            ++num;
        }
    }

    return actualRegions;
}

auto printF = [](const Cell &data) {
    // std::cout << std::setw(5) << std::format("{}:{}", data.name,
    // *data.region);
    std::cout << std::setw(5)
              << std::format("{}:{}", data.region->name, data.region->r);
};

void group(Canvas<Cell> &map) {
    auto regions = std::map<int, std::vector<Cell *>>{};

    for (auto &d : map.data) {
        regions[d.region->r].push_back(&d);
    }

    auto checkNeighbour = [&map, &regions](
                              Cell &a, size_t x, size_t y, bool &didChange) {
        if (!map.isInside({x, y})) {
            return;
        }
        auto &other = map.at(x, y);
        if (a.region->name == other.region->name && a.region != other.region) {
            auto &region = regions.at(other.region->r);
            for (auto &otherCell : region) {
                otherCell->region = a.region;
                regions[a.region->r].push_back(otherCell);
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
    char name = 0;
};

auto calculateStuff(const Canvas<Cell> &map) {
    auto resultMap = std::map<int, ResultT>{};

    auto checkNeighbour = [&map,
                           &resultMap](const Cell &a, size_t x, size_t y) {
        auto &res = resultMap[a.region->r];
        if (!map.isInside({x, y})) {
            ++res.perimiter;
            return;
        }
        auto &other = map.at(x, y);
        if (a.region->r != other.region->r) {
            ++res.perimiter;
        }
    };

    for (auto y : iota_view(0uz, map.height)) {
        for (auto x : iota_view(0uz, map.width)) {
            checkNeighbour(map.at(x, y), x - 1, y);
            checkNeighbour(map.at(x, y), x + 1, y);
            checkNeighbour(map.at(x, y), x, y + 1);
            checkNeighbour(map.at(x, y), x, y - 1);
            auto &res = resultMap[map.at(x, y).region->r];
            ++res.area;
            res.name = map.at(x, y).region->name;
        }
    }

    return resultMap;
}

struct BulkRegion {
    BulkRegion(const BulkRegion &) = delete;
    BulkRegion(BulkRegion &&) = default;
    BulkRegion &operator=(const BulkRegion &) = delete;
    BulkRegion &operator=(BulkRegion &&) = default;
    BulkRegion() = default;
    BulkRegion(size_t w, size_t h) : w{w}, h{h} {}

    size_t w = 0;
    size_t h = 0;

    Canvas<char> left{w, h};
    Canvas<char> right{w, h};
    Canvas<char> up{w, h};
    Canvas<char> down{w, h};
};

void removeVertical(Canvas<char> &map) {
    for (auto x : iota_view(0uz, map.width)) {
        for (auto y : iota_view(1uz, map.height)) {
            if (map.at(x, y - 1) && map.at(x, y)) {
                map.at(x, y - 1) = 0;
            }
        }
    }
}

void removeHorizontal(Canvas<char> &map) {
    for (auto y : iota_view(0uz, map.height)) {
        for (auto x : iota_view(1uz, map.width)) {
            if (map.at(x - 1, y) && map.at(x, y)) {
                map.at(x - 1, y) = 0;
            }
        }
    }
}

auto calculateBulk(const Canvas<Cell> &map) {
    auto regions = std::map<int, BulkRegion>{};

    auto regionCounts = std::map<int, int>{};

    auto checkNeighbour = [&map, &regions](const Cell &a,
                                           size_t oX,
                                           size_t oY,
                                           size_t x,
                                           size_t y,
                                           Canvas<char> &result) {
        if (!map.isInside({x, y})) {
            result.at(oX, oY) = true;
            return;
        }
        auto &other = map.at(x, y);
        if (a.region->r != other.region->r) {
            result.at(oX, oY) = true;
        }
    };

    for (auto y : iota_view(0uz, map.height)) {
        for (auto x : iota_view(0uz, map.width)) {
            auto &m = map.at(x, y);
            auto f = regions.find(m.region->r);
            if (f == regions.end()) {
                regions[m.region->r] = BulkRegion{map.width, map.height};
                f = regions.find(m.region->r);
            }
            auto &region = f->second;
            checkNeighbour(m, x, y, x - 1, y, region.left);
            checkNeighbour(m, x, y, x + 1, y, region.right);
            checkNeighbour(m, x, y, x, y - 1, region.up);
            checkNeighbour(m, x, y, x, y + 1, region.down);
        }
    }

    auto add = [](int &res, auto &map) {
        for (auto &it : map.data) {
            res += it;
        }
    };

    for (auto &it : regions) {
        int sum = 0;
        // std::cout << "---\n";
        // it.second.up.print<int>();
        // it.second.down.print<int>();
        // it.second.left.print<int>();
        // it.second.right.print<int>();
        removeVertical(it.second.left);
        removeVertical(it.second.right);
        removeHorizontal(it.second.up);
        removeHorizontal(it.second.down);

        add(sum, it.second.left);
        add(sum, it.second.right);
        add(sum, it.second.up);
        add(sum, it.second.down);

        regionCounts[it.first] = sum;

        // it.second.up.print<int>();
        // it.second.down.print<int>();
        // it.second.left.print<int>();
        // it.second.right.print<int>();
    }

    return regionCounts;
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
    auto count2 = calculateBulk(regionMap);

    int sum1 = 0;
    int sum2 = 0;

    for (auto &it : result) {
        sum1 += it.second.area * it.second.perimiter;
        sum2 += it.second.area * count2.at(it.first);

        std::cout << std::format(
            "name: {}. area: {}, perimiter{}, sides: {}, sum: {}\n",
            it.second.name,
            it.second.area,
            it.second.perimiter,
            count2.at(it.first),
            it.second.area * count2.at(it.first));
    }

    std::cout << std::format("Part 1: {}\n", sum1);
    std::cout << std::format("Part 2: {}\n", sum2);
}
