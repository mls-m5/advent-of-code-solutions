/*
 * 2.cpp
 *
 *  Created on: 2 dec. 2019
 *      Author: Mattias Larsson Sköld
 */


#include <iostream>
#include <regex>
#include <iterator>
#include <string>
#include <fstream>
#include <cassert>
#include <array>

using namespace std;


bool isTest = false;

vector <int> loadNumbers(string filename) {
	string line;

	ifstream file(filename);

	vector <int> numbers;
	for (string numstr; getline(file, numstr, ',');) {
		numbers.push_back(stoi(numstr));
	}
	return numbers;
}


int getHundred(int n) {
	return (n / 100) % 10;
}

int getThousand(int n) {
	return (n / 1000) % 10;
}

int runProgram(vector<int> numbers, vector<int> userInput) {
	bool finished = false;
	int outputValue = 0;

	for (int index = 0; !finished; ) {
		auto commandValue = numbers[index];
		auto mod1 = getHundred(commandValue);
		auto mod2 = getThousand(commandValue);

		auto command = commandValue % 100;
		auto *input1 = &numbers[numbers[index + 1]];
		if (mod1) {
			input1 = &numbers[index + 1];
		}
		auto *input2 = &numbers[numbers[index + 2]];
		if (mod2) {
			input2 = &numbers[index + 2];
		}
		auto &output = numbers[numbers[index + 3]]; // Always in position mode
		switch (command) {
		case 1: // add
			output = *input1 + *input2;
			index += 4;
			break;
		case 2: // multiply
			output = *input1 * *input2;
			index += 4;
			break;
		case 3: // input
			if (mod1) {
				throw runtime_error("cannot write input to absolute value");
			}
			*input1 = userInput.front();
			if (isTest) cout << "input> " << userInput.front() << endl;
			userInput.erase(userInput.begin());

			index += 2;
			break;
		case 4: // output
			if (isTest) cout << "output: " << *input1 << endl;
			outputValue = *input1;
			index += 2;
			break;
		case 5: // jump if true
			if (*input1) {
				index = *input2;
			}
			else {
				index += 3;
			}
			break;
		case 6: // jump if false
			if (!*input1) {
				index = *input2;
			}
			else {
				index += 3;
			}
			break;
		case 7: // less than
			output = *input1 < *input2;
			index += 4;

			break;
		case 8: // equals
			output = *input1 == *input2;
			index += 4;

			break;
		case 99:
			finished = true;
			index += 1;
			break;
		default:
			finished = true;
			runtime_error("unknown command");
			break;
		}

	}

	if (isTest) {
		for (auto n: numbers) {
			cout << n << ",";
		}
		cout << endl;
	}

	return outputValue;
}

int calculateSignal(
		const vector<int> &program,
		const vector<int> &settings,
		int inputValue) {
	int currentValue = 0;
	for (auto s: settings) {
		currentValue = runProgram(program, {s, currentValue});
	}
	return currentValue;
}

// Ugly brute force sollution
bool isValidSetting(const vector<int> setting) {
	array<int, 5> usedNumbers = {};
	for (auto s: setting) {
		usedNumbers[s] = true;
	}

	for (auto used: usedNumbers) {
		if (!used) {
			return false;
		}
	}
	return true;
}

int findMaxSignal(const vector <int> &program, int inputValue = 0) {
	vector<int> settings(5);
	vector<int> bestSetting;
	int highestValue = 0;

	bool running = true;

	auto increaseSetting = [&]() {
		int carry = 1;
		for (auto &s: settings) {
			if (carry) {
				s += 1;
				if (s > 4) {
					carry = 1;
					s = 0;
				}
				else {
					carry = 0;
					break;
				}
			}
			else {
				break;
			}
		}
		return carry;
	};

	while(running && !isValidSetting(settings)) {
		running = !increaseSetting();
	}

	for (;running;) {
		auto value = calculateSignal(program, settings, inputValue);
		if (isTest) {
			if (value > highestValue) {
				bestSetting = settings;
			}
		}
		highestValue = max(value, highestValue);

		do {
			running = !increaseSetting();
		} while(running && !isValidSetting(settings));
	}

	if (isTest) {
		for (auto s: bestSetting) {
			cout << s << " ";
		}
		cout << endl;
	}

	return highestValue;
}

int main(int argc, char **argv) {
	isTest = argc > 1 && string(argv[1]) == "--test";

	if (isTest) {
		// Old test cases
		runProgram({3,0,4,0,99}, {1});
		runProgram({1002,4,3,4,33}, {1});
		runProgram({1101,100,-1,4,0}, {1});

		cout << "tests for part 2" << endl;
		runProgram({3,9,8,9,10,9,4,9,99,-1,8}, {1});
		runProgram({3,9,8,9,10,9,4,9,99,-1,8}, {8});

		cout << "test2" << endl;
		runProgram({3,9,7,9,10,9,4,9,99,-1,8}, {1});
		runProgram({3,9,7,9,10,9,4,9,99,-1,8}, {10});

		cout << "test3" << endl;
		runProgram({3,3,1108,-1,8,3,4,3,99}, {1});
		runProgram({3,3,1108,-1,8,3,4,3,99}, {8});

		cout << "test4" << endl;
		runProgram({3,3,1107,-1,8,3,4,3,99}, {1});
		runProgram({3,3,1107,-1,8,3,4,3,99}, {10});

		// Tests for day 7
		assert(43210 == calculateSignal(
				{3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0},
				{4,3,2,1,0},
				0));

		assert(54321 == calculateSignal(
				{3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0},
				{0,1,2,3,4},
				0));

		assert(65210 == calculateSignal(
				{3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0},
				{1,0,4,3,2},
				0));


		assert(43210 == findMaxSignal(
				{3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0},
				0));

		assert(54321 == findMaxSignal(
						{3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0},
						0));

	}
	else {

		const auto numbers = loadNumbers(isTest? "data/7.ex.txt": "data/7.txt");

		cout << "answer 1: " << findMaxSignal(numbers, 0);
	}
}



