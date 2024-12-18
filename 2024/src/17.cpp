#include <algorithm>
#include <array>
#include <cassert>
#include <format>
#include <fstream>
#include <functional>
#include <iomanip>
#include <ios>
#include <iostream>
#include <istream>
#include <map>
#include <numeric>
#include <ranges>
#include <ratio>
#include <sstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

using std::ranges::iota_view;
using Int = __int128_t;
struct Cpu;

using OpFT = void (*)(Cpu &);

using OpListT = std::array<OpFT, 8>;

std::ostream &operator<<(std::ostream &stream, __int128 value) {
    auto str = std::string{};
    for (; value; value /= 10) {
        str.push_back('0' + static_cast<char>(value % 10));
    }

    std::ranges::reverse(str);
    return stream << str;
}

struct Cpu {
    Int a = 0;
    Int b = 0;
    Int c = 0;

    Int pc = 0;

    std::vector<char> program;
    std::vector<char> outputValues;
    std::vector<int> usedOperations;

    void clear(Int aValue) {
        a = aValue;
        b = 0;
        c = 0;
        pc = 0;
        outputValues.clear();
        usedOperations.clear();
    }

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
                                 resultString());
    }

    void printProgram() const {
        std::cout << "program:";
        for (auto p : program) {
            std::cout << static_cast<int>(p) << ",";
        }
        std::cout << "\n";
    }

    void printUsedOperations() const {
        for (auto o : usedOperations) {
            std::cout << o << " ";
        }
        std::cout << std::endl;
    }

    void output(Int value) {
        outputValues.push_back(value);
    }

    std::string resultString() const {
        auto ret = std::string{};

        if (outputValues.empty()) {
            return ret;
        }

        for (auto o : outputValues) {
            ret += std::to_string(static_cast<int>(o)) + ",";
        }
        ret.pop_back();
        return ret;
    }

    void step(bool trace) {
        auto instruction = program.at(pc);

        if (trace) {
            usedOperations.push_back(instruction);
        }

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

    void run(bool print,
             bool haltBasedOnOutputValue = false,
             bool trace = false) {
        for (; pc < program.size();) {
            step(trace);
            if (print) {
                printState();
            }

            if (haltBasedOnOutputValue) {
                if (outputValues.size() > program.size()) {
                    break;
                }
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
    assert(cpu.resultString() == "0,1,2");
}

void test3() {
    auto ss = std::istringstream{R"_(Register A: 2024
Register B: 0
Register C: 0

0,1,5,4,3,0
)_"};

    auto cpu = Cpu{ss};
    cpu.run(false);
    assert(cpu.resultString() == "4,2,5,6,7,7,7,7,3,1,0");
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

void test6() {
    auto ss = std::istringstream{R"_(Register A: 2024
Register B: 0
Register C: 0

Program: 0,3,5,4,3,0
)_"};

    auto cpu = Cpu{ss};
    cpu.a = 117440;
    cpu.run(false);
    cpu.printState();
    assert(cpu.program == cpu.outputValues);
}

auto &printBinary(Int i) {
    auto ret = std::string{};
    for (; i; i >>= 1) {
        ret += ((i & 1) ? '1' : '0');
    }
    std::reverse(ret.begin(), ret.end());
    std::cout << ret;
    return std::cout;
}

int main(int argc, char *argv[]) {
    auto file = std::ifstream{std::string{"data/17"} +
                              (argc <= 1 ? ".txt" : "-test.txt")};

    test1();
    test2();
    test3();
    test4();
    test5();
    test6();

    auto cpu = Cpu{file};
    auto cpuCopy = cpu;
    cpu.printState();
    cpu.printProgram();

    cpu.run(argc > 1);

    cpu.printState();
    std::cout << cpu.resultString() << std::endl;

    {
        auto result = cpu.resultString();
        for (size_t i = 0; i < result.size(); i += 2) {
            std::cout << result.at(i);
        }
        std::cout << std::endl;

        cpu.printUsedOperations();
    }

    {
        std::cout << "Trying to find the right value of the cpus register\n";
        auto testCpu = cpuCopy;

        std::cout << "---" << std::endl;

        auto charMap = std::map<char, char>{};

        for (auto in : iota_view{1uz, (1uz << 3)}) {
            // auto i = in << 4;
            auto i = in;
            std::cout << "Ain: " << i << "\n";
            testCpu.clear(i);
            testCpu.run(false);
            testCpu.printState();
            testCpu.printProgram();

            if (charMap.find(i) == charMap.end()) {
                charMap[testCpu.outputValues.front()] = i;
            }

            std::cout << std::endl;
        }

        std::cout << std::flush;
        Int result2 = {};

        auto prog = cpu.program;

        for (auto in : iota_view{0uz, 1024uz * 2}) {

            auto i = (in << 4) + 2;
            testCpu.clear(i);
            testCpu.run(false);
            // testCpu.printState();
            // testCpu.printProgram();
            std::cout << "i: " << std::setw(16);
            printBinary(i) << ": " << testCpu.resultString() << std::endl;
        }

        return 0;

        for (auto index : iota_view{0uz, prog.size()}) {
            // result2 <<= 4;
            for (auto in : iota_view{1uz, 8uz}) {
                // auto current = (in << 4 * (index + 1));
                // auto current = in;
                auto i = (in << 4 * index) + result2;
                testCpu.clear(i);
                testCpu.run(false);
                std::cout << "Ain: " << std::setw(20) << i << ": "
                          << "\t";
                std::cout << "R: " << testCpu.resultString() << "\n";
                std::cout << std::endl;

                if (testCpu.outputValues.at(index) ==
                    testCpu.program.at(index)) {
                    result2 = i;
                    break;
                }
                // testCpu.printState();
                // testCpu.printProgram();
            }
        }

        // for (int i = cpu.program.size() - 1; i >= 0; --i) {
        //     auto c = cpu.program.at(i);
        //     result2 <<= 3;
        //     result2 += charMap.at(c);
        // }
        // std::cout << "final cpu state:\n";

        // testCpu.clear(result2);
        // testCpu.run(false);
        // testCpu.printState();
        // testCpu.printProgram();

        // std::cout << std::format("Part 2: {}\n", result2);

        // std::cout.flush();
    }

    // 367057314  is not right
    // 14892071842402 is to low
}
