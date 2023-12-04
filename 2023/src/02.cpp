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

std::pair<int, int> game(std::string line) {
    int id = 0;

    auto max = std::map<std::string, int>{};

    {
        auto f = line.find(":");
        id = std::stoi(line.substr(5, f - 5));
        line = line.substr(f + 2);
    }

    auto ss = std::istringstream{line};

    for (std::string set; std::getline(ss, set, ';');) {

        auto ss2 = std::istringstream{set};
        for (std::string n, c; ss2 >> n >> c;) {
            if (c.back() == ',') {
                c.pop_back();
            }
            auto number = std::stoi(n);
            if (number > limits.at(c)) {
                id = 0;
            }

            auto &m = max[c];
            m = std::max(m, number);
        }
    }

    int power = max["red"] * max["green"] * max["blue"];

    return {id, power};
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/02"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};
    int sum = 0;
    int sum2 = 0;

    for (std::string line; std::getline(file, line);) {
        auto [id, power] = game(line);
        sum += id;
        sum2 += power;
    }

    std::cout << "answer 1: " << sum << "\n";
    std::cout << "answer 2: " << sum2 << "\n";
}
