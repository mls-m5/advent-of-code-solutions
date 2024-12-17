#include <array>
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <istream>
#include <ranges>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using std::ranges::iota_view;
using Int = int;
struct Cpu;

using OpFT = void (*)(Cpu &);

using OpListT = std::array<OpFT, 8>;

struct Cpu {
    Int a = 0;
    Int b = 0;
    Int c = 0;

    Int pc = 0;

    std::vector<char> program;
    std::string result;

    static Int readRegister(std::istream &stream) {
        std::string line;
        std::getline(stream, line);
        return std::stoi(line.substr(12));
    }

    Cpu(std::istream &stream)
        : a{readRegister(stream)}, b{readRegister(stream)},
          c{readRegister(stream)} {
        std::string line;
        stream >> line >> line; // Skip "Program:"
        for (size_t i = 0; i < line.size(); i += 2) {
            program.push_back(line.at(i) - '0');
        }
    }

    char opcode() const {
        return program.at(pc + 1);
    }

    Int literal() const {
        return opcode();
    }

    Int combo() const {
        auto code = opcode();

        switch (code) {
        case 0:
        case 1:
        case 2:
        case 3:
            return code;

        case 4:
            return a;
        case 5:
            return b;
        case 6:
            return c;
        }
        printState();
        throw std::runtime_error{"invalid opcode " +
                                 std::to_string(static_cast<int>(code))};
    }

    void printState() const {
        std::cout << std::format(R"_(Register A: {}
Register B: {}
Register C: {}
Program counter: {}
Result: {}
)_",
                                 a,
                                 b,
                                 c,
                                 pc,
                                 result);
    }

    void printProgram() const {
        for (auto p : program) {
            std::cout << static_cast<int>(p) << ", ";
        }
        std::cout << "\n";
    }

    void output(Int value) {
        // std::cout << value << ",";
        result += std::to_string(value) + ",";
    }

    void step() {
        auto instruction = program.at(pc);

        switch (instruction) {
        case 0: // adv
            a = a >> combo();
            break;
        case 1: // bxl
            b = b ^ literal();
            break;
        case 2: // bst
            b = combo() % 8;
            break;
        case 3:
            // jnz
            if (!a) {
                break;
            }
            pc = literal() - 2;
            break;
        case 4: // bxc
            b = b ^ c;
            break;
        case 5: // out
            output(combo() % 8);
            break;
        case 6: // bdv
            b = a >> combo();
            break;
        case 7: // cdv
            c = a >> combo();
            break;
        }

        pc += 2;
    }

    void run(bool print) {
        for (; pc < program.size();) {
            step();
            if (print) {
                printState();
            }
        }
    }
};

void test1() {
    auto ss = std::istringstream{R"_(Register A: 0
Register B: 0
Register C: 9

2,6
)_"};

    auto cpu = Cpu{ss};
    cpu.run(false);
    assert(cpu.b == 1);
}

void test2() {
    auto ss = std::istringstream{R"_(Register A: 10
Register B: 0
Register C: 0

5,0,5,1,5,4
)_"};

    auto cpu = Cpu{ss};
    cpu.run(false);
    assert(cpu.result == "0,1,2,");
}

void test3() {
    auto ss = std::istringstream{R"_(Register A: 2024
Register B: 0
Register C: 0

0,1,5,4,3,0
)_"};

    auto cpu = Cpu{ss};
    cpu.run(false);
    assert(cpu.result == "4,2,5,6,7,7,7,7,3,1,0,");
    assert(cpu.a == 0);
}

void test4() {
    auto ss = std::istringstream{R"_(Register A: 0
Register B: 29
Register C: 0

1,7
)_"};

    auto cpu = Cpu{ss};
    cpu.run(false);
    assert(cpu.b == 26);
}

void test5() {
    auto ss = std::istringstream{R"_(Register A: 0
Register B: 2024
Register C: 43690

4,0
)_"};

    auto cpu = Cpu{ss};
    cpu.run(false);
    assert(cpu.b == 44354);
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/17"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    test1();
    test2();
    test3();
    test4();
    test5();

    auto cpu = Cpu{file};
    cpu.printState();
    cpu.printProgram();

    cpu.run(argc > 1);

    cpu.printState();
    std::cout << cpu.result << std::endl;

    for (size_t i = 0; i < cpu.result.size(); i += 2) {
        std::cout << cpu.result.at(i);
    }
    std::cout << std::endl;

    // 367057314  is not right
}
