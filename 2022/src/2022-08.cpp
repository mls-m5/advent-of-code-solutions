
#include "input.h"
#include <iostream>

int main(int argc, char *argv[]) {
    auto file = openFile("08", argc, argv);

    auto trees = std::vector<std::string>{};

    for (std::string line; getline(file, line);) {
        std::cout << line << "\n";
        trees.push_back(line);
    }

    auto width = trees.front().size();
    auto height = trees.size();

    auto at = [&trees](int x, int y) -> int { return trees.at(y).at(x) - '0'; };

    auto isVisible = [&](int tx, int ty) -> bool {
        //        auto hleft = at(0, ty), hright = at(width - 1, ty);
        //        auto hup = at(tx, 0), hdown = at(tx, height - 1);
        int hleft = 0, hright = 0;
        int hup = 0, hdown = 0;
        for (int x = 0; x < tx; ++x) {
            hleft = std::max(hleft, at(x, ty));
        }
        for (int x = tx + 1; x < width; ++x) {
            hright = std::max(hright, at(x, ty));
        }
        for (int y = 0; y < ty; ++y) {
            hup = std::max(hup, at(tx, y));
        }
        for (int y = ty + 1; y < height; ++y) {
            hdown = std::max(hdown, at(tx, y));
        }

        auto h = at(tx, ty);
        return h > hleft || h > hright || h > hup || h > hdown;
    };

    int visible = width * 2 + height * 2 - 4; // Edge trees
    for (int y = 1; y < height - 1; ++y) {
        std::cout << "-";
        for (int x = 1; x < width - 1; ++x) {
            auto v = isVisible(x, y);

            if (v) {
                std::cout << '*';
            }
            else {
                std::cout << at(x, y);
            }

            visible += isVisible(x, y);
        }

        std::cout << "\n";
    }

    std::cout << "result 1: " << visible << "\n";

    std::cout.flush();
    return 0;
}
