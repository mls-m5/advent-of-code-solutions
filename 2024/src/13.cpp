#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <format>
#include <fstream>
#include <iostream>
#include <limits>
#include <ostream>
#include <ranges>
#include <stdexcept>
#include <string>

// 57202139832621 is to low
// 74638646129555 is to low
// 161137675724695 is not right

using std::ranges::iota_view;

constexpr auto offset = 10000000000000l;

struct Coord {
    __int128 x = 0;
    __int128 y = 0;

    auto operator<=>(const Coord &other) const = default;
};

struct Machine {
    Coord a;
    Coord b;
    Coord prize;
};

std::ostream &operator<<(std::ostream &stream, __int128 value) {
    auto str = std::string{};
    for (; value; value /= 10) {
        str.push_back('0' + static_cast<char>(value % 10));
    }

    std::ranges::reverse(str);
    return stream << str;
}

__int128 getLeastCost(const Machine &machine,
                      Coord startingPosition,
                      int maxPresses = 100) {
    __int128 leastCost = std::numeric_limits<__int128>::max();

    for (auto a : iota_view{0, maxPresses}) {
        for (auto b : iota_view{0, maxPresses}) {
            auto result = Coord{
                .x = startingPosition.x + machine.a.x * a + machine.b.x * b,
                .y = startingPosition.y + machine.a.y * a + machine.b.y * b,
            };

            if (result == machine.prize) {

                std::cout << std::format("found: {}, {}\n", a, b);
                auto cost = a * 3 + b;
                leastCost = std::min<__int128>(cost, leastCost);
            }
        }
    }

    if (leastCost < 0) {
        throw std::runtime_error{"this should not happen"};
    }

    return leastCost;
}

// __int128 getLeastCost2(const Machine &machine) {
//     auto aCount = 0ul;
//     auto bCount = 0ul;

//     auto currentPosition = Coord{};

//     bool isAFlattest = false;
//     if (machine.a.x < machine.a.y) {
//         if (machine.b.x < machine.b.y) {
//             return std::numeric_limits<__int128>::max();
//         }
//         isAFlattest = true;
//     }
//     else {
//         if (machine.b.x > machine.b.y) {
//             return std::numeric_limits<__int128>::max();
//         }
//     }

//     auto crudeFind =
//         [&currentPosition, &machine, &aCount, &bCount, isAFlattest](
//             __int128 step) {
//             // Just get near to the starting point
//             for (; currentPosition.x + 10000 * step < offset &&
//                    currentPosition.y + 10000 * step < offset;) {

//                 auto quota = static_cast<double>(currentPosition.x) /
//                              static_cast<double>(currentPosition.y);
//                 if ((currentPosition.x < currentPosition.y) != isAFlattest) {
//                     currentPosition.x += machine.a.x * step;
//                     currentPosition.y += machine.a.y * step;
//                     aCount += step;
//                 }
//                 else {
//                     currentPosition.x += machine.b.x * step;
//                     currentPosition.y += machine.b.y * step;
//                     bCount += 1 * step;
//                 }
//             }
//         };

//     crudeFind(100000);
//     crudeFind(10000);
//     crudeFind(1000);
//     crudeFind(100);
//     crudeFind(10);
//     crudeFind(1);

//     auto otherCost = getLeastCost(machine, currentPosition, 1000);

//     if (otherCost == std::numeric_limits<__int128>::max()) {
//         return otherCost;
//     }
//     auto thisCoost = aCount * 2 + bCount;
//     auto ret = thisCoost + otherCost;

//     if (ret < 0) {
//         throw std::runtime_error{"this should not happen"};
//     }
//     return ret;
// }

// struct Vec {
//     double x;
//     double y;

//     double operator*(const Vec &other) const {
//         return x * other.x + y * other.y;
//     }

//     friend Vec operator+(const Vec &a, const Vec &b) {
//         return {a.x + b.x, a.y + b.y};
//     }
// };

// Vec toVec(const Coord &coord) {
//     return Vec{static_cast<double>(coord.x), static_cast<double>(coord.y)};
// }

// std::optional<Vec> findIntersection(const Machine &machine) {
//     // Convert all Coord to Vec for double precision computation
//     Vec aVec = toVec(machine.a);
//     Vec bVec = toVec(machine.b);
//     Vec prizeVec = toVec(machine.prize);

//     // Extract values for readability
//     double a1 = aVec.x, b1 = aVec.y;
//     double a2 = -bVec.x, b2 = -bVec.y;
//     double c1 = prizeVec.x, c2 = prizeVec.y;

