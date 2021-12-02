#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

void solve(std::istream &file) {
    std::string direction;
    // For part 1
    int x = 0;
    int y = 0;

    // For part 2
    int x2 = 0;
    int y2 = 0;
    int aim = 0;
    for (int amount; file >> direction, file >> amount;) {
        if (direction == "forward") {
            x += amount;
            x2 += amount;
            y2 += aim * amount;
        }
        else if (direction == "up") {
            y -= amount;
            aim -= amount;
        }
        else if (direction == "down") {
            y += amount;
            aim += amount;
        }
    }

    std::cout << "\npart 1:\n";
    std::cout << "x = " << x << ", y = " << y << "\n";
    std::cout << x * y << "\n";

    std::cout << "\npart 2:\n";
    std::cout << "x = " << x2 << ", y = " << y2 << "\n";
    std::cout << x2 * y2 << std::endl;
}

std::ifstream openFile(int argc, char *argv[]) {
    auto args = std::vector<std::string_view>(argv + 1, argv + argc);
    auto isTest = (!args.empty() && args.at(0) == "--test");

    auto path =
        std::filesystem::path{"data/02"s + (isTest ? "-test.txt" : ".txt")};

    auto file = std::ifstream{path};

    if (!file) {
        std::cerr << "could not open " << path << "\n";
        exit(1);
    }

    return file;
}

int main(int argc, char *argv[]) {
    auto file = openFile(argc, argv);
    solve(file);

    return 0;
}
