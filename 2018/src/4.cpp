/*
 * 4.cpp
 *
 *  Created on: 28 nov. 2019
 *      Author: Mattias Larsson Sköld
 */

//#define isTest

#include <iostream>
#include <fstream>
#include <regex>
#include <iterator>
#include <ctime>
#include <locale>
#include <map>

using namespace std;

struct Entry {
	enum Action {
		ShiftStart,
		Sleep = 1,
		Awake = -1,
	};

	int yy = 0, mm = 0, dd = 0;
	int h = 0, m = 0;
	int guard = -1;

	Action action = ShiftStart;

	string source;

	Entry(string line) {
		source = line;
		regex r("[0-9]+");
		transform(
				sregex_iterator(line.begin(), line.end(), r),
				sregex_iterator(),
				&yy, // Using pointers as iterators
				[] (smatch match) {
			return stoi(match.str());
		});

		auto description = line.substr(19);
		if (description == "falls asleep") {
			action = Sleep;
		}
		else if (description == "wakes up") {
			action = Awake;
		}
	}

	void print() {
		// Notice that printf is used here only because it is easier for
		// this large number of arguments
		printf("%d-%02d-%02d %02d:%02d\t- \t#%d %d\n",
				yy, mm, dd,     h, m,       guard, action);
	}
};

int main(int argc, char **argv) {
#ifdef isTest
	ifstream input("4ex.txt");
#else
	ifstream input("4.txt");
#endif

	vector<Entry> entries;
	for (string line; getline(input, line);) {
		if (line.empty()) {
			continue;
		}
		entries.emplace_back(line);

		entries.back().print();
	}

	sort(entries.begin(), entries.end(), [] (Entry e1, Entry e2) {
		return e1.source < e2.source;
	});

	cout << "--" << endl;

	int lastGuard;
	map<int, int> sleepTime;
	for (auto &entry: entries) {
		if (entry.guard == -1) {
			entry.guard = lastGuard;
		}
		else {
			lastGuard = entry.guard;
		}

		if (entry.action == entry.Awake) {
			sleepTime[lastGuard] += entry.m;
		}
		else if (entry.action == entry.Sleep) {
			sleepTime[lastGuard] -= entry.m;
		}
		entry.print();
	}

	auto sleepiestGuardIt = max_element(sleepTime.begin(), sleepTime.end(),
			[] (auto a, auto b) {
		return a.second < b.second;
	});

	auto sleepiestGuard = sleepiestGuardIt->first;
	auto longestSleep = sleepiestGuardIt->second;

	cout << "#" << sleepiestGuard << " slept most with " << longestSleep << "m" << endl;

	vector <int> sleptMinutes(60);

	// <guard, clock minute> slept minute
	map<pair<int, int>, int> sleptMap;
	// guard, slept minutes
	map<int, int> guardSleepiness;
	for (auto &entry: entries) {
		if (entry.guard == sleepiestGuard) {
			for (int i = entry.m; i < 60; ++i) {
				guardSleepiness[entry.guard] += entry.action;
				sleptMap[{entry.guard, i}] += entry.action;
			}
			if (entry.action) {
				for (int i = entry.m; i < 60; ++i) {
					sleptMinutes[i] += entry.action;
				}
			}
		}
	}

	auto maxSleepyGuard = max_element(sleptMinutes.begin(), sleptMinutes.end());

	auto sleepiestTime = (maxSleepyGuard - sleptMinutes.begin());

	cout << "the guard sleeps most at 00:" << sleepiestTime << endl;
	cout << "answer 1: " << sleepiestTime * sleepiestGuard << endl;

	// Part 2


	auto maxElement = max_element(sleptMap.begin(), sleptMap.end(), [] (auto a, auto b) {
		return a.second < b.second;
	});

	cout << "#" << maxElement->first.second << " slept " << maxElement->second;
	cout << " during minute " << maxElement->first.second << endl;
	cout << "answer 2: " << maxElement->first.first * maxElement->first.second;
}




