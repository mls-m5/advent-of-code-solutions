#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <string_view>

using namespace std::literals;

auto limits = std::map<std::string, int>{
    {"red", 12},
    {"green", 13},
    {"blue", 14},
};

int game(std::string line) {
    int id = 0;

    {
        auto f = line.find(":");
        id = std::stoi(line.substr(5, f - 5));
        line = line.substr(f + 2);
    }

    std::cout << id << ": " << line << std::endl;

    auto ss = std::istringstream{line};

    for (std::string set; std::getline(ss, set, ';');) {
        std::cout << set << "\n";

        auto ss2 = std::istringstream{set};
        for (std::string n, c; ss2 >> n >> c;) {
            if (c.back() == ',') {
                c.pop_back();
            }
            std::cout << n << " " << c << "\n";
            if (std::stoi(n) > limits.at(c)) {
                std::cout << "no\n";
                return 0;
            }
        }
    }

    return id;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/02"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};
    int sum = 0;

    for (std::string line; std::getline(file, line);) {
        sum += game(line);
    }

    std::cout << sum << "\n";
}
