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

int runProgram(vector<int> &numbers, int &index, vector<int> &userInput, bool &finished);

int runProgram(vector<int> numbers, vector<int> userInput) {
	int index = 0;
	int value;
	bool finished = false;

	while (!finished) {
		int newValue = runProgram(numbers, index, userInput, finished);
		if (!finished) {
			value = newValue;
		}
	}

	return value;
}

int runProgram(vector<int> &numbers, int &index, vector<int> &userInput, bool &finished) {
	int outputValue = 0;

	for (;!finished; ) {
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
			return outputValue;
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
		const vector<int> &settings) {
	int currentValue = 0;
	for (auto s: settings) {
		currentValue = runProgram(program, {s, currentValue});
	}
	return currentValue;
}

int calculateSignalFeedback(
		const vector<int> &orgProgram,
		const vector<int> &settings
		) {
	int currentValue = 0;
	vector<vector<int>> programs;
	for (int i = 0; i < 5; ++i) {
		programs.push_back(orgProgram);
	}

	array<bool, 5> finished = {};
	array<int, 5> indices = {};
	array<vector<int>, 5> inputs;

	for (int i = 0; i < inputs.size(); ++i) {
		inputs.at(i).push_back(settings.at(i));
	}

	inputs.front().push_back(0);

	while (!finished.back()) {
		for (int i = 0; i < 5; ++i) {
			auto &program = programs.at(i);
			auto &f = finished.at(i);
			auto &input = inputs.at(i);
			auto &output = inputs.at((i + 1) % 5);
			auto &index = indices.at(i);

			auto value = runProgram(program, index, input, f);
			if (!f) {
				currentValue = value;
				output.push_back(value);
			}
			else {
				if (isTest) cout << "program has finished" << endl;
			}
		}
	}

	return currentValue;
}

// Ugly brute force solution
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

vector <int> convertOldSettings(vector <int> oldSettings) {
	for (auto &s: oldSettings) {
		s += 5;
	}
	return oldSettings;
}

int findMaxSignal(const vector <int> &program, bool useFeedback = false) {
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
		int value = 0;
		if (useFeedback) {
			value = calculateSignalFeedback(program, convertOldSettings(settings));
		}
		else {
			value = calculateSignal(program, settings);
		}
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
				{4,3,2,1,0}));

		assert(54321 == calculateSignal(
				{3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0},
				{0,1,2,3,4}));

		assert(65210 == calculateSignal(
				{3,31,3,32,1002,32,10,32,1001,31,-2,31,1007,31,0,33,1002,33,7,33,1,33,31,31,1,32,31,31,4,31,99,0,0,0},
				{1,0,4,3,2}));


		assert(43210 == findMaxSignal(
				{3,15,3,16,1002,16,10,16,1,16,15,15,4,15,99,0,0}));

		assert(54321 == findMaxSignal(
						{3,23,3,24,1002,24,10,24,1002,23,-1,23,101,5,23,23,1,24,23,23,4,23,99,0,0}));


		// Part2
		assert(139629729 == calculateSignalFeedback(
				{3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5},
				{9,8,7,6,5}));

		assert(18216 == calculateSignalFeedback(
				{3,52,1001,52,-5,52,3,53,1,52,56,54,1007,54,5,55,1005,55,26,1001,54,
			-5,54,1105,1,12,1,53,54,53,1008,54,0,55,1001,55,1,55,2,53,55,53,4,
			53,1001,56,-1,56,1005,56,6,99,0,0,0,0,10},
				{9,7,8,5,6}));


		assert(139629729 == findMaxSignal(
				{3,26,1001,26,-4,26,3,27,1002,27,2,27,1,27,26,27,4,27,1001,28,-1,28,1005,28,6,99,0,0,5}, true));

	}
	else {
		const auto numbers = loadNumbers(isTest? "data/7.ex.txt": "data/7.txt");

		cout << "answer 1: " << findMaxSignal(numbers) << endl;

		cout << "answer 2: " << findMaxSignal(numbers, true) << endl;
	}
}



