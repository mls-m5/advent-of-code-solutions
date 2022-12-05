#include "input.h"
#include <algorithm>
#include <iostream>
#include <regex>

using StacksT = std::vector<std::vector<char>>;
void printStacks(StacksT &stacks) {
    for (int i = 0; i < stacks.size(); ++i) {
        std::cout << i << ": \n";
        for (auto &c : stacks.at(i)) {
            std::cout << c << "\n";
        }
    }
}

int main(int argc, char *argv[]) {
    auto file = openFile("05", argc, argv);

    auto stacks = StacksT{};

    bool firstNonSpace = false;

    // Get the status of the crates
    for (std::string line; getline(file, line);) {
        stacks.resize((line.size() - 2) / 4 + 1);

        if (line.front() == ' ' && firstNonSpace) {
            getline(file, line);
            break;
        }
        else {
            firstNonSpace = true;
        }

        size_t i = 0;
        for (auto &stack : stacks) {
            if (auto c = line.at(i * 4 + 1); c != ' ') {
                stack.push_back(c);
            }
            ++i;
        }
    }

    for (auto &stack : stacks) {
        std::reverse(stack.begin(), stack.end());
    }

    printStacks(stacks);

    auto re = std::regex{"move ([0-9]+) from ([0-9]+) to ([0-9]+)"};

    for (std::string line; getline(file, line);) {
        std::smatch match;
        std::regex_match(line, match, re);

        auto num = std::stoi(match[1]);
        auto from = std::stoi(match[2]);
        auto to = std::stoi(match[3]);

        //        std::cout << "should move " << num << " from " << from << " to
        //        " << to
        //                  << "\n";

        for (int i = 0; i < num; ++i) {
            stacks.at(to - 1).push_back(stacks.at(from - 1).back());
            stacks.at(from - 1).pop_back();
        }

        //        printStacks(stacks);
        //        std::cout << std::endl;
    }

    for (auto &stack : stacks) {
        std::cout << stack.back();
    }

    std::cout << std::endl;

    return 0;
}
