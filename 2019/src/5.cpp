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

int runProgram(vector<int> numbers/*, int noun, int verb*/, int input) {
//	if (!isTest) {
//		numbers[1] = noun;
//		numbers[2] = verb;
//	}



	bool finished = false;
	for (int index = 0; !finished; ) {
		auto commandValue = numbers[index];
		auto mod1 = getHundred(commandValue);
		auto mod2 = getThousand(commandValue);
		cout << "commandvalue " << commandValue << endl;

//		auto command = numbers[index];
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
		case 1:
			output = *input1 + *input2;
			index += 4;
			break;
		case 2:
			output = *input1 * *input2;
			index += 4;
			break;
		case 3:
			index += 2;
			if (mod1) {
				throw runtime_error("cannot write input to absolute value");
			}
			output = input;
			cout << "input> " << input << endl;

			break;
		case 4:
			index += 2;
			cout << "output: " << *input1 << endl;
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
	}
	else {

		const auto numbers = loadNumbers(isTest? "data/5.ex.txt": "data/5.txt");


		runProgram(numbers, 1);
//    cout << "answer 1: " << runProgram(numbers, 12, 2) << endl;
//
//    for (int noun = 0; noun < 100; ++noun) {
//        for (int verb = 0; verb < 100; ++verb) {
//            if (19690720 == runProgram(numbers, noun, verb)) {
//                cout << "noun: " << noun << " verb " << verb << endl;
//                cout << "answer 2: " << noun * 100 + verb << endl;
//                return 0;
//            }
//        }
//    }
	}
}



