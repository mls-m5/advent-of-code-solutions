#include <chrono>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

struct Element {
    std::string name;
    std::string leftName;
    std::string rightName;
    Element *left = nullptr;
    Element *right = nullptr;
};

int main(int argc, char *argv[]) {
    auto start = std::chrono::high_resolution_clock::now();

    auto file = std::ifstream{std::string{"data/08"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto elements = std::unordered_map<std::string, Element>{};
    auto current = std::vector<Element *>{};

    auto sequence = std::string{};

    std::getline(file, sequence);

    auto match = std::smatch{};

    auto re = std::regex{R"_(([A-Z]{3}) = \(([A-Z]{3}), ([A-Z]{3})\))_"};

    for (std::string line; std::getline(file, line);) {
        if (line.empty()) {
            continue;
        }

        auto ss = std::istringstream{line};

        if (std::regex_match(line, match, re)) {
            auto name = match[1];
            auto left = match[2];
            auto right = match[3];
            std::cout << match[1] << " = " << match[2] << "," << match[3]
                      << "\n";

            elements[name] = Element{name, left, right};
        }
    }

    for (auto &element : elements) {
        element.second.left = &elements.at(element.second.leftName);
        element.second.right = &elements.at(element.second.rightName);
    }

    long long steps = 0;
    size_t sequenceIndex = 0;
    for (auto element = &elements.at("AAA");; ++steps) {
        if (element->name == "ZZZ") {
            break;
        }

        if (sequence.at(sequenceIndex) == 'L') {
            element = element->left;
        }
        else {
            element = element->right;
        }

        sequenceIndex += 1;
        if (sequenceIndex >= sequence.size()) {
            sequenceIndex = 0;
        }
    }

    std::cout << steps << std::endl;

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = stop - start;

    std::cout << "us time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(duration)
                     .count()
              << std::endl;
}
