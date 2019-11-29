/*
 * 5.cpp
 *
 *  Created on: 28 nov. 2019
 *      Author: Mattias Larsson Sköld
 */


#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <chrono>
#include <thread>
#include <future>

using namespace std;
using namespace chrono;

//#define isTest

constexpr auto multithreaded = true;

constexpr auto casedist = 'a' - 'A';

string react(string polymer) {
	bool running = true;
	while (running) {
		running = false;
		for (int i = 0; i < polymer.size() - 1; ++i) {
			int c1 = polymer[i];
			int c2 = polymer[i + 1];
			if (abs(c1 - c2) == casedist) {
				polymer.erase(i, 2);

				running = true;
			}
		}
	}
	return polymer;
}

int reactWithHelp(string polymer, char c) {
	auto tmpPolymer = polymer;
	char C = c + casedist;
	tmpPolymer.erase(remove_if(tmpPolymer.begin(), tmpPolymer.end(), [c, C](auto letter){
		return letter == c || letter == C;
	}), tmpPolymer.end());
	tmpPolymer = react(tmpPolymer);

	return tmpPolymer.size();
}

int main(int argc, char **argv) {
	string polymer;
	{
#ifdef isTest
		ifstream file("5ex.txt");
#else
		ifstream file("5.txt");
#endif
		file >> polymer;
	}

	auto processedPolymer = react(polymer);

	cout << "Answer 1: final length without help " << processedPolymer.size() << endl;

	map<char, int> lengths;


	auto start = high_resolution_clock::now();

	if (!multithreaded) {
		for (char c = 'A'; c <= 'Z'; ++c) {
			auto len = reactWithHelp(polymer, c);
			if (len < processedPolymer.size()) {
				lengths[c] = len;
			}
		}
	}
	else {
		// In this case I'm not multithreading because i need, but
		// because it is possible to measure the result
		map<char, future<int>> futureLengths;
		for (char c = 'A'; c <= 'Z'; ++c) {
			futureLengths[c] = async(reactWithHelp, polymer, c);
		}

		auto endThreadStart = high_resolution_clock::now();
		cout << "started threads in : " << duration_cast<milliseconds>(endThreadStart - start).count() << endl;
		for (auto &l: futureLengths) {
			lengths[l.first] = l.second.get();
		}
	}


	auto end = high_resolution_clock::now();

	auto el = min_element(lengths.begin(), lengths.end(), [] (auto a, auto b) {
		return a.second < b.second;
	});

	auto end2 = high_resolution_clock::now();

	auto t1 = duration_cast<milliseconds>(end - start);
	auto t2 = duration_cast<milliseconds>(end2 - end);

	// optimized: 386ms for single thread, 141s multithreaded --> more than 2,5x speedup
	// unoptimized debug: 2.4s for single thread, 0,69s multithreaded --> more than 3x speedup
	// conclusion: Optimization flags helped more than multithreading in this case
	cout << "time for helping algorithm = " << t1.count() << "ms, for getting min = " << t2.count() << "ms" << endl;

	cout << "Answer 2: final length with help (" << el->first << "): " << el->second << endl;
}



