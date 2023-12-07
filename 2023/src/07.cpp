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

auto conversionMap = std::map<char, int>{
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

    std::array<int, 5> cards = {};
    IntT bet = 0;
    int type = High;

    Hand(std::string_view cardsValue, IntT bet) : bet{bet} {
        for (size_t i = 0; i < cards.size(); ++i) {
            cards.at(i) = conversionMap.at(cardsValue.at(i));
        }

        parseType();
    }

    bool operator<(const Hand &other) const {
        if (type != other.type) {
            return type < other.type;
        }

        for (size_t i = 0; i < cards.size(); ++i) {
            auto c1 = cards.at(i);
            auto c2 = other.cards.at(i);
            if (c1 == c2) {
                continue;
            }
            return c1 < c2;
        }

        throw std::runtime_error{"undefined with same hands"};
    }

    void parseType() {
        auto count = std::map<int, int>{};
        for (auto c : cards) {
            ++count[c];
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
        for (auto c : cards) {
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

    for (std::string handStr, bet; file >> handStr >> bet;) {
        auto hand = Hand(handStr, std::stoll(bet));

        set.insert(hand);
    }

    auto sum = 0;
    int rank = 1;
    for (auto hand : set) {
        std::cout << hand.str() << " bet: " << hand.bet
                  << ", type: " << hand.type << std::endl;

        sum += rank * hand.bet;
        ++rank;
    }

    std::cout << "sum: " << sum << std::endl;
}
