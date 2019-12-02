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

int main(int argc, char **argv) {
	const bool isTest = argc > 1 && string(argv[1]) == "--test";

	const auto numbers = loadNumbers(isTest? "data/2.ex.txt": "data/2.txt");

	bool finished = false;

	{
		auto data = numbers;
		
		if (!isTest) {
		    data[1] = 12;
		    data[2] = 2;
		}

		for (int index = 0; !finished; index += 4) {
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
				break;
			case 2:
				output = data1 * data2;
				
				cout << "add" << endl;
				break;
			case 99:
				finished = true;
				cout << "exit" << endl;
				break;
			default:
				cout << "error" << endl;
				break;
			}
			
			for (auto n: data) {
			    cout << n << ",";
		    }
		    cout << endl;

		}
		cout << "answer 1: " << data[0] << endl;

		/// 190687 är fel, 3790689 är rätt
	}
}



