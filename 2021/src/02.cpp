#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std::literals;

void solve(std::istream &file) {
    std::string direction;
    int x = 0;
    int y = 0;
    for (int amount; file >> direction, file >> amount;) {
        std::cout << direction << " " << amount << "\n";
        if (direction == "forward") {
            x += amount;
        }
        else if (direction == "up") {
            y -= amount;
        }
        else if (direction == "down") {
            y += amount;
        }
    }

    std::cout << "x = " << x << ", y = " << y << "\n";
    std::cout << x * y << std::endl;
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
