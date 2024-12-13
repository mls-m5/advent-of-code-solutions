#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <ranges>
#include <string>

using std::ranges::iota_view;

struct Coord {
    int x = 0;
    int y = 0;

    auto operator<=>(const Coord &other) const = default;
};

struct Machine {
    Coord a;
    Coord b;
    Coord prize;
};

int getLeastCost(const Machine &machine) {
    int leastCost = std::numeric_limits<int>::max();

    for (auto a : iota_view{0, 100}) {
        for (auto b : iota_view{0, 100}) {
            auto result = Coord{
                .x = machine.a.x * a + machine.b.x * b,
                .y = machine.a.y * a + machine.b.y * b,
            };

            if (result == machine.prize) {
                auto cost = a * 3 + b;
                leastCost = std::min(cost, leastCost);
            }
        }
    }

    return leastCost;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/13"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    int sum1 = 0;

    // The ugliest parsing in the world
    for (std::string aLine, bLine, prizeLine, _;;) {
        std::getline(file, aLine), std::getline(file, bLine),
            std::getline(file, prizeLine), std::getline(file, _);

        auto f1 = prizeLine.find("=");
        auto f2 = prizeLine.find("=", f1 + 1);

        // std::cout << prizeLine.substr(f1 + 1, f2 - f1 - 4) << "\n";
        // std::cout << prizeLine.substr(f2 + 1) << std::endl;

        auto machine =
            Machine{.a = {std::stoi(aLine.substr(12, 15)),
                          std::stoi(aLine.substr(18, 18 + 3))},
                    .b = {std::stoi(bLine.substr(12, 15)),
                          std::stoi(bLine.substr(18, 18 + 3))},
                    .prize = {std::stoi(prizeLine.substr(f1 + 1, f2 - f1 - 4)),
                              std::stoi(prizeLine.substr(f2 + 1))}

            };

        std::cout << aLine << std::endl;
        std::cout << std::format(
            "Button A: {}, {}\n", machine.a.x, machine.a.y);
        std::cout << std::format(
            "Button B: {}, {}\n", machine.b.x, machine.b.y);
        std::cout << std::format(
            "Price: {}, {}\n", machine.prize.x, machine.prize.y);

        auto cost = getLeastCost(machine);
        if (cost != std::numeric_limits<int>::max()) {
            sum1 += cost;
        }

        if (!file) {
            break;
        }
    }

    std::cout << std::format("Part 1: {}\n", sum1);
}
