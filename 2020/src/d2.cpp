#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>

using namespace std;
using namespace std::experimental;
using namespace std::literals;

namespace {

enum class Validation {
    Old,
    New,
};

auto split(std::string line, char separator = ' ') -> std::vector<std::string> {
    istringstream ss(line);

    std::vector<std::string> ret;

    for (std::string word; getline(ss, word, separator);) {
        ret.push_back(word);
    }

    return ret;
}

struct Entry {
    size_t min = 0, max = 0;
    char c = '\0';
    std::string content;

    Entry(std::string line) {
        auto words = split(std::move(line));
        auto sizes = split(words.front(), '-');

        min = stoul(sizes.at(0));
        max = stoul(sizes.at(1));
        c = words.at(1).front();
        content = words.at(2);
    }

    bool isValid(Validation validation) {
        if (validation == Validation::Old) {
            size_t count = std::count(content.begin(), content.end(), c);
            return count >= min && count <= max;
        }
        else {
            try {
                return content.at(min - 1) == c xor content.at(max - 1) == c;
            }
            catch (...) {
                return false;
            }
        }
    }
};

auto loadEntries(std::istream &stream) -> std::vector<Entry> {
    std::vector<Entry> entries;

    for (string line; getline(stream, line);) {
        entries.emplace_back(std::move(line));
    }

    return entries;
}

void process(filesystem::path path, Validation validation) {
    auto file = fstream{path};
    auto entries = loadEntries(file);

    for (auto &entry : entries) {
        std::cout << "min: " << entry.min << ", max: " << entry.max;
        std::cout << ", " << entry.c << ": " << entry.content << ", "
                  << (entry.isValid(validation) ? "valid" : "invalid") << "\n";
    }

    size_t count = std::count_if(
        entries.begin(), entries.end(), [validation](auto &&entry) {
            return entry.isValid(validation);
        });

    std::cout << "num valid: " << count << "\n";
}

} // namespace

int main(int argc, char *argv[]) {
    process("data/2-test.txt", Validation::Old);
    process("data/2.txt", Validation::Old);

    process("data/2-test.txt", Validation::New);
    process("data/2.txt", Validation::New);
    return 0;
}
