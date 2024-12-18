#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <memory>
#include <optional>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

using std::ranges::iota_view;

using Int = int;

struct Vec {
    Int x = 0;
    Int y = 0;

    Vec(Int x, Int y) : x{x}, y{y} {}

    Vec operator+(const Vec &other) {
        return {
            x + other.x,
            y + other.y,
        };
    }

    bool operator==(const Vec &other) const {
        return x == other.x && y == other.y;
    }
};

template <typename T>
struct Canvas {
    std::vector<T> data;
    size_t width = 0;
    size_t height = 0;

    Canvas() = default;

    Canvas(const Canvas &) = default;
    Canvas(Canvas &&) = default;
    Canvas &operator=(const Canvas &) = default;
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

    auto &at(Vec c) {
        return at(c.x, c.y);
    }

    auto at(Vec c) const {
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

    bool isInside(Vec c) const {
        return c.x >= 0 && c.y >= 0 && c.x < width && c.y < height;
    }
};

struct SearchPosition {
    Vec pos;
    Vec dir;
    int score = 0;
    int grace = 2;

    std::vector<Vec> visitedTiles;
};

Vec coordFromLine(std::string line) {
    auto f = line.find(',');
    return {
        std::stoi(line.substr(0, f)),
        std::stoi(line.substr(f + 1)),
    };
}

struct SearchHead {
    std::shared_ptr<SearchHead> prev = nullptr;
    Vec position;
};

int breadthFirst(Canvas<char> &map,
                 Canvas<Int> &distanceMap,
                 Vec position,
                 Vec target) {

    auto searchHeads = std::list<std::shared_ptr<SearchHead>>{};

    auto explore = [&](std::shared_ptr<SearchHead> prev, Vec dir) -> bool {
        auto p = prev->position + dir;
        if (p == target) {
            return true;
        }

        if (!map.isInside(p)) {
            return false;
        }

        auto &c = map.at(p);

        if (c == '#') {
            return false;
        }

        Int score = distanceMap.at(prev->position) + 1;

        if (c == '-') {
            // if (distanceMap.at(p) < score) {
            return false;
            // }
        }

        distanceMap.at(p) = score;

        c = '-';

        searchHeads.push_back(std::make_shared<SearchHead>(SearchHead{
            .prev = prev,
            .position = prev->position + dir,
        }));

        return false;
    };

    map.at(position) = '-';

    auto count = [&](std::shared_ptr<SearchHead> head) {
        int sum = 1;
        for (auto h = head.get(); h->prev; h = h->prev.get()) {
            ++sum;
            map.at(h->position) = 'O';
        }
        map.at(position) = 'O';
        return sum;
    };

    searchHeads.push_back(std::make_shared<SearchHead>(SearchHead{
        .position = position,
    }));

    for (; !searchHeads.empty();) {
        if (!searchHeads.size()) {
            break;
        }
        auto head = searchHeads.front();
        // searchHeads.pop_back();
        searchHeads.pop_front();
        if (auto f = explore(head, {0, -1})) {
            return count(head);
        }
        if (auto f = explore(head, {0, 1})) {
            return count(head);
        }
        if (auto f = explore(head, {-1, 0})) {
            return count(head);
        }
        if (auto f = explore(head, {1, 0})) {
            return count(head);
        }

        // map.print();
    }

    throw std::runtime_error{"Could not find path"};
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/18"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto isTest = argc > 1;

    size_t size = 0;
    file >> size;

    auto dropped = std::vector<Vec>{};

    std::cout << "size: " << size << std::endl;

    auto map = Canvas<char>{size, size, '.'};
    auto originalMap = map;

    for (std::string line; file >> line;) {
        dropped.push_back(coordFromLine(line));
        // std::cout << line << "\n";
        // std::cout << dropped.back().x << "," << dropped.back().y << "\n";
    }

    auto start = Vec{0, 0};
    auto end = Vec{size - 1, size - 1};

    for (auto i : iota_view{0uz, isTest ? 12uz : 1024uz}) {
        map.at(dropped.at(i)) = '#';
    }

    map.print();

    auto distanceMap = Canvas<int>{size, size, std::numeric_limits<Int>::max()};

    auto count = breadthFirst(map, distanceMap, start, end);
    map.print();

    // 158 is to low
    std::cout << std::format("Part 1: {}\n", count);

    std::cout << std::count(map.data.begin(), map.data.end(), 'O') << std::endl;

    std::cout << std::endl;
}
