#include "input.h"
#include <algorithm>
#include <iostream>

int priority(unsigned char c) {
    if (c >= 'a') {
        return c - 'a' + 1;
    }
    return c - 'A' + 27;
};

std::string intersect(std::string a, std::string b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());

    auto intersection = std::string{};

    std::set_intersection(a.begin(),
                          a.end(),
                          b.begin(),
                          b.end(),
                          std::back_insert_iterator{intersection});

    return intersection;
};

int main(int argc, char *argv[]) {
    auto file = openFile("03", argc, argv);

    int memberNum = 0;
    auto groupCommon = std::string{};

    int result1 = 0;
    int result2 = 0;

    for (std::string line; getline(file, line); ++memberNum) {
        { // part 1
            auto a = line.substr(0, line.size() / 2);
            auto b = line.substr(line.size() / 2);
            auto intersection = intersect(a, b);

            result1 += priority(intersection.front());
        }

        { // part 2
            if (memberNum % 3 == 0) {
                groupCommon = line;
                continue;
            }

            groupCommon = intersect(line, groupCommon);

            if (memberNum % 3 == 2) {
                result2 += priority(groupCommon.front());
            }
        }
    }

    std::cout << "result 1: " << result1 << std::endl;
    std::cout << "result 2: " << result2 << std::endl;

    return 0;
}
