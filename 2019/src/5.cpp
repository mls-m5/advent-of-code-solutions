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

int runProgram(vector<int> numbers/*, int noun, int verb*/, int userInput) {
	bool finished = false;
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
			*input1 = userInput;
			cout << "input> " << userInput << endl;

			index += 2;
			break;
		case 4: // output
			cout << "output: " << *input1 << endl;
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

	return numbers[0];
}


int main(int argc, char **argv) {
	isTest = argc > 1 && string(argv[1]) == "--test";

	if (isTest) {
		runProgram({3,0,4,0,99}, 1);
		runProgram({1002,4,3,4,33}, 1);
		runProgram({1101,100,-1,4,0}, 1);

		cout << "tests for part 2" << endl;
		runProgram({3,9,8,9,10,9,4,9,99,-1,8}, 1);
		runProgram({3,9,8,9,10,9,4,9,99,-1,8}, 8);

		cout << "test2" << endl;
		runProgram({3,9,7,9,10,9,4,9,99,-1,8}, 1);
		runProgram({3,9,7,9,10,9,4,9,99,-1,8}, 10);

		cout << "test3" << endl;
		runProgram({3,3,1108,-1,8,3,4,3,99}, 1);
		runProgram({3,3,1108,-1,8,3,4,3,99}, 8);

		cout << "test4" << endl;
		runProgram({3,3,1107,-1,8,3,4,3,99}, 1);
		runProgram({3,3,1107,-1,8,3,4,3,99}, 10);
	}
	else {

		const auto numbers = loadNumbers(isTest? "data/5.ex.txt": "data/5.txt");


		runProgram(numbers, 1);


		// Part 2

		runProgram(numbers, 5);
	}
}



