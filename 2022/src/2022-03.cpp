#include "input.h"
#include <algorithm>
#include <iostream>

int main(int argc, char *argv[]) {
    auto file = openFile("03", argc, argv);

    auto priority = [](unsigned char c) {
        if (c >= 'a') {
            return c - 'a' + 1;
        }
        return c - 'A' + 27;
    };

    int result1 = 0;

    for (std::string line; getline(file, line);) {
        auto a = line.substr(0, line.size() / 2);
        auto b = line.substr(line.size() / 2);

        std::sort(a.begin(), a.end());
        std::sort(b.begin(), b.end());

        std::cout << line << "\n";

        std::string intersection;

        std::set_intersection(a.begin(),
                              a.end(),
                              b.begin(),
                              b.end(),
                              std::back_insert_iterator{intersection});

        std::cout << intersection << "\n";
        std::cout << priority(intersection.front()) << "\n";

        result1 += priority(intersection.front());
    }

    std::cout << "result 1: " << result1 << std::endl;

    return 0;
}
