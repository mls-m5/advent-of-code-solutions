#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <ranges>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::ranges::iota_view;

struct Vec {
    int x = 0;
    int y = 0;

    Vec(int x, int y) : x{x}, y{y} {}
    Vec(size_t x, size_t y) : x{static_cast<int>(x)}, y{static_cast<int>(y)} {}

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

Vec extractRobotPosition(const Canvas<char> &canvas) {
    for (auto y : iota_view(0uz, canvas.height)) {
        for (auto x : iota_view{0uz, canvas.width}) {
            auto &c = canvas.at(x, y);
            if (c == '@') {
                return {x, y};
            }
        }
    }

    throw std::runtime_error{"could not find robot"};
}

Vec charToCoord(char c) {
    switch (c) {
    case '^':
        return {0, -1};
    case '>':
        return {1, 0};
    case 'v':
        return {0, 1};
    case '<':
        return {-1, 0};
    }
    throw std::runtime_error{"invalid direction"};
}

void tryMoveRobot(Canvas<char> &map, Vec &robot, char directionC) {
    auto dir = charToCoord(directionC);

    bool canMove = false;
    auto pos = robot;
    for (; map.isInside(pos); pos.x += dir.x, pos.y += dir.y) {
        if (map.at(pos) == '#') {
            return;
        }
        if (map.at(pos) == '.') {
            canMove = true;
            break;
        }
    }

    if (!canMove) {
        return;
    }

    for (;;) {
        auto np = Vec{pos.x - dir.x, pos.y - dir.y};

        if (pos == robot) {
            break;
        }

        map.at(pos) = std::exchange(map.at(np), '.');

        pos = np;
    }

    robot.x += dir.x;
    robot.y += dir.y;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/15"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto content = std::vector<std::string>{};

    for (std::string line; std::getline(file, line);) {
        if (line.empty()) {
            break;
        }

        content.push_back(line);
    }

    auto moves = std::string{};
    for (std::string line; std::getline(file, line);) {
        moves += line;
    }

    auto map = Canvas<char>{content};

    auto robotPos = extractRobotPosition(map);

    std::cout << std::format("Robot {},{}\n", robotPos.x, robotPos.y);

    map.print();

    std::cout << moves << std::endl;

    for (auto c : moves) {
        tryMoveRobot(map, robotPos, c);
        // std::cout << "Move: " << c << "\n";
        // map.print();
    }

    int sum1 = 0;

    for (auto y : iota_view{0uz, map.width}) {
        for (auto x : iota_view{0uz, map.width}) {
            auto c = map.at(x, y);
            if (c == 'O') {
                sum1 += y * 100 + x;
            }
        }
    }

    std::cout << std::format("Part 1: {}\n", sum1);
}
