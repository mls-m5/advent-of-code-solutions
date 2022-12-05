#include "input.h"
#include <algorithm>
#include <iostream>
#include <regex>

int main(int argc, char *argv[]) {
    auto file = openFile("05", argc, argv);

    auto stacks1 = std::vector<std::string>{};

    bool firstNonSpace = false;

    // Get the status of the crates
    for (std::string line; getline(file, line);) {
        stacks1.resize((line.size() - 2) / 4 + 1);

        if (line.front() == ' ' && firstNonSpace) {
            getline(file, line);
            break;
        }
        else {
            firstNonSpace = true;
        }

        for (size_t i = 0; i < stacks1.size(); ++i) {
            auto &stack = stacks1.at(i);
            if (auto c = line.at(i * 4 + 1); c != ' ') {
                stack.insert(stack.begin(), c);
            }
        }
    }

    auto stacks2 = stacks1;

    auto re = std::regex{"move ([0-9]+) from ([0-9]+) to ([0-9]+)"};

    for (std::string line; getline(file, line);) {
        std::smatch match;
        std::regex_match(line, match, re);

        auto num = std::stoi(match[1]);
        auto from = std::stoi(match[2]);
        auto to = std::stoi(match[3]);

        auto &fromS2 = stacks2.at(from - 1);
        int startIndex2 = fromS2.size() - num;

        for (int i = 0; i < num; ++i) {
            stacks1.at(to - 1).push_back(stacks1.at(from - 1).back());
            stacks1.at(from - 1).pop_back();

            stacks2.at(to - 1).push_back(fromS2.at(startIndex2));
            fromS2.erase(fromS2.begin() + startIndex2);
        }
    }

    auto printResult = [](int num, const auto &stacks) {
        std::cout << "result " << num << ": ";
        for (auto &stack : stacks) {
            std::cout << stack.back();
        }
        std::cout << std::endl;
    };

    printResult(1, stacks1);
    printResult(2, stacks2);

    return 0;
}
