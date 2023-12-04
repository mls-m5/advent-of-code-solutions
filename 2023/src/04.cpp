
#include <fstream>
#include <iostream>
#include <set>
#include <sstream>
#include <string>

int processLine(std::string line) {
    auto ss = std::istringstream{line};
    std::string trash;
    ss >> trash >> trash;

    auto winning = std::set<int>{};

    for (std::string n; ss >> n;) {
        if (n == "|") {
            break;
        }

        winning.insert(std::stoi(n));
    }

    int sum = 0;

    for (std::string n; ss >> n;) {
        if (winning.contains(std::stoi(n))) {
            if (!sum) {
                sum = 1;
            }
            else {
                sum <<= 1;
            }
        }
    }

    return sum;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/04"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    int sum1 = 0;

    for (std::string line; std::getline(file, line);) {
        sum1 += processLine(line);
    }

    std::cout << "sum 1: " << sum1 << std::endl;
}
