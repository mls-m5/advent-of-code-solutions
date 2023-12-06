
#include <fstream>
#include <iostream>
#include <numeric>
#include <ranges>
#include <sstream>
#include <string>
#include <vector>

using IntT = long long;

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/06"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto discard = std::string{};

    auto f = [&]() {
        auto concat = std::string{};
        auto line = std::string{};
        std::getline(file, line);

        auto ss = std::stringstream{line};
        ss >> discard;

        auto values = std::vector<IntT>{};
        for (std::string str; ss >> str;) {
            IntT value = std::stoi(str);
            concat += str;

            values.push_back(value);
        }

        return std::pair{values, std::stoll(concat)};
    };

    auto [times, timeConcat] = f();
    auto [records, recordConcat] = f();

    auto f2 = [](const std::vector<IntT> &times,
                 const std::vector<IntT> &records,
                 IntT min) {
        auto results = std::vector<IntT>(records.size(), 0);

        std::cout << "results: ";

        using namespace std::literals;

        for (size_t i : std::ranges::iota_view{0uz, times.size()}) {
            auto time = times.at(i);
            auto record = records.at(i);

            for (IntT pressed = min; pressed <= time - min; ++pressed) {
                auto result = pressed * (time - pressed);
                if (result > record) {
                    ++results.at(i);
                }
            }

            std::cout << results.at(i);
        }

        std::cout << "\n";

        auto product = std::accumulate(
            results.begin(), results.end(), 1, std::multiplies<IntT>());

        return product;
    };

    auto product = f2(times, records, 0);

    std::cout << "product: " << product << std::endl;

    std::cout << "concat times: " << timeConcat << std::endl;

    auto product2 = f2({timeConcat}, {recordConcat}, 14);

    std::cout << "product 2: " << product2 << std::endl;
}