//     // Calculate the determinant
//     double det = a1 * b2 - a2 * b1;

//     // Check for parallel lines (det = 0)
//     if (std::abs(det) < 1e-9) {
//         std::cerr << "Lines are parallel or coincident, no unique
//         intersection."
//                   << std::endl;
//         return std::nullopt;
//     }

//     // Solve for t and s using Cramer's Rule
//     double detT = c1 * b2 - c2 * b1;
//     double detS = a1 * c2 - a2 * c1;

//     double t = detT / det;
//     double s = detS / det;

//     // Find the intersection point using t
//     Vec intersection = {t * aVec.x, t * aVec.y};

//     return intersection;
// }

struct Matrix {
    __int128_t a = 0, b = 0, c = 0, d = 0;

    __int128_t det() const {
        return a * d - b * c;
    }

    Coord inverseMultiply(Coord v) {
        auto x = v.x;
        auto y = v.y;
        auto det = this->det();
        return {
            (d * x - b * y) / det,
            (-c * x + a * y) / det,
        };
    }
};

Coord algebraicSolution(const Machine &machine) {
    auto m = Matrix{
        machine.a.x,
        machine.b.x,
        machine.a.y,
        machine.b.y,
    };

    if (m.det() == 0) {
        return {std::numeric_limits<__int128>::max(), 0};
    }

    auto solution = m.inverseMultiply(machine.prize);

    auto control = Coord{
        machine.a.x * solution.x + machine.b.x * solution.y,
        machine.a.y * solution.x + machine.b.y * solution.y,
    };

    if (machine.prize != control) {
        return {-1, -1};
        // throw std::runtime_error{"Aha!"};
    }

    return solution;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/13"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    __int128 sum1 = 0;
    __int128 sum2 = 0;

    // The ugliest parsing in the world
    for (std::string aLine, bLine, prizeLine, _;;) {
        std::getline(file, aLine), std::getline(file, bLine),
            std::getline(file, prizeLine), std::getline(file, _);

        auto f1 = prizeLine.find("=");
        auto f2 = prizeLine.find("=", f1 + 1);

        // std::cout << prizeLine.substr(f1 + 1, f2 - f1 - 4) << "\n";
        // std::cout << prizeLine.substr(f2 + 1) << std::endl;

        auto machine =
            Machine{.a = {std::stoul(aLine.substr(12, 15)),
                          std::stoul(aLine.substr(18, 18 + 3))},
                    .b = {std::stoul(bLine.substr(12, 15)),
                          std::stoul(bLine.substr(18, 18 + 3))},
                    .prize = {std::stoul(prizeLine.substr(f1 + 1, f2 - f1 - 4)),
                              std::stoul(prizeLine.substr(f2 + 1))}

            };

        std::cout << aLine << std::endl;
        std::cout << std::format(
            "Button A: {}, {}\n", machine.a.x, machine.a.y);
        std::cout << std::format(
            "Button B: {}, {}\n", machine.b.x, machine.b.y);
        std::cout << std::format(
            "Price: {}, {}\n", machine.prize.x, machine.prize.y);

        auto cost = getLeastCost(machine, Coord{});
        if (cost != std::numeric_limits<__int128>::max()) {
            sum1 += cost;
            std::cout << "cost " << cost << std::endl;

            auto controlSolution = algebraicSolution(machine);

            auto controlCost = controlSolution.x * 3 + controlSolution.y;

            if (controlSolution.x + controlSolution.y < 100) {
                if (controlCost != cost) {
                    std::cerr << "they do not match " << controlCost << "\n";
                }
            }
        }
        else {
            std::cout << "cost " << -1 << std::endl;
        }

        machine.prize.x += offset;
        machine.prize.y += offset;

        auto solution = algebraicSolution(machine);
        std::cout << "..." << solution.x << ", " << solution.y << "\n";

        if (solution.x >= 0 && solution.y >= 0) {
            auto cost2 = solution.x * 3 + solution.y;
            std::cout << "cost2 " << cost2 << "\n";
            sum2 += cost2;
        }
        else {
            std::cout << "invalid solution\n";
        }

        std::cout << "sum2 " << sum2 << std::endl;

        if (!file) {
            break;
        }
    }

    std::cout << std::format("Part 1: {}\n", sum1);
    std::cout << std::format("Part 2: {}\n", sum2);
}
