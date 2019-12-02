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

	{
		ifstream file(filename);
		getline(file, line);
	}

	regex r("[0-9]+");
	vector <int> numbers;

	for (
			auto it = sregex_iterator(line.begin(), line.end(), r);
			it != sregex_iterator();
			++it) {
		numbers.push_back(stoi(it->str()));
	}

	return numbers;
}



int runProgram(vector<int> numbers, int noun, int verb) {
    
	bool finished = false;

	{
		auto data = numbers;
		
		if (!isTest) {
		    data[1] = noun;
		    data[2] = verb;
		}

		for (int index = 0; !finished; ) {
		    cout << "index " << index << endl;
			auto &command = data[index];
			
			auto datapos1 = data[index + 1];
			cout << "datapos 1 " << datapos1 << endl;
			auto &data1 = data[datapos1];
			auto datapos2 = data[index + 2];
			cout << "datapos 2 " << datapos2 << endl;
			auto &data2 = data[datapos2];
			auto &targetPos = data[index + 3];
			auto &output = data[targetPos];
			switch (command) {
			case 1:
				output = data1 + data2;
				cout << "add" << endl;
				cout << " --> " << output<< endl;
				cout << "targetPos" << targetPos << endl;
				index += 4;
				break;
			case 2:
				output = data1 * data2;
				
				cout << "multiply" << endl;
				index += 4;
				break;
			case 99:
				finished = true;
				cout << "exit" << endl;
				index += 1;
				break;
			default:
				cout << "error" << endl;
				finished = true;
				break;
			}
			
			for (auto n: data) {
			    cout << n << ",";
		    }
		    cout << endl;

		}

        
        return data[0];
	}
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



