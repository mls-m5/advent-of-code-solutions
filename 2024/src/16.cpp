#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <ranges>
#include <stdexcept>
#include <string>
#include <vector>

using std::ranges::iota_view;

struct Vec {
    int x = 0;
    int y = 0;

    Vec(int x, int y) : x{x}, y{y} {}

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

Vec extractPosition(Canvas<char> &canvas, char f, char replacement = '.') {
    for (auto y : iota_view(0uz, canvas.height)) {
        for (auto x : iota_view{0uz, canvas.width}) {
            auto &c = canvas.at(x, y);
            if (c == f) {
                if (replacement) {
                    c = replacement;
                }
                return {x, y};
            }
        }
    }

    throw std::runtime_error{"could not find robot"};
}

Vec rotateLeft(Vec v) {
    return {v.y, -v.x};
}

Vec rotateRight(Vec v) {
    return {-v.y, v.x};
}

void step(Canvas<char> &map,
          Canvas<int> &searchMap,
          std::vector<SearchPosition> &positions,
          const Vec endPosition,
          int &smallestEndScore,
          std::vector<SearchPosition> &bestPaths) {
    auto newPositions = std::vector<SearchPosition>{};

    auto explore = [&endPosition,
                    &newPositions,
                    &map,
                    &searchMap,
                    &smallestEndScore,
                    &bestPaths](Vec pos,
                                Vec dir,
                                int score,
                                std::vector<Vec> visitedTiles,
                                int grace) {
        auto np = pos + dir;
        auto &i = searchMap.at(np);
        auto c = map.at(np);
        if (score > i) {
            if (grace == 0) {
                return;
            }
            grace -= 1;
        }
        else {
            grace = 2;
        }
        if (c == '#') {
            return;
        }
        visitedTiles.push_back(np);

        auto newHead = SearchPosition{
            .pos = np,
            .dir = dir,
            .score = score,
            .grace = grace,
            .visitedTiles = visitedTiles,
        };

        if (np == endPosition) {
            smallestEndScore = std::min(smallestEndScore, score);

            bestPaths.push_back(newHead);

            return;
        }
        if (score < i) {
            i = score;
        }
        newPositions.push_back(newHead);
    };
    for (auto &head : positions) {
        {
            // Straight

            explore(head.pos,
                    head.dir,
                    head.score + 1,
                    head.visitedTiles,
                    head.grace);
            explore(head.pos,
                    rotateLeft(head.dir),
                    head.score + 1 + 1000,
                    head.visitedTiles,
                    head.grace);
            explore(head.pos,
                    rotateRight(head.dir),
                    head.score + 1 + 1000,
                    head.visitedTiles,
                    head.grace);
        }
    }

    positions = std::move(newPositions);
}

void drawSearchMap(const Canvas<char> &map,
                   const std::vector<SearchPosition> &positions) {
    auto newMap = Canvas<char>{map.width, map.height};
    newMap.data = map.data;
    for (auto &p : positions) {
        newMap.at(p.pos) = 'x';
    }
    newMap.print();
}

int main(int argc, char *argv[]) {
    auto isTest = argc > 1;
    auto file = std::ifstream{std::string{"data/16"} +
                              (!isTest ? ".txt" : "-test.txt")};

    auto content = std::vector<std::string>{};

    for (std::string line; std::getline(file, line);) {
        content.push_back(line);
    }

    auto map = Canvas<char>{content};
    map.print();
    auto scoreMap =
        Canvas<int>{map.width, map.height, std::numeric_limits<int>::max()};

    auto start = extractPosition(map, 'S');
    auto end = extractPosition(map, 'E');

    auto searchPositions = std::vector{SearchPosition{
        .pos = start,
        .dir = {1, 0},
        .visitedTiles = {start},
    }};

    int smallestEndScore = std::numeric_limits<int>::max();

    auto bestPaths = std::vector<SearchPosition>{};

    drawSearchMap(map, searchPositions);
    for (; !searchPositions.empty();) {
        step(map, scoreMap, searchPositions, end, smallestEndScore, bestPaths);
        if (isTest) {
            std::cout << std::format("current smallest: {}\n",
                                     smallestEndScore);
            drawSearchMap(map, searchPositions);
        }
    }

    auto it = std::remove_if(bestPaths.begin(),
                             bestPaths.end(),
                             [smallestEndScore](SearchPosition &p) {
                                 return p.score > smallestEndScore;
                             });

    bestPaths.erase(it, bestPaths.end());

    auto seatMap = Canvas<char>{map.width, map.height, ' '};
    seatMap.data = map.data;

    for (auto &p : bestPaths) {
        for (auto &v : p.visitedTiles) {
            seatMap.at(v) = 'O';
        }
    }

    seatMap.print();

    std::cout << std::format("Part 1: {}\n", smallestEndScore);

    auto sum2 = std::count(seatMap.data.begin(), seatMap.data.end(), 'O');

    std::cout << std::format("Part 2: {}\n", sum2);
}
