#include <algorithm>
#include <experimental/filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace filesystem = std::experimental::filesystem;

namespace {

std::vector<std::string> requiredFields = {
    "byr", "iyr", "eyr", "hgt", "hcl", "ecl", "pid"};

std::vector<std::string> validEyes = {
    "amb", "blu", "brn", "gry", "grn", "hzl", "oth"};

bool validate(std::string fieldName, std::string value) {
    if (fieldName == "byr") {
        auto i = std::stoi(value);
        return i >= 1920 && i <= 2002;
    }
    if (fieldName == "iyr") {
        auto i = std::stoi(value);
        return i >= 2010 && i <= 2020;
    }
    if (fieldName == "eyr") {
        auto i = std::stoi(value);
        return i >= 2020 && i <= 2030;
    }
    if (fieldName == "hgt") {
        if (!isalpha(value.back())) {
            return false;
        }
        auto unit = value.substr(value.size() - 2);
        auto i = std::stoi(value.substr(0, value.size() - 2));
        return unit == "cm" ? (i >= 150 && i <= 193) : (i >= 59 && i <= 76);
    }
    if (fieldName == "hcl") {
        if (value.front() != '#') {
            return false;
        }
        for (auto c : value.substr(1)) {
            if (!(c >= '0' && c <= '9') && !(c >= 'a' && c <= 'f')) {
                return false;
            }
        }
        return true;
    }
    if (fieldName == "ecl") {
        return std::find(validEyes.begin(), validEyes.end(), value) !=
               validEyes.end();
    }
    if (fieldName == "pid") {
        for (auto c : value) {
            if (c < '0' || c > '9') {
                return false;
            }
        }
        return value.size() == 9;
    }

    return false;
}

struct Passport {
    size_t missingFields = 7;

    Passport(std::istream &stream, bool extraValidation = false) {
        std::cout << "passport begin\n";
        for (std::string line; getline(stream, line) && !line.empty();) {
            std::istringstream ss(move(line));
            for (std::string word; ss >> word;) {
                auto name = word.substr(0, 3);
                auto value = word.substr(4);

                if (std::find(requiredFields.begin(),
                              requiredFields.end(),
                              name) != requiredFields.end()) {
                    if (!extraValidation || validate(name, value)) {
                        --missingFields;
                    }
                }
            }
        }

        std::cout << "missing fields: " << missingFields << "\n";
    }
};

size_t countValid(filesystem::path path, bool extraValidation = false) {
    auto file = std::ifstream{path};
    auto sum = 0;
    do {
        auto passport = Passport{file, extraValidation};
        if (!passport.missingFields) {
            ++sum;
        }
    } while (file);

    return sum;
}

auto assert_eq = [](auto a, auto b) {
    if (a != b) {
        throw std::runtime_error("this should be true");
    }
};

void test() {
    assert_eq(validate("byr", "2002"), true);
    assert_eq(validate("byr", "2003"), false);

    assert_eq(validate("hgt", "60in"), true);
    assert_eq(validate("hgt", "190cm"), true);
    assert_eq(validate("hgt", "190in"), false);
    assert_eq(validate("hgt", "190"), false);

    assert_eq(validate("hcl", "#123abc"), true);
    assert_eq(validate("hcl", "#123abz"), false);
    assert_eq(validate("hcl", "123abz"), false);

    assert_eq(validate("ecl", "brn"), true);
    assert_eq(validate("ecl", "wat"), false);

    assert_eq(validate("pid", "000000001"), true);
    assert_eq(validate("pid", "0123456789"), false);
}

} // namespace

int main(int, char *[]) {
    test();

    std::cout << "num valid test 1: " << countValid("data/4-test.txt") << "\n";
    std::cout << "num valid 1: " << countValid("data/4.txt") << "\n";

    std::cout << "num valid 2: " << countValid("data/4.txt", true) << "\n";

    return 0;
}
