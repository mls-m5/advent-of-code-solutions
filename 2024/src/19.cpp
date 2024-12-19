#include <format>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

int countMatches(std::string_view str, std::vector<std::string> &words) {
    int count = 0;
    for (auto &word : words) {
        if (word.size() > str.size()) {
            continue;
        }

        if (str == word) {
            // return 1;
            count += 1;
            continue;
        }

        if (!str.starts_with(word)) {
            continue;
        }

        count += countMatches(str.substr(word.size()), words);
    }

    return count;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/19-filtered"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto match = std::smatch{};

    auto words = [&] {
        auto words = std::vector<std::string>{};
        std::string line;
        std::getline(file, line);
        auto ss = std::istringstream{line};
        for (std::string word; ss >> word;) {
            if (word.back() == ',') {
                word.pop_back();
            }

            words.push_back(std::move(word));
        }

        std::sort(words.begin(), words.end(), [](auto &a, auto &b) {
            return a.size() > b.size();
        });
        return words;
    }();

    for (std::string_view word : words) {
        std::cout << word << " ";
    }

    std::cout << std::endl;

    int sum2 = 0;

    for (std::string line; std::getline(file, line);) {
        if (line.empty()) {
            continue;
        }
        std::cout << line << std::endl;
        auto count = countMatches(line, words);
        sum2 += count;
        std::cout << count << std::endl;
    }

    std::cout << std::format("Part 2: {}", sum2) << std::endl;
}
