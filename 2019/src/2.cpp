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


int runProgram(vector<int> numbers, int noun, int verb) {
	if (!isTest) {
		numbers[1] = noun;
		numbers[2] = verb;
	}

	bool finished = false;
	for (int index = 0; !finished; index += 4) {
		auto command = numbers[index];
		auto input1 = numbers[numbers[index + 1]];
		auto input2 = numbers[numbers[index + 2]];
		auto &output = numbers[numbers[index + 3]];
		switch (command) {
		case 1:
			output = input1 + input2;
			break;
		case 2:
			output = input1 * input2;
			break;
		case 99:
			finished = true;
			break;
		default:
			finished = true;
			break;
		}

	}

	return numbers[0];
}


int main(int argc, char **argv) {
	isTest = argc > 1 && string(argv[1]) == "--test";

	const auto numbers = loadNumbers(isTest? "data/2.ex.txt": "data/2.txt");

    
    cout << "answer 1: " << runProgram(numbers, 12, 2) << endl;
    
    for (int noun = 0; noun < 100; ++noun) {
        for (int verb = 0; verb < 100; ++verb) {
            if (19690720 == runProgram(numbers, noun, verb)) {
                cout << "noun: " << noun << " verb " << verb << endl;
                cout << "answer 2: " << noun * 100 + verb << endl;
                return 0;
            }
        }
    }
}



