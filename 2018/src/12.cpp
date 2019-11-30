/*
 * 12.cpp
 *
 *  Created on: 30 nov. 2019
 *      Author: Mattias Larsson Sköld
 */

#include <iostream>
#include <map>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;



class PotState {
public:

	PotState(string filename) {
		ifstream file(filename);

		string line;
		getline(file, line);
		state = line;

		getline(file, line); // Remove empty line

		lookup = {};
		for (; getline(file, line);) {
			lookup[string(line.begin(), line.begin() + 5)] = line.back();
		}

	}

	void print() const {
		cout << string(20 + start, ' ');
		cout << state;
		cout << "  " << sum() << endl;
	}

	void expand() {
		for (int i = 0; i < 3; ++i) {
			if (state[i] == '#') {
				start -= 3;
				state.insert(0, "...");
				break;
			}
		}
		for (int i = 0; i < 3; ++i) {
			if (state[state.size() - i - 1] == '#') {
				state += "...";
			}
		}
	}


	void step() {
		expand();
		auto oldState = state;

		for (int i = 2; i < state.size() - 2; ++i) {
			auto name = string(
					oldState.begin() + i - 2,
					oldState.begin() + i + 3);
			auto &value = lookup[name];
			if (value == 0) {
				value = '.';
			}
			state[i] = value;
		}
	}

	int sum() const {
		int sum = 0;
		for (int i = 0; i < state.size(); ++i) {
			auto potValue = i + start;
			auto value = potValue * (state[i] == '#');
			sum += value;
		}
		return sum;
	}

	string state;
	map<string, char> lookup;
	int start = 0;
};




int main(int argc, char **argv) {
	bool isTest = false;
	if (argc > 1 && string(argv[1]) == "--test") {
		isTest = true;
	}

	const PotState origin(isTest? "12ex.txt": "12.txt");
	{
		auto state = origin;
		cout << setw(5) << 0;
		state.print();

		for (int i = 1; i <= 20; ++i) {
			cout << setw(5) << i;
			state.step();
			state.print();
		}
	}

	// Part 2

	{
		cout << "part 2 --- " << endl;
		auto state = origin;
		int previousSum = state.sum();
		int diff = 0;
		for (int i = 1; i <= 200; ++i) {
			cout << setw(5) << i;
			state.print();
			state.step();
			auto sum = state.sum();
			diff = sum - previousSum;
			previousSum = sum;
		}
		cout << "sum at generation 200: " << previousSum << " with diff " << diff << endl;

		long longRangeSum = (50000000000l - 200) * diff + previousSum;
		cout << "answer 2: sum = " << longRangeSum;
	}
}




