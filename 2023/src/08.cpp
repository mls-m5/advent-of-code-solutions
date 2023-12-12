#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <ostream>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>

struct Element {
    std::string name;
    std::string leftName;
    std::string rightName;
    Element *left = nullptr;
    Element *right = nullptr;
};

int main(int argc, char *argv[]) {

    auto file = std::ifstream{std::string{"data/08"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    // auto elements = std::unordered_map<std::string, Element>{};
    auto elements = std::vector<Element>{};
    auto current = std::vector<Element *>{};

    auto sequence = std::string{};

    std::getline(file, sequence);

    auto match = std::smatch{};

    auto re =
        std::regex{R"_(([A-Z0-9]{3}) = \(([A-Z0-9]{3}), ([A-Z0-9]{3})\))_"};

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

            // elements[name] = Element{name, left, right};
            elements.push_back({name, left, right});
        }
    }

    auto at = [&](std::string name) {
        auto f = std::find_if(
            elements.begin(), elements.end(), [name](auto &element) {
                return element.name == name;
            });

        if (f == elements.end()) {
            throw std::out_of_range{"could not find element"};
        }
        return &*f;
    };

    for (auto &element : elements) {
        // element.second.left = &elements.at(element.second.leftName);
        // element.second.right = &elements.at(element.second.rightName);

        element.left = at(element.leftName);
        element.right = at(element.rightName);

        if (element.name.ends_with('A')) {
            current.push_back(&element);
        }
    }

    try {
        auto start = std::chrono::high_resolution_clock::now();
        long long steps = 0;
        size_t sequenceIndex = 0;
        for (auto element = at("AAA");; ++steps) {
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
                  << std::chrono::duration_cast<std::chrono::microseconds>(
                         duration)
                         .count()
                  << std::endl;
    }
    catch (...) {
        std::cout << "probably not the first type" << std::endl;
    }

    long long steps = 1;
    size_t sequenceIndex = 0;

    auto start = std::chrono::high_resolution_clock::now();
    for (;;) {

        if (elements.size() < 20) {
            for (auto &c : current) {
                std::cout << c->name << " ";
            }
            std::cout << "\n";
        }

        bool isDone = true;
        for (auto &c : current) {
            auto &element = c;
            if (sequence.at(sequenceIndex) == 'L') {
                element = element->left;
            }
            else {
                element = element->right;
            }

            if (c->name.back() != 'Z') {
                isDone = false;
            }
        }

        if (isDone) {
            break;
        }

        ++steps;

        sequenceIndex += 1;
        if (sequenceIndex >= sequence.size()) {
            sequenceIndex = 0;
        }

        if (steps % 10000000 == 0) {
            std::cout << "currently " << steps << " steps" << std::endl;
        }
    }

    std::cout << "ghost steps: " << steps << std::endl;
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = stop - start;
    std::cout << "us time: "
              << std::chrono::duration_cast<std::chrono::microseconds>(duration)
                     .count()
              << std::endl;
}
