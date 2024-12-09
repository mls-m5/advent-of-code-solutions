#include <format>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

struct Coord {
    int x = 0;
    int y = 0;
};

Coord charToCoord(char c) {
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
    return {-2, -2};
}

char nextChar(char c) {
    switch (c) {
    case '^':
        return '>';
    case '>':
        return 'v';
    case 'v':
        return '<';
    case '<':
        return '^';
    }
    return '?';
}

struct Canvas {
    std::vector<std::string> lines;

    Coord guardPos;
    char guardDir = '\0';
    bool didFinish = false;
    int stepsSinceLastNewCell = 0;
    int maxStepSinceLastNewCell = 0;
    int limitBeforeBreaking = 1000;

    Canvas() {}

    char &at(int x, int y) {
        return lines.at(y).at(x);
    }

    void parsePosition() {
        for (size_t y = 0; y < lines.size(); ++y) {
            auto &line = lines.at(y);
            for (size_t x = 0; x < line.size(); ++x) {
                auto &c = line.at(x);
                if (c == '^') {
                    guardPos.x = x;
                    guardPos.y = y;
                    guardDir = c;
                    c = 'X';
                    return;
                }
            }
        }
    }

    void print() {
        for (auto &line : lines) {
            std::cout << line << "\n";
        }
        std::cout << "\n";
    }

    bool isInside(int x, int y) {
        return x >= 0 && y >= 0 && x < lines.front().size() && y < lines.size();
    }

    bool step() {
        auto vel = charToCoord(guardDir);
        auto newPos = Coord{guardPos.x + vel.x, guardPos.y + vel.y};
        ++stepsSinceLastNewCell;
        maxStepSinceLastNewCell =
            std::max(maxStepSinceLastNewCell, stepsSinceLastNewCell);

        if (!isInside(newPos.x, newPos.y)) {
            didFinish = true;
            return false;
        }

        if (at(newPos.x, newPos.y) == '#' || at(newPos.x, newPos.y) == 'O') {
            guardDir = nextChar(guardDir);
        }
        else {
            // print();

            guardPos = newPos;
            auto &c = at(guardPos.x, guardPos.y);
            if (c != 'X') {
                c = 'X';
                stepsSinceLastNewCell = 0;
            }
        }

        return true;
    }

    void run() {
        for (; step();) {
            if (maxStepSinceLastNewCell > limitBeforeBreaking) {
                return;
            }
        }
        didFinish = true;
    }

    int count() {
        int sum = 0;
        for (auto &line : lines) {
            for (auto c : line) {
                if (c == 'X') {
                    ++sum;
                }
            }
        }

        return sum;
    }
};

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/06"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto canvas = Canvas{};

    for (std::string line; std::getline(file, line);) {
        canvas.lines.push_back(line);
    }

    canvas.print();
    canvas.parsePosition();
    auto copy = canvas;

    canvas.run();

    canvas.print();

    auto sum1 = canvas.count();

    std::cout << std::format("Part 1: {}\n", sum1);

    int sum2 = 0;
    for (size_t y = 0; y < canvas.lines.size(); ++y) {
        for (size_t x = 0; x < canvas.lines.front().size(); ++x) {
            auto tmpCanvas = copy;

            auto &c = tmpCanvas.at(x, y);
            if (c == '#') {
                continue;
            }

            c = 'O';
            // tmpCanvas.print();
            tmpCanvas.run();
            // tmpCanvas.print();

            sum2 += !tmpCanvas.didFinish;
        }
    }

    std::cout << std::format("Part 2: {}\n", sum2);
}
