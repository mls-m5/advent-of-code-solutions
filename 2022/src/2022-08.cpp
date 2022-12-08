#include "input.h"
#include <iostream>

int main(int argc, char *argv[]) {
    auto file = openFile("08", argc, argv);

    auto trees = std::vector<std::string>{};

    for (std::string line; getline(file, line);) {
        trees.push_back(line);
    }
    auto width = trees.front().size(), height = trees.size();

    auto at = [&trees](int x, int y) -> int { return trees.at(y).at(x) - '0'; };

    auto isVisible = [&](int tx, int ty) -> bool {
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

    auto scenicScore = [&](int tx, int ty) {
        auto h = at(tx, ty);
        int hleft = 0, hright = 0;
        int hup = 0, hdown = 0;

        for (int x = tx - 1; x >= 0; --x) {
            ++hleft;
            if (at(x, ty) >= h) {
                break;
            }
        }
        for (int x = tx + 1; x < width; ++x) {
            ++hright;
            if (at(x, ty) >= h) {
                break;
            }
        }
        for (int y = ty - 1; y >= 0; --y) {
            ++hup;
            if (at(tx, y) >= h) {
                break;
            }
        }
        for (int y = ty + 1; y < height; ++y) {
            ++hdown;
            if (at(tx, y) >= h) {
                break;
            }
        }
        return hleft * hright * hup * hdown;
    };

    int visible = width * 2 + height * 2 - 4; // Edge trees
    int sscore = 0;

    for (int y = 1; y < height - 1; ++y) {
        for (int x = 1; x < width - 1; ++x) {
            visible += isVisible(x, y);
            sscore = std::max(sscore, scenicScore(x, y));
        }
    }

    std::cout << "result 1: " << visible << "\n";
    std::cout << "result 1: " << sscore << "\n";
}
