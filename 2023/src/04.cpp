
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>

struct CardResult {
    int uselessPoints = 0;
    int index = 0;
    int newCopies = 0;
};

CardResult processLine(std::string line) {
    auto ss = std::istringstream{line};
    std::string trash, indexStr;
    ss >> trash >> indexStr;
    int index = std::stoi(indexStr);

    auto winning = std::set<int>{};

    for (std::string n; ss >> n;) {
        if (n == "|") {
            break;
        }

        winning.insert(std::stoi(n));
    }

    int sum = 0;
    int sum2 = 0;

    for (std::string n; ss >> n;) {
        if (winning.contains(std::stoi(n))) {
            if (!sum) {
                sum = 1;
            }
            else {
                sum <<= 1;
            }
            sum2 += 1;
        }
    }

    return {
        .uselessPoints = sum,
        .index = index,
        .newCopies = sum2,
    };
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/04"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    int sum1 = 0;

    auto resultingCopies = std::map<int, int>{};

    int lastIndex = 0;

    for (std::string line; std::getline(file, line);) {
        auto res = processLine(line);
        sum1 += res.uselessPoints;

        auto &numCopies = resultingCopies[res.index];
        // First copy
        ++numCopies;

        for (int i = res.index + 1; i <= res.index + res.newCopies; ++i) {
            resultingCopies[i] += numCopies;
        }

        lastIndex = res.index;
    }

    auto it = resultingCopies.find(lastIndex);
    ++it;
    resultingCopies.erase(it, resultingCopies.end());

    int sum2 = 0;
    for (auto it : resultingCopies) {
        std::cout << it.first << ": " << it.second << "\n";
        sum2 += it.second;
    }

    std::cout << "sum 1: " << sum1 << std::endl;
    std::cout << "sum 2: " << sum2 << std::endl;
}
