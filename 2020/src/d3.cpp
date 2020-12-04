#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <vector>

namespace filesystem = std::experimental::filesystem;
using namespace std;

struct Canvas {
    std::vector<std::string> lines;

    Canvas(filesystem::path path) {
        auto file = std::fstream{path};

        for (std::string line; std::getline(file, line);) {
            lines.push_back(std::move(line));
        }
    }

    auto operator()(size_t x, size_t y) const {
        auto &line = lines.at(y);
        return line.at(x % line.size());
    }

    void pushLine(std::string line) {}
};

size_t go(const Canvas &canvas, std::pair<size_t, size_t> step = {3, 1}) {
    size_t sum = 0;

    for (size_t i = 0; i * step.second < canvas.lines.size(); ++i) {
        if (canvas(i * step.first, i * step.second) == '#') {
            ++sum;
        }
    }

    return sum;
}

size_t multipliedSlopes(Canvas canvas) {
    size_t product = 0;

    auto stepSizes = std::vector<std::pair<size_t, size_t>>{
        {1, 1}, {3, 1}, {5, 1}, {7, 1}, {1, 2}};

    for (auto step : stepSizes) {
        if (product) {
            product *= go(canvas, step);
        }
        else {
            product = go(canvas, step);
        }
    }
    return product;
}

int main(int argc, char **argv) {
    std::cout << "num hits (test): " << go(Canvas{"data/3-test.txt"}) << "\n";
    std::cout << "num hits (real): " << go(Canvas{"data/3.txt"}) << "\n";

    std::cout << "part two\n";
    std::cout << "num hits (real): "
              << multipliedSlopes(Canvas{"data/3-test.txt"}) << "\n";

    std::cout << "num hits (real): " << multipliedSlopes(Canvas{"data/3.txt"})
              << "\n";

    return 0;
}
