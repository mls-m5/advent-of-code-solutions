#if 0

c++ $0 -o /tmp/$0
/tmp/$0
rm /tmp/$0

exit
#endif

// A c++ program that compiles itself and then runs
// can be executed as a script

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

using namespace std::literals;

auto srcContent = R"_(
#include "input.h"
#include <algorithm>
#include <iostream>
#include <regex>

int main(int argc, char *argv[]) {
    auto file = openFile("0x", argc, argv);

}

)_";

void createDay(std::string day, std::filesystem::path src) {

    std::ofstream{src} << srcContent;
    std::ofstream{"data/" + day + ".txt"} << "";
    std::ofstream{"data/" + day + "-test.txt"} << "";

    std::ofstream{"CMakeLists.txt", std::ios::app} << "day(" << day << ")\n";

    std::cout << "created day " << day << ": " << src << "\n";
}

int main() {
    for (int i = 1; i < 25; ++i) {
        auto intStr = ((i < 10) ? "0"s : ""s) + std::to_string(i);
        auto path = std::filesystem::path{"src/2022-"s + intStr + ".cpp"};
        if (!std::filesystem::exists(path)) {
            createDay(intStr, path);
            break;
        }
    }
}
