
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;
using namespace std::experimental;
using namespace std::literals;

int main(int argc, char **argv) {
    filesystem::path path =
        (argc > 1 && argv[1] == "--test"s) ? "data/1-test.txt" : "data/1.txt";

    auto file = fstream{path};

    if (!file.is_open()) {
        std::cerr << "could not open " << path << "\n";
    }

    auto values = std::vector<long>{};

    for (long i; file >> i;) {
        values.push_back(i);
    }

    if (values.empty()) {
        std::cerr << "no input\n";
        return 1;
    }

    for (size_t i = 0; i < values.size() - 1; ++i) {
        for (size_t j = i + 1; j < values.size(); ++j) {
            if (values.at(i) + values.at(j) == 2020) {
                std::cout << values.at(i) * values.at(j) << std::endl;

                break; // Only breaks one layer... who cares
            }
        }
    }

    std::cout << "part two:\n";

    for (size_t i = 0; i < values.size() - 1; ++i) {
        for (size_t j = i + 1; j < values.size(); ++j) {
            for (size_t k = 0; k < values.size(); ++k) {
                if (i != j && j != k && i != k) {
                    if (values.at(i) + values.at(j) + values.at(k) == 2020) {
                        std::cout << values.at(i) * values.at(j) * values.at(k)
                                  << std::endl;
                        return 0;
                    }
                }
            }
        }
    }

    return 0;
}
