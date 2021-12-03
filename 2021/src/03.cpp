#include "openfile.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

struct BitStats {
    struct Line {
        Line(std::string_view line) {
            bits.resize(line.size());
            std::transform(line.begin(), line.end(), bits.begin(), [](char c) {
                return c == '1';
            });
        }

        int value() {
            int ret = 0;
            for (auto b : bits) {
                ret <<= 1;
                ret |= b;
            }
            return ret;
        }

        std::vector<bool> bits;
        bool activeOne = 1;
        bool activeZero = 1;
    };

    std::vector<int> numOnes;
    std::vector<int> numZeros;
    std::vector<Line> lines;

    void push(std::string_view line) {
        if (numOnes.empty()) {
            numOnes.resize(line.size());
            numZeros.resize(line.size());
        }

        for (size_t i = 0; i < line.size(); ++i) {
            auto c = line.at(i);
            if (c == '1') {
                ++numOnes.at(i);
            }
            else {
                ++numZeros.at(i);
            }
        }

        lines.push_back(Line{std::string{line}});
    }

    struct Sum {
        int gamma, epsilon, oxygen, co;
    };

    Sum sum() {
        int gamma = 0;
        int epsilon = 0;
        int oxygen = -1;
        int co = -1;
        for (size_t i = 0; i < numOnes.size(); ++i) {
            gamma <<= 1;
            epsilon <<= 1;
            if (numOnes.at(i) > numZeros.at(i)) {
                gamma |= 1;
            }
            else {
                epsilon |= 1;
            }
        }

        int activeOnes = lines.size();
        int activeZeros = lines.size();

        for (size_t i = 0; i < numOnes.size() && (activeOnes || activeZeros);
             ++i) {

            int localOnes1 = 0;
            int localZeros1 = 0;
            int localOnes2 = 0;
            int localZeros2 = 0;

            for (auto &line : lines) {
                localOnes1 += line.activeOne && line.bits.at(i);
                localZeros1 += line.activeOne && !line.bits.at(i);
                localOnes2 += line.activeZero && line.bits.at(i);
                localZeros2 += line.activeZero && !line.bits.at(i);
            }

            bool oxygenBit = localOnes1 >= localZeros1;
            bool coBit = localOnes2 < localZeros2;

            for (size_t lineNum = 0; lineNum < lines.size(); ++lineNum) {
                auto &line = lines.at(lineNum);
                if (activeOnes > 1 && line.activeOne &&
                    line.bits.at(i) != oxygenBit) {
                    line.activeOne = false;
                    --activeOnes;
                }
                if (activeZeros > 1 && line.activeZero &&
                    line.bits.at(i) != coBit) {
                    line.activeZero = false;
                    --activeZeros;
                }
            }
        }

        {
            auto f = std::find_if(lines.begin(), lines.end(), [](Line &line) {
                return line.activeOne;
            });
            oxygen = f->value();
        }
        {
            auto f = std::find_if(lines.begin(), lines.end(), [](Line &line) {
                return line.activeZero;
            });
            co = f->value();
        }

        return {gamma, epsilon, oxygen, co};
    }
};

void solve(std::istream &file) {
    auto stats = BitStats{};

    for (std::string line; getline(file, line);) {
        stats.push(line);
    }

    auto [gamma, epsilon, oxygen, co] = stats.sum();

    std::cout << "gamma: " << gamma << " epsilon: " << epsilon << "\n";
    std::cout << gamma * epsilon << "\n";

    std::cout << "oxygen: " << oxygen << " co " << co << std::endl;
    std::cout << oxygen * co << std::endl;
}

int main(int argc, char *argv[]) {
    auto file = openFile("03", argc, argv);
    solve(file);

    return 0;
}
