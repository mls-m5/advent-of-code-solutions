
//https://adventofcode.com/2018/day/1#part2

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <numeric>
#include <vector>
#include <fstream>
#include <iterator>
using namespace std;

istringstream exampleInput1{"+1 -1"};
istringstream exampleInput2{"+3 +3 +4 -2 -4"};
istringstream exampleInput3{"-6 +3 +8 +5 -6"};
istringstream exampleInput4{"+7 +7 -2 -7 -4"};


auto loadNumbers(istream &stream) {
	return vector <int> (
		istream_iterator<int>(stream),
		istream_iterator<int>()
	);
}


int findDuplicate(istream &stream) {
    vector<int> l = loadNumbers(stream);
    map<int, int> usedFrequencies;
    bool running = true;
    int sum = 0;

    while (running) {
        for (auto n: l) {
            sum += n;
            auto &f = usedFrequencies[sum];
            ++f;
            if (f > 1) {
                return sum;
            }

        }
    }

    return -1; // Should never be reached
}

int main(int argc, char **argv) {
	{
		// Part 1
		ifstream inputStream("1.txt");
		auto nums = loadNumbers(inputStream);
		cout << "final sum: " <<  accumulate(nums.begin(), nums.end(), 0) << endl;
	}

	{
		ifstream inputStream("1.txt");

		// Tests
		cout << "ex1: " << findDuplicate(exampleInput1) << endl;
		cout << "ex2: " << findDuplicate(exampleInput2) << endl;
		cout << "ex3: " << findDuplicate(exampleInput3) << endl;
		cout << "ex4: " << findDuplicate(exampleInput4) << endl;

		// The real result
		cout << "final: " << findDuplicate(inputStream) << endl;
	}

	return 0;
}





