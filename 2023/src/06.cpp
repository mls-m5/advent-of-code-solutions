
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/06"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto discard = std::string{};

    auto f = [&]() {
        auto line = std::string{};
        std::getline(file, line);

        auto ss = std::stringstream{line};
        ss >> discard;

        auto values = std::vector<int>{};
        for (int value = 0; ss >> value;) {
            values.push_back(value);
        }

        return values;
    };

    auto times = f();
    auto records = f();
    auto results = std::vector<int>(records.size(), 0);

    std::cout << "results: ";

    using namespace std::literals;

    for (size_t i : std::ranges::iota_view{0uz, times.size()}) {
        auto time = times.at(i);
        auto record = records.at(i);

        for (int pressed = 0; pressed <= time; ++pressed) {
            auto result = pressed * (time - pressed);
            if (result > record) {
                ++results.at(i);
            }
        }

        std::cout << results.at(i);
    }

    std::cout << "\n";

    auto product = std::accumulate(
        results.begin(), results.end(), 1, std::multiplies<int>());

    std::cout << "product: " << product << std::endl;
}
