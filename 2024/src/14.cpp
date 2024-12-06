#include <fstream>
#include <iostream>
#include <string>

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/14"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    for (std::string line; std::getline(file, line);) {
        std::cout << line << std::endl;
    }
}
