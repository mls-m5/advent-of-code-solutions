#include <algorithm>
#include <charconv>
#include <format>
#include <fstream>
#include <iostream>
#include <ranges>
#include <string>
#include <vector>

using std::ranges::iota_view;

using Int = int;

Int width = 11;
Int height = 7;

struct Vec {
    Int x = {};
    Int y = {};

    Vec operator+(const Vec &other) const {
        return {x + other.x, y + other.y};
    }

    Vec operator*(Int value) const {
        return {x * value, y * value};
    }
};

Int mod(Int value, Int div) {
    return ((value % div) + div) % div;
}

struct Robot {
    Vec pos;
    Vec vel;

    Vec positionAt(Int time) const {
        auto p = pos + vel * time;
        p.x = mod(p.x, width);
        p.y = mod(p.y, height);

        return p;
    }
};

Vec getCoord(std::string_view str) {
    str.remove_prefix(str.find('=') + 1);
    auto f = str.find(',');
    auto a = str.substr(0, f);
    auto b = str.substr(f + 1);

    auto ret = Vec{};

    std::from_chars(a.begin(), a.end(), ret.x);
    std::from_chars(b.begin(), b.end(), ret.y);

    return ret;
}

Int countArea(const std::vector<Robot> &robots,
              Int step,
              Int minX,
              Int minY,
              Int maxX,
              Int maxY) {
    auto sum =
        std::count_if(robots.begin(), robots.end(), [&](const Robot &robot) {
            auto p = robot.positionAt(step);
            return p.x >= minX && p.y >= minY && p.x < maxX && p.y < maxY;
        });

    return sum;
}

void drawRobots(const std::vector<Robot> &robots, Int step) {
    for (auto y : iota_view(0, height)) {
        for (auto x : iota_view(0, width)) {
            int sum = 0;
            for (auto &robot : robots) {
                auto p = robot.positionAt(step);
                sum += (p.x == x && p.y == y);
            }
            if (sum > 0) {
                std::cout << sum;
            }
            else {
                std::cout << ".";
            }
        }
        std::cout << "\n";
    }
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/14"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto robots = std::vector<Robot>{};

    file >> width;
    file >> height;

    for (std::string coord, vel; file >> coord, std::getline(file, vel);) {
        std::cout << coord << "--" << vel << "\n";
        auto robot = Robot{getCoord(coord), getCoord(vel)};

        std::cout << std::format("{},{}-{},{}\n",
                                 robot.pos.x,
                                 robot.pos.y,
                                 robot.vel.x,
                                 robot.vel.y);

        robots.push_back(robot);
    }

    auto halfW = width / 2;
    auto halfH = height / 2;

    auto a = countArea(robots, 100, 0, 0, halfW, halfH);
    auto b = countArea(robots, 100, halfW + 1, 0, width + 1, halfH);
    auto c = countArea(robots, 100, 0, halfH + 1, halfW, height + 1);
    auto d =
        countArea(robots, 100, halfW + 1, halfH + 1, width + 1, height + 1);

    drawRobots(robots, 100);

    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    std::cout << d << std::endl;

    std::cout << std::format("Part 1: {}\n", a * b * c * d);
}
