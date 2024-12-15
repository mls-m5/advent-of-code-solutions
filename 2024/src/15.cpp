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

void tryMoveRobot(Canvas<char> &map, Vec &robot, Vec dir) {
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

Canvas<char> scaleUp(const Canvas<char> &in) {
    auto ret = Canvas<char>{in.width * 2, in.height, '.'};

    for (auto y : iota_view{0uz, in.height}) {
        for (auto x : iota_view{0uz, in.width}) {
            auto a = '.';
            auto b = '.';

            switch (in.at(x, y)) {
            case '#':
                a = '#';
                b = '#';
                break;
            case 'O':
                a = '[';
                b = ']';
                break;
            case '@':
                a = '@';
                b = '.';
                break;
            }

            ret.at(x * 2, y) = a;
            ret.at(x * 2 + 1, y) = b;
        }
    }
    return ret;
}

bool tryMoveVertical(Canvas<char> &map,
                     int y,
                     const std::vector<bool> &cellsToCheck,
                     Vec dir) {
    auto newRow = std::vector(cellsToCheck.size(), false);
    for (auto x : iota_view{1uz, cellsToCheck.size() - 1}) {
        if (!cellsToCheck.at(x)) {
            continue;
        }
        auto c = map.at(Vec{static_cast<int>(x), y});

        switch (c) {
        case '#':
            return false;
        case '[':
            newRow.at(x) = true;
            newRow.at(x + 1) = true;
            break; // Det här hade jag missat
        case ']':
            newRow.at(x) = true;
            newRow.at(x - 1) = true;
            break;
        case '@':
            newRow.at(x) = true;
            break;
        }
    }

    auto hasBits = false;

    for (auto bit : newRow) {
        if (bit) {
            hasBits = true;
            break;
        }
    }

    // Nothing to check for and nathing to move
    if (!hasBits) {
        return true;
    }

    if (tryMoveVertical(map, y + dir.y, newRow, dir)) {
        for (auto x : iota_view(1uz, cellsToCheck.size() - 1)) {
            auto &cOld = map.at(Vec{static_cast<int>(x), y});

            // if (cOld == '[' || cOld == ']') {
            if (newRow.at(x)) {
                auto &cNew = map.at(Vec{static_cast<int>(x), y + dir.y});
                cNew = std::exchange(cOld, '.');
            }
        }

        return true;
    }

    return false;
}

int main(int argc, char *argv[]) {
    auto isTest = argc > 1;
    auto file = std::ifstream{std::string{"data/15"} +
                              (!isTest ? ".txt" : "-test.txt")};

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

    {
        auto map = Canvas<char>{content};
        auto robotPos = extractRobotPosition(map);

        std::cout << std::format("Robot {},{}\n", robotPos.x, robotPos.y);

        map.print();

        std::cout << moves << std::endl;

        for (auto c : moves) {
            tryMoveRobot(map, robotPos, charToCoord(c));
            // std::cout << "Move: " << c << "\n";
            // map.print();
        }

        int sum1 = 0;

        for (auto y : iota_view{0uz, map.height}) {
            for (auto x : iota_view{0uz, map.width}) {
                auto c = map.at(x, y);
                if (c == 'O') {
                    sum1 += y * 100 + x;
                }
            }
        }

        std::cout << std::format("Part 1: {}\n", sum1);
    }

    {
        auto map = Canvas<char>{content};
        auto largerMap = scaleUp(map);
        auto robotPos = extractRobotPosition(largerMap);

        largerMap.print();

        /// Test
        // for (std::string c; std::cin >> c;) {
        //     auto dir = charToCoord(c.front());
        //     if (dir.y == 0) {
        //         tryMoveRobot(largerMap, robotPos, dir);
        //     }
        //     else {
        //         auto cellsToCheck = std::vector(largerMap.width, false);
        //         cellsToCheck.at(robotPos.x) = true;
        //         if (tryMoveVertical(largerMap, robotPos.y, cellsToCheck,
        //         dir)) {
        //             robotPos = extractRobotPosition(largerMap);
        //         }
        //     }
        //     std::cout << "Move: " << c << "\n";
        //     largerMap.print();
        // }
        // end test

        for (auto c : moves) {
            auto dir = charToCoord(c);
            if (dir.y == 0) {
                tryMoveRobot(largerMap, robotPos, dir);
            }
            else {
                auto cellsToCheck = std::vector(largerMap.width, false);
                cellsToCheck.at(robotPos.x) = true;
                if (tryMoveVertical(largerMap, robotPos.y, cellsToCheck, dir)) {
                    robotPos = extractRobotPosition(largerMap);
                }
            }
            if (isTest) {
                std::cout << "Move: " << c << "\n";
                largerMap.print();
            }
        }

        int sum2 = 0;

        for (auto y : iota_view{0uz, largerMap.height}) {
            for (auto x : iota_view{0uz, largerMap.width}) {
                auto c = largerMap.at(x, y);

                if (c == '[') {
                    auto left = x;
                    auto right = largerMap.width - x - 2;
                    auto top = y;
                    auto bottom = largerMap.height - y - 1;

                    auto minX = std::min(left, right);
                    auto minY = std::min(top, bottom);
                    sum2 += y * 100 + x;
                }
            }
        }

        largerMap.print();

        std::cout << std::format("Part 1: {}\n", sum2);
    }
}
