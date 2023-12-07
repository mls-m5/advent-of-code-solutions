#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <stdexcept>
#include <string>
#include <string_view>

using IntT = int64_t;

const auto conversionMap = std::map<char, int>{
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10}, // T for 10
    {'J', 11}, // J for Jack
    {'Q', 12}, // Q for Queen
    {'K', 13}, // K for King
    {'A', 14}  // A for Ace
};

const auto conversionMap2 = std::map<char, int>{
    {'J', 0}, // J for Joker
    {'2', 2},
    {'3', 3},
    {'4', 4},
    {'5', 5},
    {'6', 6},
    {'7', 7},
    {'8', 8},
    {'9', 9},
    {'T', 10}, // T for 10
    {'Q', 12}, // Q for Queen
    {'K', 13}, // K for King
    {'A', 14}  // A for Ace
};

struct Hand {
    enum Type {
        High,
        Pair,
        TwoPair,
        Three,
        FullHouse,
        Four,
        Five,
    };

    std::string cards;
    std::array<int, 5> values = {};
    IntT bet = 0;
    int type = High;

    Hand(std::string_view cardsValue, IntT bet, int version) : bet{bet} {
        cards = std::string{cardsValue};
        for (size_t i = 0; i < cards.size(); ++i) {
            if (version == 0) {
                values.at(i) = conversionMap.at(cardsValue.at(i));
            }
            else {
                values.at(i) = conversionMap2.at(cardsValue.at(i));
            }
        }

        parseType(version);
    }

    bool operator<(const Hand &other) const {
        if (type != other.type) {
            return type < other.type;
        }

        for (size_t i = 0; i < values.size(); ++i) {
            auto c1 = values.at(i);
            auto c2 = other.values.at(i);
            if (c1 == c2) {
                continue;
            }
            return c1 < c2;
        }

        throw std::runtime_error{"undefined with same hands"};
    }

    void parseType(int version) {
        auto count = std::map<int, int>{};

        auto jCount = 0;
        for (auto c : values) {
            if (version == 1 && c == 0) {
                ++jCount;
            }
            else {
                ++count[c];
            }
        }

        {
            // All cards were jokers
            if (count.empty()) {
                type = Five;
                return;
            }
            else {
                auto maxElement = std::max_element(
                    count.begin(), count.end(), [](auto &a, auto &b) {
                        return a.second < a.second;
                    });
                maxElement->second += jCount;
            }
        }

        auto reversed = std::map<int, int>{};
        for (auto it : count) {
            ++reversed[it.second];
        }

        if (reversed[5]) {
            type = Five;
            return;
        }

        if (reversed[4]) {
            type = Four;
            return;
        }

        if (reversed[3] && reversed[2]) {
            type = FullHouse;
            return;
        }

        if (reversed[3]) {
            type = Three;
            return;
        }

        if (reversed[2] >= 2) {
            type = TwoPair;
            return;
        }

        if (reversed[2]) {
            type = Pair;
            return;
        }
    }

    std::string str() {
        auto ret = std::string{};
        ret += cards;
        ret.push_back('-');

        for (auto c : values) {
            ret += std::to_string(c) + " ";
        }

        ret.pop_back();
        return ret;
    }
};

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/07"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    auto set = std::set<Hand>{};
    auto set2 = std::set<Hand>{};

    for (std::string handStr, bet; file >> handStr >> bet;) {
        auto hand = Hand(handStr, std::stoll(bet), 0);
        auto hand2 = Hand(handStr, std::stoll(bet), 1);

        set.insert(hand);
        set2.insert(hand2);
    }

    auto f = [](const auto &set) {
        IntT sum = 0;
        IntT rank = 1;
        for (auto hand : set) {
            std::cout << hand.str() << " bet: " << hand.bet
                      << ", type: " << hand.type << ", rank " << rank
                      << ", result: " << rank * hand.bet << std::endl;

            sum += rank * hand.bet;
            ++rank;
        }

        std::cout << "sum: " << sum << std::endl;
    };

    f(set);
    f(set2);
}
